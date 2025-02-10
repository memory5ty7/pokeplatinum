#include "trainer_data.h"

#include "constants/battle.h"
#include "constants/pokemon.h"
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

static void TrainerData_BuildParty(FieldBattleDTO *dto, int battler, int heapID);

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
            Strbuf_ToChars(trainerName, dto->trainer[i].name, TRAINER_NAME_LEN + 1);
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

    Trainer_LoadParty(dto->trainerIDs[battler], buf);

    switch (dto->trainer[battler].header.monDataType) {
    case TRDATATYPE_BASE: {
        TrainerMonBase *trmon = (TrainerMonBase *)buf;
        for (i = 0; i < dto->trainer[battler].header.partySize; i++) {
            u16 species = trmon[i].species & 0x3FF;
            u8 form = (trmon[i].species & 0xFC00) >> TRAINER_MON_FORM_SHIFT;

            ivs = GetIVsFromDV(trmon[i].dv);

            u8 abilitySlot = (trmon[i].species & 0x100) >> 8;
            u8 ability = Pokemon_LoadAbilityValue(species, form, abilitySlot);

            Pokemon_InitWith(mon, species, trmon[i].level, ivs, TRUE, trmon[i].dv, OTID_NOT_SHINY, 0);
            Pokemon_SetValue(mon, MON_DATA_FORM, &form);
            Pokemon_SetValue(mon, MON_DATA_ABILITY, &ability);

            u32 status = getStatusFromDV(trmon[i].dv);
            Pokemon_SetValue(mon, MON_DATA_STATUS_CONDITION, &status);
    
            AdjustHP(mon, trmon[i].dv);

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

            ivs = GetIVsFromDV(trmon[i].dv);

            u8 abilitySlot = (trmon[i].species & 0x100) >> 8;
            u8 ability = Pokemon_LoadAbilityValue(species, form, abilitySlot);

            Pokemon_InitWith(mon, species, trmon[i].level, ivs, TRUE, trmon[i].dv, OTID_NOT_SHINY, 0);
            Pokemon_SetValue(mon, MON_DATA_FORM, &form);
            Pokemon_SetValue(mon, MON_DATA_ABILITY, &ability);

            for (j = 0; j < 4; j++) {
                Pokemon_SetMoveSlot(mon, trmon[i].moves[j], j);
            }

            u32 status = getStatusFromDV(trmon[i].dv);
            Pokemon_SetValue(mon, MON_DATA_STATUS_CONDITION, &status);
    
            AdjustHP(mon, trmon[i].dv);

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

            ivs = GetIVsFromDV(trmon[i].dv);

            u8 abilitySlot = (trmon[i].species & 0x100) >> 8;
            u8 ability = Pokemon_LoadAbilityValue(species, form, abilitySlot);

            Pokemon_InitWith(mon, species, trmon[i].level, ivs, TRUE, trmon[i].dv, OTID_NOT_SHINY, 0);
            Pokemon_SetValue(mon, MON_DATA_HELD_ITEM, &trmon[i].item);
            Pokemon_SetValue(mon, MON_DATA_FORM, &form);
            Pokemon_SetValue(mon, MON_DATA_ABILITY, &ability);

            u32 status = getStatusFromDV(trmon[i].dv);
            Pokemon_SetValue(mon, MON_DATA_STATUS_CONDITION, &status);
    
            AdjustHP(mon, trmon[i].dv);

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

            ivs = GetIVsFromDV(trmon[i].dv);

            u8 abilitySlot = (trmon[i].species & 0x100) >> 8;
            u8 ability = Pokemon_LoadAbilityValue(species, form, abilitySlot);

            Pokemon_InitWith(mon, species, trmon[i].level, ivs, TRUE, trmon[i].dv, OTID_NOT_SHINY, 0);
            Pokemon_SetValue(mon, MON_DATA_HELD_ITEM, &trmon[i].item);
            Pokemon_SetValue(mon, MON_DATA_FORM, &form);
            Pokemon_SetValue(mon, MON_DATA_ABILITY, &ability);

            for (j = 0; j < 4; j++) {
                Pokemon_SetMoveSlot(mon, trmon[i].moves[j], j);
            }

            u32 status = getStatusFromDV(trmon[i].dv);
            Pokemon_SetValue(mon, MON_DATA_STATUS_CONDITION, &status);
    
            AdjustHP(mon, trmon[i].dv);

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

u32 getStatusFromDV(u16 dv)
{
    u32 status = MON_CONDITION_NONE;

    switch (dv) {
    case 178:
    case 188:
    case 328:
    case 338:
        status = MON_CONDITION_POISON;
        break;
    case 203:
    case 213:
    case 303:
    case 313:
        status = MON_CONDITION_BURN;
        break;
    }
    return status;
}

u8 GetIVsFromDV(u16 dv)
{
    u8 ivs = 31;

    switch (dv) {
    case 0:
        ivs = 0;
        break;
    }
    return ivs;
}

void AdjustIVs(Pokemon *mon, u16 dv)
{
    // If Nature is -Speed, set Speed IVs to 0
    /*
    if(Pokemon_GetStatAffinityOf(Pokemon_GetNatureOf(dv),3) == -1)
    {
        Pokemon_SetValue(mon, MON_DATA_SPEED_IV, 0);
    }
    */

    return;
}

void AdjustHP(Pokemon *mon, u16 dv)
{
    u32 currentHP = Pokemon_GetValue(mon, MON_DATA_MAX_HP, NULL);
    switch (dv) {
    case 153:
    case 353:
        currentHP = currentHP / 4;
        break;
    }

    Pokemon_SetValue(mon, MON_DATA_CURRENT_HP, &currentHP);

    return;
}
