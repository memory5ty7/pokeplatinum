#include "trainer_data.h"

#include "constants/battle.h"
#include "constants/pokemon.h"
#include "constants/savedata/vars_flags.h"
#include "constants/trainer.h"

#include "struct_defs/trainer.h"

#include "data/trainer_class_genders.h"

#include "charcode_util.h"
#include "field_battle_data_transfer.h"
#include "heap.h"
#include "math.h"
#include "message.h"
#include "narc.h"
#include "party.h"
#include "pokemon.h"
#include "savedata.h"
#include "savedata_misc.h"
#include "strbuf.h"
#include "vars_flags.h"

#include "gender_ratios.h"

static void TrainerData_BuildParty(FieldBattleDTO *dto, int battler, int heapID);

static void AdjustHP(Pokemon *mon, u8 preDamage);
static void AdjustIVs(Pokemon *mon, u8 IVProfile);
static void AdjustStatus(Pokemon *mon, u8 status);
static u32 AdjustGender(Pokemon *mon, u8 nature, u8 gender, u32 trainerGender);

void Trainer_Encounter(FieldBattleDTO *dto, const SaveData *save, int heapID)
{
    Trainer trdata;
    MessageLoader *msgLoader = MessageLoader_Init(MESSAGE_LOADER_NARC_HANDLE, NARC_INDEX_MSGDATA__PL_MSG, 618, heapID);
    const charcode_t *rivalName = MiscSaveBlock_RivalName(SaveData_MiscSaveBlockConst(save));

    for (int i = 0; i < MAX_BATTLERS; i++) {
        if (!dto->trainerIDs[i]) {
            continue;
        }

        Trainer_Load(dto->trainerIDs[i], &trdata);
        dto->trainer[i] = trdata;

        if (trdata.header.trainerType == TRAINER_CLASS_RIVAL) {
            CharCode_Copy(dto->trainer[i].name, rivalName);
        } else {
            Strbuf *trainerName = MessageLoader_GetNewStrbuf(msgLoader, dto->trainerIDs[i]);
            Strbuf_ToChars(trainerName, dto->trainer[i].name, TRAINER_NAME_LEN + 7);
            Strbuf_Free(trainerName);
        }

        TrainerData_BuildParty(dto, i, heapID);
    }

    dto->battleType |= trdata.header.battleType;
    MessageLoader_Free(msgLoader);
}

u32 Trainer_LoadParam(int trainerID, enum TrainerDataParam paramID)
{
    // TODO: can this be trainerheader?
    u32 result;
    Trainer trdata;

    Trainer_Load(trainerID, &trdata);

    switch (paramID) {
    case TRDATA_TYPE:
        result = trdata.header.monDataType;
        break;

    case TRDATA_CLASS:
        result = trdata.header.trainerType;
        break;

    case TRDATA_SPRITE:
        result = trdata.header.sprite;
        break;

    case TRDATA_PARTY_SIZE:
        result = trdata.header.partySize;
        break;

    case TRDATA_ITEM_1:
    case TRDATA_ITEM_2:
    case TRDATA_ITEM_3:
    case TRDATA_ITEM_4:
        result = trdata.header.items[paramID - TRDATA_ITEM_1];
        break;

    case TRDATA_AI_MASK:
        result = trdata.header.aiMask;
        break;

    case TRDATA_BATTLE_TYPE:
        result = trdata.header.battleType;
        break;
    }

    return result;
}

BOOL Trainer_HasMessageType(int trainerID, enum TrainerMessageType msgType, int heapID)
{
    NARC *narc; // must declare up here to match
    u16 offset, data[2];

    BOOL result = FALSE;
    int size = NARC_GetMemberSizeByIndexPair(NARC_INDEX_POKETOOL__TRMSG__TRTBL, 0);
    NARC_ReadFromMemberByIndexPair(&offset, NARC_INDEX_POKETOOL__TRMSG__TRTBLOFS, 0, trainerID * 2, 2);
    narc = NARC_ctor(NARC_INDEX_POKETOOL__TRMSG__TRTBL, heapID);

    while (offset != size) {
        NARC_ReadFromMember(narc, 0, offset, 4, data);

        if (data[0] == trainerID && data[1] == msgType) {
            result = TRUE;
            break;
        }

        if (data[0] != trainerID) {
            break;
        }

        offset += 4;
    }

    NARC_dtor(narc);
    return result;
}

