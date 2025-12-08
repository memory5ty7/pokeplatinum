#include "trainer_data.h"

#include "constants/battle.h"
#include "constants/pokemon.h"
#include "generated/trainer_message_types.h"

#include "struct_defs/trainer.h"

#include "data/trainer_class_genders.h"

#include "charcode_util.h"
#include "field_battle_data_transfer.h"
#include "heap.h"
#include "math_util.h"
#include "message.h"
#include "narc.h"
#include "party.h"
#include "pokemon.h"
#include "savedata.h"
#include "savedata_misc.h"
#include "strbuf.h"

#include "macros.h"
#include "desmume.h"
#include "pokemon.h"

static void TrainerData_BuildParty(FieldBattleDTO *dto, int battler, int heapID);

void Trainer_Encounter(FieldBattleDTO *dto, const SaveData *saveData, int heapID)
{
    Trainer trdata;
    MessageLoader *msgLoader = MessageLoader_Init(MSG_LOADER_LOAD_ON_DEMAND, NARC_INDEX_MSGDATA__PL_MSG, TEXT_BANK_NPC_TRAINER_NAMES, heapID);
    const charcode_t *rivalName = MiscSaveBlock_RivalName(SaveData_MiscSaveBlockConst(saveData));

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
            MessageBank_GetStrbufFromNARC(NARC_INDEX_MSGDATA__PL_MSG, TEXT_BANK_NPC_TRAINER_MESSAGES, offset / 4, heapID, strbuf);
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

// Trainer classes not affected by level offset adjustments
u16 specialTrainerClasses[] = {
    TRAINER_CLASS_RIVAL,
    TRAINER_CLASS_LEADER_ROARK,
    TRAINER_CLASS_LEADER_GARDENIA,
    TRAINER_CLASS_LEADER_FANTINA,
    TRAINER_CLASS_LEADER_MAYLENE,
    TRAINER_CLASS_LEADER_WAKE,
    TRAINER_CLASS_LEADER_BYRON,
    TRAINER_CLASS_LEADER_CANDICE,
    TRAINER_CLASS_LEADER_VOLKNER,
    TRAINER_CLASS_ELITE_FOUR_AARON,
    TRAINER_CLASS_ELITE_FOUR_BERTHA,
    TRAINER_CLASS_ELITE_FOUR_FLINT,
    TRAINER_CLASS_ELITE_FOUR_LUCIAN,
    TRAINER_CLASS_CHAMPION_CYNTHIA,
    TRAINER_CLASS_COMMANDER_MARS,
    TRAINER_CLASS_COMMANDER_JUPITER,
    TRAINER_CLASS_COMMANDER_SATURN,
    TRAINER_CLASS_GALACTIC_BOSS,

};

// Level offsets for trainer classes (at level 50+)
static s8 levelOffsets[] = {
    [TRAINER_CLASS_YOUNGSTER]           = -10,
    [TRAINER_CLASS_LASS]                = -10,
    [TRAINER_CLASS_CAMPER]              = -8,
    [TRAINER_CLASS_PICNICKER]           = -8,
    [TRAINER_CLASS_BUG_CATCHER]         = -10,
    [TRAINER_CLASS_AROMA_LADY]          = -7,
    [TRAINER_CLASS_TWINS]               = -8,
    [TRAINER_CLASS_HIKER]               = -5,
    [TRAINER_CLASS_BATTLE_GIRL]         = -5,
    [TRAINER_CLASS_BLACK_BELT]          = -5,
    [TRAINER_CLASS_ACE_TRAINER_MALE]   = -1,
    [TRAINER_CLASS_ACE_TRAINER_FEMALE] = -1,
    [TRAINER_CLASS_ACE_TRAINER_SNOW_MALE]   = -1,
    [TRAINER_CLASS_ACE_TRAINER_SNOW_FEMALE] = -1,
    [TRAINER_CLASS_DRAGON_TAMER] = -1,
    [TRAINER_CLASS_VETERAN]     = -1,

};

static void AdjustPartySpecies(Party *party)
{
    u16 initialSpecies, species, evoSpecies;
    u8 monLevel;
    Strbuf *speciesName;
    
    for (int i = 0; i < Party_GetCurrentCount(party); i++)
    {
        Pokemon *mon = Party_GetPokemonBySlotIndex(party, i);
        monLevel = Pokemon_GetValue(mon, MON_DATA_LEVEL, NULL);
        initialSpecies = Pokemon_GetValue(mon, MON_DATA_SPECIES, NULL);
        species = initialSpecies;
        Desmume_Log("Initial Species : %d\n", initialSpecies);
        
        // Level Evolution
        evoSpecies = Pokemon_GetEvolutionTargetSpecies(party, mon, EVO_CLASS_BY_LEVEL, monLevel, NULL);
        Desmume_Log("Evolution Species : %d\n", evoSpecies);

        if (evoSpecies != SPECIES_NONE)
        {
            species = evoSpecies;
            speciesName = MessageUtil_SpeciesName(species, HEAP_ID_SYSTEM);
            Pokemon_SetValue(mon, MON_DATA_SPECIES, &species);
            Pokemon_SetValue(mon, MON_DATA_NICKNAME_STRING, speciesName);
            Pokemon_CalcStats(mon);

            // Second stage Evolution
            evoSpecies = Pokemon_GetEvolutionTargetSpecies(party, mon, EVO_CLASS_BY_LEVEL, monLevel, NULL);
            Desmume_Log("Evolution Species : %d\n", evoSpecies);

            if (evoSpecies != SPECIES_NONE)
            {
                species = evoSpecies;
                speciesName = MessageUtil_SpeciesName(species, HEAP_ID_SYSTEM);
                Pokemon_SetValue(mon, MON_DATA_SPECIES, &species);
                Pokemon_SetValue(mon, MON_DATA_NICKNAME_STRING, speciesName);
                Pokemon_CalcStats(mon);
            }
        }

        // Stone Evolution

        // Trade Evolution

        // Happiness Evolution

        // Miscellaneous Evolution

        if (species != initialSpecies)
        {
            Desmume_Log("Initial Species: %d, Adjusted Species: %d\n", initialSpecies, species);
        }
    }

    Strbuf_Free(speciesName);
}

static void AdjustPartyLevels(Party *party, u8 playerMinLevel, u8 playerMaxLevel, u8 playerMeanLevel, u8 playerMedianLevel, u8 enemyMinLevel, u8 enemyMaxLevel, u8 enemyMeanLevel, u8 enemyMedianLevel, u16 trainerClass, _Bool isSpecialClass)
{
    s8 offset = levelOffsets[trainerClass];

    for (int i = 0; i < Party_GetCurrentCount(party); i++)
    {
        Pokemon *mon = Party_GetPokemonBySlotIndex(party, i);
        u8 initialLevel = Pokemon_GetLevel(mon);
        u16 level = initialLevel;

        if (isSpecialClass)
        {
            level = playerMaxLevel;

            if (initialLevel != enemyMaxLevel)
            {
                level -= (enemyMaxLevel - initialLevel);
            }
        } else {
            level = ((playerMeanLevel + MAX(playerMeanLevel, playerMedianLevel)) / 2) + (initialLevel - enemyMeanLevel);
        }

        Desmume_Log("Initial Level: %d, Adjusted Level: %d\n", initialLevel, level);

        Pokemon_SetValue(mon, MON_DATA_LEVEL, &level);
        Pokemon_CalcStats(mon);
    }
};


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
    buf = Heap_Alloc(heapID, sizeof(TrainerMonWithMovesAndItem) * MAX_PARTY_SIZE);
    mon = Pokemon_New(heapID);

    Trainer_LoadParty(dto->trainerIDs[battler], buf);

    // determine which magic gender-specific modifier to use for the RNG function
    genderMod = TrainerClass_Gender(dto->trainer[battler].header.trainerType) == GENDER_FEMALE
        ? 120
        : 136;

    Party *playerParty = SaveData_GetParty(dto->saveData);
    u8 partyMaxLevel = Party_GetMaxLevel(playerParty);
    u8 partyMinLevel = Party_GetMinLevel(playerParty);
    u8 partyMeanLevel = Party_GetMeanLevel(playerParty);
    u8 partyMedianLevel = Party_GetMedianLevel(playerParty);

    u16 trainerClass = dto->trainer[battler].header.trainerType;
    _Bool isSpecialClass = FALSE;

    for (i = 0; i < NELEMS(specialTrainerClasses); i++) {
        if (trainerClass == specialTrainerClasses[i]) {
            isSpecialClass = TRUE;
            break;
        }   
    }

    switch (dto->trainer[battler].header.monDataType) {
    case TRDATATYPE_BASE: {
        TrainerMonBase *trmon = (TrainerMonBase *)buf;
        for (i = 0; i < dto->trainer[battler].header.partySize; i++) {
            u16 species = trmon[i].species & 0x3FF;
            u8 form = (trmon[i].species & 0xFC00) >> TRAINER_MON_FORM_SHIFT;

            rnd = trmon[i].dv + trmon[i].level + species + dto->trainerIDs[battler];
            LCRNG_SetSeed(rnd);

            for (j = 0; j < dto->trainer[battler].header.trainerType; j++) {
                rnd = LCRNG_Next();
            }

            rnd = (rnd << 8) + genderMod;
            ivs = trmon[i].dv * MAX_IVS_SINGLE_STAT / MAX_DV;

            Pokemon_InitWith(mon, species, trmon[i].level, ivs, TRUE, rnd, OTID_NOT_SHINY, 0);
            Pokemon_SetBallSeal(trmon[i].cbSeal, mon, heapID);
            Pokemon_SetValue(mon, MON_DATA_FORM, &form);
            Party_AddPokemon(dto->parties[battler], mon);
        }

        break;
    }

    case TRDATATYPE_WITH_MOVES: {
        TrainerMonWithMoves *trmon = (TrainerMonWithMoves *)buf;
        for (i = 0; i < dto->trainer[battler].header.partySize; i++) {
            u16 species = trmon[i].species & 0x3FF;
            u8 form = (trmon[i].species & 0xFC00) >> TRAINER_MON_FORM_SHIFT;

            rnd = trmon[i].dv + trmon[i].level + species + dto->trainerIDs[battler];
            LCRNG_SetSeed(rnd);

            for (j = 0; j < dto->trainer[battler].header.trainerType; j++) {
                rnd = LCRNG_Next();
            }

            rnd = (rnd << 8) + genderMod;
            ivs = trmon[i].dv * MAX_IVS_SINGLE_STAT / MAX_DV;

            Pokemon_InitWith(mon, species, trmon[i].level, ivs, TRUE, rnd, OTID_NOT_SHINY, 0);

            for (j = 0; j < 4; j++) {
                Pokemon_SetMoveSlot(mon, trmon[i].moves[j], j);
            }

            Pokemon_SetBallSeal(trmon[i].cbSeal, mon, heapID);
            Pokemon_SetValue(mon, MON_DATA_FORM, &form);
            Party_AddPokemon(dto->parties[battler], mon);
        }

        break;
    }

    case TRDATATYPE_WITH_ITEM: {
        TrainerMonWithItem *trmon = (TrainerMonWithItem *)buf;
        for (i = 0; i < dto->trainer[battler].header.partySize; i++) {
            u16 species = trmon[i].species & 0x3FF;
            u8 form = (trmon[i].species & 0xFC00) >> TRAINER_MON_FORM_SHIFT;

            rnd = trmon[i].dv + trmon[i].level + species + dto->trainerIDs[battler];
            LCRNG_SetSeed(rnd);

            for (j = 0; j < dto->trainer[battler].header.trainerType; j++) {
                rnd = LCRNG_Next();
            }

            rnd = (rnd << 8) + genderMod;
            ivs = trmon[i].dv * MAX_IVS_SINGLE_STAT / MAX_DV;

            Pokemon_InitWith(mon, species, trmon[i].level, ivs, TRUE, rnd, OTID_NOT_SHINY, 0);
            Pokemon_SetValue(mon, MON_DATA_HELD_ITEM, &trmon[i].item);
            Pokemon_SetBallSeal(trmon[i].cbSeal, mon, heapID);
            Pokemon_SetValue(mon, MON_DATA_FORM, &form);
            Party_AddPokemon(dto->parties[battler], mon);
        }

        break;
    }

    case TRDATATYPE_WITH_MOVES_AND_ITEM: {
        TrainerMonWithMovesAndItem *trmon = (TrainerMonWithMovesAndItem *)buf;
        for (i = 0; i < dto->trainer[battler].header.partySize; i++) {
            u16 species = trmon[i].species & 0x3FF;
            u8 form = (trmon[i].species & 0xFC00) >> TRAINER_MON_FORM_SHIFT;

            rnd = trmon[i].dv + trmon[i].level + species + dto->trainerIDs[battler];
            LCRNG_SetSeed(rnd);

            for (j = 0; j < dto->trainer[battler].header.trainerType; j++) {
                rnd = LCRNG_Next();
            }

            rnd = (rnd << 8) + genderMod;
            ivs = trmon[i].dv * MAX_IVS_SINGLE_STAT / MAX_DV;

            Pokemon_InitWith(mon, species, trmon[i].level, ivs, TRUE, rnd, OTID_NOT_SHINY, 0);
            Pokemon_SetValue(mon, MON_DATA_HELD_ITEM, &trmon[i].item);

            for (j = 0; j < 4; j++) {
                Pokemon_SetMoveSlot(mon, trmon[i].moves[j], j);
            }

            Pokemon_SetBallSeal(trmon[i].cbSeal, mon, heapID);
            Pokemon_SetValue(mon, MON_DATA_FORM, &form);
            Party_AddPokemon(dto->parties[battler], mon);
        }

        break;
    }
    }

    u8 enemyMaxLevel = Party_GetMaxLevel(dto->parties[battler]);
    u8 enemyMinLevel = Party_GetMinLevel(dto->parties[battler]);
    u8 enemyMeanLevel = Party_GetMeanLevel(dto->parties[battler]);
    u8 enemyMedianLevel = Party_GetMedianLevel(dto->parties[battler]);

    AdjustPartyLevels(
        dto->parties[battler],
        partyMinLevel,
        partyMaxLevel,
        partyMeanLevel,
        partyMedianLevel,
        enemyMinLevel,
        enemyMaxLevel,
        enemyMeanLevel,
        enemyMedianLevel,
        trainerClass,
        isSpecialClass
    );

    AdjustPartySpecies(dto->parties[battler]);

    Heap_Free(buf);
    Heap_Free(mon);
    LCRNG_SetSeed(oldSeed);
}
