#include "struct_defs/trainer_data.h"

#include <nitro.h>
#include <string.h>

#include "constants/battle.h"
#include "constants/pokemon.h"
#include "constants/trainer.h"

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
#include "trainer_data.h"

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

        if (trdata.class == TRAINER_CLASS_RIVAL) {
            CharCode_Copy(dto->trainer[i].name, rivalName);
        } else {
            Strbuf *trainerName = MessageLoader_GetNewStrbuf(msgLoader, dto->trainerIDs[i]);
            Strbuf_ToChars(trainerName, dto->trainer[i].name, TRAINER_NAME_LEN + 1);
            Strbuf_Free(trainerName);
        }

        TrainerData_BuildParty(dto, i, heapID);
    }

    dto->battleType |= trdata.battleType;
    MessageLoader_Free(msgLoader);
}

u32 Trainer_LoadParam(int trainerID, enum TrainerDataParam paramID)
{
    u32 result;
    Trainer trdata;

    Trainer_Load(trainerID, &trdata);

    switch (paramID) {
    case TRDATA_TYPE:
        result = trdata.type;
        break;

    case TRDATA_CLASS:
        result = trdata.class;
        break;

    case TRDATA_SPRITE:
        result = trdata.sprite;
        break;

    case TRDATA_PARTY_SIZE:
        result = trdata.partySize;
        break;

    case TRDATA_ITEM_1:
    case TRDATA_ITEM_2:
    case TRDATA_ITEM_3:
    case TRDATA_ITEM_4:
        result = trdata.items[paramID - TRDATA_ITEM_1];
        break;

    case TRDATA_AI_MASK:
        result = trdata.aiMask;
        break;

    case TRDATA_BATTLE_TYPE:
        result = trdata.battleType;
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
    u8 *buf;
    int i, j;
    u32 genderMod, rnd, oldSeed;
    Pokemon *mon;

    oldSeed = LCRNG_GetSeed();

    // alloc enough space to support the maximum possible data size
    Party_InitWithCapacity(dto->parties[battler], MAX_PARTY_SIZE);
    buf = Heap_AllocFromHeap(heapID, sizeof(TrainerMonWithMovesAndItem) * MAX_PARTY_SIZE);

    Trainer_LoadParty(dto->trainerIDs[battler], buf);

    u8 partySize = dto->trainer[battler].partySize;

    u16 offset = 0;
    Pokemon *party[partySize];

    for (i = 0; i < partySize; i++) {
        party[i] = Pokemon_New(heapID);

        u16 dv = buf[offset]
            | (buf[offset + 1] << 8);
        buf += 2;

        u8 ivs = GetIVsFromDV(dv);

        u8 abilitySlot = (dv & 0x100) >> 8;

        u16 level = buf[offset]
            | (buf[offset + 1] << 8);
        buf += 2;

        u16 species = buf[offset]
            | (buf[offset + 1] << 8);
        buf += 2;

        u8 form = (species & 0xFC00) >> 10;
        species = (species & 0x03FF);

        u16 item;
        if (dto->trainer[battler].type == TRDATATYPE_WITH_ITEM || dto->trainer[battler].type == TRDATATYPE_WITH_MOVES_AND_ITEM) {
            item = buf[offset]
                | (buf[offset + 1] << 8);
            buf += 2;
        }

        u16 moves[4];
        if (dto->trainer[battler].type == TRDATATYPE_WITH_MOVES || dto->trainer[battler].type == TRDATATYPE_WITH_MOVES_AND_ITEM) {
            for (j = 0; j < 4; j++) {
                moves[j] = buf[offset]
                    | (buf[offset + 1] << 8);
                buf += 2;
            }
        }

        u16 ballSeal = buf[offset]
            | (buf[offset + 1] << 8);
        buf += 2;

        u8 ability = Pokemon_LoadAbilityValue(species, form, abilitySlot);

        Pokemon_InitWith(party[i], species, level, 31, TRUE, dv, OTID_NOT_SHINY, 0);
        AdjustIVs(party[i], dv);
        Pokemon_SetValue(party[i], MON_DATA_FORM, &form);
        Pokemon_SetValue(party[i], MON_DATA_ABILITY, &ability);

        if (dto->trainer[battler].type == TRDATATYPE_WITH_ITEM || dto->trainer[battler].type == TRDATATYPE_WITH_MOVES_AND_ITEM) {
            Pokemon_SetValue(party[i], MON_DATA_HELD_ITEM, &item);
        }

        if (dto->trainer[battler].type == TRDATATYPE_WITH_MOVES || dto->trainer[battler].type == TRDATATYPE_WITH_MOVES_AND_ITEM) {
            for (j = 0; j < 4; j++) {
                Pokemon_SetMoveSlot(party[i], moves[j], j);
            }
        }

        u32 status = getStatusFromDV(dv);
        Pokemon_SetValue(party[i], MON_DATA_STATUS_CONDITION, &status);

        AdjustHP(party[i], dv);

        Pokemon_CalcLevelAndStats(party[i]);
        Pokemon_SetBallSeal(ballSeal, party[i], heapID);
    }

    for (int i = 0; i < partySize; i++) {
        Party_AddPokemon(dto->parties[battler], party[i]);
        Heap_FreeToHeap(party[i]);
    }

    Heap_FreeToHeap(buf);
    LCRNG_SetSeed(oldSeed);
}

u32 getStatusFromDV(u16 dv)
{
    u32 status = MON_CONDITION_NONE;

    switch (dv) {
    case 128:
    case 138:
    case 153:
    case 163:
        status = MON_CONDITION_POISON;
        break;
    case 178:
    case 188:
    case 203:
    case 213:
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
    switch(dv)
    {
        case 255:
            currentHP = currentHP / 4;
        break;
    }

    Pokemon_SetValue(mon, MON_DATA_CURRENT_HP, &currentHP);

    return;
}