void Trainer_LoadMessage(int trainerID, enum TrainerMessageType msgType, Strbuf *strbuf, int heapID)
{
    NARC *narc; // must declare up here to match
    u16 offset, data[2];

    int size = NARC_GetMemberSizeByIndexPair(NARC_INDEX_POKETOOL__TRMSG__TRTBL, 0);
    NARC_ReadFromMemberByIndexPair(&offset, NARC_INDEX_POKETOOL__TRMSG__TRTBLOFS, 0, trainerID * 2, 2);
    narc = NARC_ctor(NARC_INDEX_POKETOOL__TRMSG__TRTBL, heapID);

    while (offset != size) {
        NARC_ReadFromMember(narc, 0, offset, 4, data);

        if (data[0] == trainerID && data[1] == msgType) {
            MessageBank_GetStrbufFromNARC(NARC_INDEX_MSGDATA__PL_MSG, 617, offset / 4, heapID, strbuf);
            break;
        }

        offset += 4;
    }

    NARC_dtor(narc);

    if (offset == size) {
        Strbuf_Clear(strbuf);
    }
}

void Trainer_Load(int trainerID, Trainer *trdata)
{
    NARC_ReadWholeMemberByIndexPair(trdata, NARC_INDEX_POKETOOL__TRAINER__TRDATA, trainerID);
}

void Trainer_LoadParty(int trainerID, void *trparty)
{
    NARC_ReadWholeMemberByIndexPair(trparty, NARC_INDEX_POKETOOL__TRAINER__TRPOKE, trainerID);
}

u8 TrainerClass_Gender(int trclass)
{
    return sTrainerClassGender[trclass];
}

/**
 * @brief Build the party for a trainer as loaded in the FieldBattleDTO struct.
 *
 * @param dto  The parent FieldBattleDTO struct containing trainer data.
 * @param battler       Which battler's party is to be loaded.
 * @param heapID        Heap on which to perform any allocations.
 */
