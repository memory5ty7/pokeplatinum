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
#include "message_util.h"
#include "narc.h"
#include "party.h"
#include "pokemon.h"
#include "savedata.h"
#include "savedata_misc.h"
#include "string_gf.h"

#include "macros.h"
#include "desmume.h"
#include "pokemon.h"

static void TrainerData_BuildParty(FieldBattleDTO *dto, int battler, enum HeapID heapID);

void Trainer_Encounter(FieldBattleDTO *dto, const SaveData *saveData, enum HeapID heapID)
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
            String *trainerName = MessageLoader_GetNewString(msgLoader, dto->trainerIDs[i]);
            String_ToChars(trainerName, dto->trainer[i].name, TRAINER_NAME_LEN + 1);
            String_Free(trainerName);
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

BOOL Trainer_HasMessageType(int trainerID, enum TrainerMessageType msgType, enum HeapID heapID)
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

void Trainer_LoadMessage(int trainerID, enum TrainerMessageType msgType, String *string, enum HeapID heapID)
{
    NARC *narc; // must declare up here to match
    u16 offset, data[2];

    int size = NARC_GetMemberSizeByIndexPair(NARC_INDEX_POKETOOL__TRMSG__TRTBL, 0);
    NARC_ReadFromMemberByIndexPair(&offset, NARC_INDEX_POKETOOL__TRMSG__TRTBLOFS, 0, trainerID * 2, 2);
    narc = NARC_ctor(NARC_INDEX_POKETOOL__TRMSG__TRTBL, heapID);

    while (offset != size) {
        NARC_ReadFromMember(narc, 0, offset, 4, data);

        if (data[0] == trainerID && data[1] == msgType) {
            MessageBank_GetStringFromNARC(NARC_INDEX_MSGDATA__PL_MSG, TEXT_BANK_NPC_TRAINER_MESSAGES, offset / 4, heapID, string);
            break;
        }

        offset += 4;
    }

    NARC_dtor(narc);

    if (offset == size) {
        String_Clear(string);
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

static void SetNewSpecies(Pokemon *mon, u16 species)
{
    String *speciesName = MessageUtil_SpeciesName(species, HEAP_ID_SYSTEM);
    Pokemon_SetValue(mon, MON_DATA_SPECIES, &species);
    Pokemon_SetValue(mon, MON_DATA_NICKNAME_STRING, speciesName);
    Pokemon_CalcStats(mon);

    String_Free(speciesName);
};

static u8 specialEvoLevels[] = {
    // Branch Evolution
    [SPECIES_TYROGUE] = 20,
    [SPECIES_KIRLIA] = 36,
    [SPECIES_SNORUNT] = 36,
    [SPECIES_BURMY] = 20,
    [SPECIES_COMBEE] = 21,
    [SPECIES_WURMPLE] = 7,
    [SPECIES_NINCADA] = 20,

    // Happiness Evolution
    [SPECIES_PICHU] = 14,
    [SPECIES_IGGLYBUFF] = 14,
    [SPECIES_CLEFFA] = 14,
    [SPECIES_GOLBAT] = 40,
    [SPECIES_CHANSEY] = 44,
    [SPECIES_MUNCHLAX] = 30,
    [SPECIES_TOGEPI] = 16,
    [SPECIES_AZURILL] = 12,
    [SPECIES_BUNEARY] = 18,
    [SPECIES_BUDEW] = 16,
    [SPECIES_RIOLU] = 26,
    [SPECIES_CHINGLING] = 24,

    // Trade Evolution
    [SPECIES_MACHOKE] = 40,
    [SPECIES_KADABRA] = 40,
    [SPECIES_GRAVELER] = 40,
    [SPECIES_HAUNTER] = 40,
    [SPECIES_SLOWPOKE] = 37,
    [SPECIES_POLIWHIRL] = 34,
    [SPECIES_ONIX] = 36,
    [SPECIES_SCYTHER] = 38,
    [SPECIES_CLAMPERL] = 26,
    [SPECIES_SEADRA] = 44,
    [SPECIES_RHYDON] = 48,
    [SPECIES_ELECTABUZZ] = 48,
    [SPECIES_MAGMAR] = 48,
    [SPECIES_PORYGON] = 35,
    [SPECIES_PORYGON2] = 45,
    [SPECIES_DUSCLOPS] = 48,

    // Stone Evolution
    [SPECIES_SHELLDER] = 34,
    [SPECIES_STARYU] = 36,
    [SPECIES_LOMBRE] = 36,
    [SPECIES_VULPIX] = 40,
    [SPECIES_GROWLITHE] = 40,
    [SPECIES_PIKACHU] = 38,
    [SPECIES_MAGNETON] = 46,
    [SPECIES_NOSEPASS] = 40,
    [SPECIES_NIDORINA] = 35,
    [SPECIES_NIDORINO] = 35,
    [SPECIES_CLEFAIRY] = 35,
    [SPECIES_JIGGLYPUFF] = 35,
    [SPECIES_SKITTY] = 20,
    [SPECIES_MURKROW] = 40,
    [SPECIES_MISDREAVUS] = 40,
    [SPECIES_TOGETIC] = 40,
    [SPECIES_ROSELIA] = 38,
    [SPECIES_GLOOM] = 36,
    [SPECIES_WEEPINBELL] = 36,
    [SPECIES_EXEGGCUTE] = 30,
    [SPECIES_NUZLEAF] = 36,
    [SPECIES_EEVEE] = 28,
    [SPECIES_GLIGAR] = 43,
    [SPECIES_SNEASEL] = 43,
    [SPECIES_HAPPINY] = 24,

    // Evolution by Move
    [SPECIES_MIME_JR] = 25,
    [SPECIES_BONSLY] = 25,
    [SPECIES_YANMA] = 35,
    [SPECIES_PILOSWINE] = 40,
    [SPECIES_TANGROWTH] = 40,
    [SPECIES_AIPOM] = 34,
    [SPECIES_LICKILICKY] = 37,

    // Miscellaneous Evolution
    [SPECIES_MANTYKE] = 35,
    [SPECIES_FEEBAS] = 30,

    // Balancing
    [SPECIES_MAGIKARP] = 30,
};

static void AdjustPartySpecies(Party *party)
{
    u32 rnd;
    u16 initialSpecies, species, evoSpecies;
    u8 monLevel, regularEvoLevel, specialEvoLevel, nbAvailableEvos;
    int i, j, k, totalEvolutions;
    u16 availableEvos[MAX_EVOLUTIONS];
    SpeciesEvolution *speciesEvolutions = Heap_Alloc(HEAP_ID_SYSTEM, sizeof(SpeciesEvolution) * MAX_EVOLUTIONS);
    
    for (i = 0; i < Party_GetCurrentCount(party); i++)
    {
        Pokemon *mon = Party_GetPokemonBySlotIndex(party, i);
        monLevel = Pokemon_GetValue(mon, MON_DATA_LEVEL, NULL);
        initialSpecies = Pokemon_GetValue(mon, MON_DATA_SPECIES, NULL);
        species = initialSpecies;

        for (j = 0 ; j < 2; j++)
        {
            evoSpecies = SPECIES_NONE;
            LoadSpeciesEvolutions(species, speciesEvolutions);

            // Number of total Evolutions
            totalEvolutions = 0;
            for (k = 0; k < MAX_EVOLUTIONS; k++)
            {
                if (speciesEvolutions[k].targetSpecies != SPECIES_NONE)
                {
                    totalEvolutions++;
                } 

                availableEvos[k] = SPECIES_NONE;
            }

            // Available Evolutions
            nbAvailableEvos = 0;
            for (k = 0; k < totalEvolutions; k++)
            {
                regularEvoLevel = speciesEvolutions[k].method == EVO_CLASS_BY_LEVEL ? speciesEvolutions[k].param : 0;
                specialEvoLevel = specialEvoLevels[species];

                if (monLevel >= MAX(regularEvoLevel, specialEvoLevel))
                    {
                        availableEvos[nbAvailableEvos] = speciesEvolutions[k].targetSpecies;
                        nbAvailableEvos += 1;
                    }
            }

            // Select an Available Evolution at random
            if (nbAvailableEvos > 0)
            {
                LCRNG_SetSeed(rnd);
                rnd = LCRNG_Next();
                evoSpecies = availableEvos[rnd % nbAvailableEvos];
            }

            // Change the mon
            if (evoSpecies != SPECIES_NONE)
            {
                species = evoSpecies;
                SetNewSpecies(mon, species);
            }
        }
      
        if (species != initialSpecies)
        {
            Desmume_Log("Initial Species: %d, Adjusted Species: %d\n", initialSpecies, species);
        }
    }

    Heap_Free(speciesEvolutions);
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
            level = MAX(initialLevel, playerMaxLevel);

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
static void TrainerData_BuildParty(FieldBattleDTO *dto, int battler, enum HeapID heapID)
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

            rnd = trmon[i].ivScale + trmon[i].level + species + dto->trainerIDs[battler];
            LCRNG_SetSeed(rnd);

            for (j = 0; j < dto->trainer[battler].header.trainerType; j++) {
                rnd = LCRNG_Next();
            }

            rnd = (rnd << 8) + genderMod;
            ivs = trmon[i].ivScale * MAX_IVS_SINGLE_STAT / MAX_IV_SCALE;

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

            rnd = trmon[i].ivScale + trmon[i].level + species + dto->trainerIDs[battler];
            LCRNG_SetSeed(rnd);

            for (j = 0; j < dto->trainer[battler].header.trainerType; j++) {
                rnd = LCRNG_Next();
            }

            rnd = (rnd << 8) + genderMod;
            ivs = trmon[i].ivScale * MAX_IVS_SINGLE_STAT / MAX_IV_SCALE;

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

            rnd = trmon[i].ivScale + trmon[i].level + species + dto->trainerIDs[battler];
            LCRNG_SetSeed(rnd);

            for (j = 0; j < dto->trainer[battler].header.trainerType; j++) {
                rnd = LCRNG_Next();
            }

            rnd = (rnd << 8) + genderMod;
            ivs = trmon[i].ivScale * MAX_IVS_SINGLE_STAT / MAX_IV_SCALE;

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
            u16 ability = trmon[i].ivScale;

            rnd = trmon[i].ivScale + trmon[i].level + species + dto->trainerIDs[battler];
            LCRNG_SetSeed(rnd);

            for (j = 0; j < dto->trainer[battler].header.trainerType; j++) {
                rnd = LCRNG_Next();
            }

            rnd = (rnd << 8) + genderMod;
            ivs = trmon[i].level * MAX_IVS_SINGLE_STAT / MAX_POKEMON_LEVEL;

            if (ability == ABILITY_NONE)
            {
                rnd = LCRNG_Next();
                ability = SpeciesData_GetSpeciesValue(species, SPECIES_DATA_ABILITY_1 + (rnd % 2));
            }

            Pokemon_InitWith(mon, species, trmon[i].level, ivs, TRUE, rnd, OTID_NOT_SHINY, 0);
            Pokemon_SetValue(mon, MON_DATA_HELD_ITEM, &trmon[i].item);

            for (j = 0; j < 4; j++) {
                Pokemon_SetMoveSlot(mon, trmon[i].moves[j], j);
            }

            Pokemon_SetBallSeal(trmon[i].cbSeal, mon, heapID);
            Pokemon_SetValue(mon, MON_DATA_FORM, &form);
            Pokemon_SetValue(mon, MON_DATA_ABILITY, &ability);
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

    //(!isSpecialClass)
    //{
        AdjustPartySpecies(dto->parties[battler]);
    //}

    Heap_Free(buf);
    Heap_Free(mon);
    LCRNG_SetSeed(oldSeed);
}