static void TrainerData_BuildParty(FieldBattleDTO *dto, int battler, int heapID)
{
    // must make declarations C89-style to match
    void *buf;
    int i, j;
    u32 genderMod, rnd, oldSeed;
    u8 ivs;
    Pokemon *mon;

    oldSeed = LCRNG_GetSeed();

    // alloc enough space to support the maximum possible data size
    Party_InitWithCapacity(dto->parties[battler], MAX_PARTY_SIZE);
    buf = Heap_AllocFromHeap(heapID, sizeof(TrainerMonWithMovesAndItem) * MAX_PARTY_SIZE);
    mon = Pokemon_New(heapID);

    u32 trainerGender = TrainerClass_Gender(dto->trainer[battler].header.trainerType);

    Trainer_LoadParty(dto->trainerIDs[battler], buf);

    switch (dto->trainer[battler].header.monDataType) {
    case TRDATATYPE_BASE: {
        TrainerMonBase *trmon = (TrainerMonBase *)buf;
        for (i = 0; i < dto->trainer[battler].header.partySize; i++) {
            u16 species = trmon[i].species & 0x3FF;
            u8 form = (trmon[i].species & 0xFC00) >> TRAINER_MON_FORM_SHIFT;

            u8 abilitySlot = trmon[i].abilitySlot;
            u8 ability = Pokemon_LoadAbilityValue(species, form, abilitySlot);

            u8 nature = trmon[i].nature;
            u32 pid = AdjustGender(mon, nature, trmon[i].gender, trainerGender);

            Pokemon_InitWith(mon, species, trmon[i].level, ivs, TRUE, pid, OTID_NOT_SHINY, 0);
            Pokemon_SetValue(mon, MON_DATA_FORM, &form);
            Pokemon_SetValue(mon, MON_DATA_ABILITY, &ability);

            AdjustHP(mon, trmon[i].preDamage);
            AdjustIVs(mon, trmon[i].ivs);
            AdjustStatus(mon, trmon[i].status);
            

            Pokemon_CalcLevelAndStats(mon);
            Pokemon_SetBallSeal(trmon[i].cbSeal, mon, heapID);
            Party_AddPokemon(dto->parties[battler], mon);
        }

        break;
    }

    case TRDATATYPE_WITH_MOVES: {
        TrainerMonWithMoves *trmon = (TrainerMonWithMoves *)buf;
        for (i = 0; i < dto->trainer[battler].header.partySize; i++) {
            u16 species = trmon[i].species & 0x3FF;
            u8 form = (trmon[i].species & 0xFC00) >> TRAINER_MON_FORM_SHIFT;

            u8 abilitySlot = trmon[i].abilitySlot;
            u8 ability = Pokemon_LoadAbilityValue(species, form, abilitySlot);

            u8 nature = trmon[i].nature;
            u32 pid = AdjustGender(mon, nature, trmon[i].gender, trainerGender);

            Pokemon_InitWith(mon, species, trmon[i].level, ivs, TRUE, pid, OTID_NOT_SHINY, 0);
            Pokemon_SetValue(mon, MON_DATA_FORM, &form);
            Pokemon_SetValue(mon, MON_DATA_ABILITY, &ability);

            for (j = 0; j < 4; j++) {
                Pokemon_SetMoveSlot(mon, trmon[i].moves[j], j);
            }

            AdjustHP(mon, trmon[i].preDamage);
            AdjustIVs(mon, trmon[i].ivs);
            AdjustStatus(mon, trmon[i].status);

            Pokemon_CalcLevelAndStats(mon);
            Pokemon_SetBallSeal(trmon[i].cbSeal, mon, heapID);
            Party_AddPokemon(dto->parties[battler], mon);
        }

        break;
    }

    case TRDATATYPE_WITH_ITEM: {
        TrainerMonWithItem *trmon = (TrainerMonWithItem *)buf;
        for (i = 0; i < dto->trainer[battler].header.partySize; i++) {
            u16 species = trmon[i].species & 0x3FF;
            u8 form = (trmon[i].species & 0xFC00) >> TRAINER_MON_FORM_SHIFT;

            u8 abilitySlot = trmon[i].abilitySlot;
            u8 ability = Pokemon_LoadAbilityValue(species, form, abilitySlot);

            u8 nature = trmon[i].nature;
            u32 pid = AdjustGender(mon, nature, trmon[i].gender, trainerGender);

            Pokemon_InitWith(mon, species, trmon[i].level, ivs, TRUE, pid, OTID_NOT_SHINY, 0);
            Pokemon_SetValue(mon, MON_DATA_HELD_ITEM, &trmon[i].item);
            Pokemon_SetValue(mon, MON_DATA_FORM, &form);
            Pokemon_SetValue(mon, MON_DATA_ABILITY, &ability);

            AdjustHP(mon, trmon[i].preDamage);
            AdjustIVs(mon, trmon[i].ivs);
            AdjustStatus(mon, trmon[i].status);

            Pokemon_CalcLevelAndStats(mon);
            Pokemon_SetBallSeal(trmon[i].cbSeal, mon, heapID);
            Party_AddPokemon(dto->parties[battler], mon);
        }

        break;
    }

    case TRDATATYPE_WITH_MOVES_AND_ITEM: {
        TrainerMonWithMovesAndItem *trmon = (TrainerMonWithMovesAndItem *)buf;
        for (i = 0; i < dto->trainer[battler].header.partySize; i++) {
            u16 species = trmon[i].species & 0x3FF;
            u8 form = (trmon[i].species & 0xFC00) >> TRAINER_MON_FORM_SHIFT;

            u8 abilitySlot = trmon[i].abilitySlot;
            u8 ability = Pokemon_LoadAbilityValue(species, form, abilitySlot);

            u8 nature = trmon[i].nature;
            u32 pid = AdjustGender(mon, trmon[i].gender, nature, trainerGender);

            Pokemon_InitWith(mon, species, trmon[i].level, 31, TRUE, pid, OTID_NOT_SHINY, 0);
            Pokemon_SetValue(mon, MON_DATA_HELD_ITEM, &trmon[i].item);
            Pokemon_SetValue(mon, MON_DATA_FORM, &form);
            Pokemon_SetValue(mon, MON_DATA_ABILITY, &ability);

            for (j = 0; j < 4; j++) {
                Pokemon_SetMoveSlot(mon, trmon[i].moves[j], j);
            }

            AdjustHP(mon, trmon[i].preDamage);
            AdjustIVs(mon, trmon[i].ivs);
            AdjustStatus(mon, trmon[i].status);

            Pokemon_CalcLevelAndStats(mon);
            Pokemon_SetBallSeal(trmon[i].cbSeal, mon, heapID);
            Party_AddPokemon(dto->parties[battler], mon);
        }

        break;
    }
    }

    Heap_FreeToHeap(buf);
    Heap_FreeToHeap(mon);
    LCRNG_SetSeed(oldSeed);
}

static void AdjustHP(Pokemon *mon, u8 preDamage)
{
    u32 currentHP = Pokemon_GetValue(mon, MON_DATA_MAX_HP, NULL);

    switch (preDamage) {
    case 50:
        currentHP = currentHP / 2;
        break;
    case 25:
        currentHP = currentHP / 4;
        break;
    case 1:
        currentHP = 1;
        break;
    case 0:
        break;
    default:
        break;
    }

    Pokemon_SetValue(mon, MON_DATA_CURRENT_HP, &currentHP);

    return;
}

static void AdjustIVs(Pokemon *mon, u8 IVProfile)
{
    int ivs;

    if (getVar(VAR_DIFFICULTY) & EASY_MODE_ENABLED) {
        ivs = Pokemon_GetValue(mon, MON_DATA_LEVEL, NULL) * 31 / 100;

        for (int i = 0; i < 6; i++) {
            Pokemon_SetValue(mon, MON_DATA_HP_IV + i, &ivs);
        }
    } else if (IVProfile == 1) {
        ivs = 0;
        Pokemon_SetValue(mon, MON_DATA_SPEED_IV, &ivs);
    } else if (IVProfile != 0) {

        u8 IVSpreads[NUM_POKEMON_TYPES][6] = {
            { 31, 31, 31, 31, 31, 31 }, // Normal
            { 31, 31, 30, 30, 30, 30 }, // Fighting
            { 30, 30, 30, 30, 30, 31 }, // Flying
            { 31, 31, 30, 30, 30, 31 }, // Poison
            { 31, 31, 31, 30, 30, 31 }, // Ground
            { 31, 31, 30, 31, 30, 30 }, // Rock
            { 31, 30, 30, 31, 30, 31 }, // Bug
            { 31, 31, 30, 31, 30, 31 }, // Ghost
            { 31, 31, 31, 31, 30, 31 }, // Steel
            { 31, 31, 31, 31, 31, 31 }, // Mystery
            { 31, 30, 31, 30, 31, 30 }, // Fire
            { 31, 30, 30, 30, 31, 31 }, // Water
            { 31, 30, 31, 30, 31, 31 }, // Grass
            { 31, 31, 31, 30, 31, 31 }, // Electric
            { 31, 30, 31, 31, 31, 30 }, // Psychic
            { 31, 30, 30, 31, 31, 31 }, // Ice
            { 31, 30, 31, 31, 31, 31 }, // Dragon
            { 31, 31, 31, 31, 31, 31 } // Dark
        };

        IVProfile -= 2;

        Pokemon_SetValue(mon, MON_DATA_HP_IV, &IVSpreads[IVProfile][0]);
        Pokemon_SetValue(mon, MON_DATA_ATK_IV, &IVSpreads[IVProfile][1]);
        Pokemon_SetValue(mon, MON_DATA_DEF_IV, &IVSpreads[IVProfile][2]);
        Pokemon_SetValue(mon, MON_DATA_SPATK_IV, &IVSpreads[IVProfile][3]);
        Pokemon_SetValue(mon, MON_DATA_SPDEF_IV, &IVSpreads[IVProfile][4]);
        Pokemon_SetValue(mon, MON_DATA_SPEED_IV, &IVSpreads[IVProfile][5]);
    }

    return;
}

static void AdjustStatus(Pokemon *mon, u8 status)
{
    u32 statusList[] = { MON_CONDITION_NONE, MON_CONDITION_BURN, MON_CONDITION_POISON, MON_CONDITION_PARALYSIS, MON_CONDITION_FREEZE };

    Pokemon_SetValue(mon, MON_DATA_STATUS_CONDITION, &statusList[status]);

    return;
}

static u32 AdjustGender(Pokemon *mon, u8 nature, u8 gender, u32 trainerGender)
{
    u32 pid = nature;

    /*
    u8 species = Pokemon_GetValue(mon, MON_DATA_SPECIES, NULL);
    u8 monGenderChance = SpeciesData_GetSpeciesValue(species, SPECIES_DATA_GENDER_RATIO);

    if (gender == 3) {
        gender = trainerGender;
    }

    return pid + 25 * gender;

    */

    return pid;
}
