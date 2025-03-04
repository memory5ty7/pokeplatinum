#include "party.h"

#include <nitro.h>
#include <string.h>

#include "heap.h"
#include "pokemon.h"
#include "savedata.h"
#include "field_system.h"
#include "field/field_system.h"
#include "field/field_system_decl.h"
#include "field_task.h"
#include "save_player.h"

#include "struct_decls/struct_02098700_decl.h"
#include "struct_defs/struct_0202610C.h"
#include "struct_defs/struct_0204AFC4.h"

#define PARTY_ASSERT_SLOT(party, slot)           \
    {                                            \
        GF_ASSERT(slot >= 0);                    \
        GF_ASSERT(slot < (party)->currentCount); \
        GF_ASSERT(slot < (party)->capacity);     \
    }

int Party_SaveSize(void)
{
    return sizeof(Party);
}

Party *Party_New(u32 heapID)
{
    Party *party = Heap_AllocFromHeap(heapID, sizeof(Party));
    Party_Init(party);

    return party;
}

void Party_Init(Party *party)
{
    Party_InitWithCapacity(party, MAX_PARTY_SIZE);
}

void Party_InitWithCapacity(Party *party, int capacity)
{
    int i;

    GF_ASSERT(capacity <= MAX_PARTY_SIZE);
    memset(party, 0, sizeof(Party));

    party->currentCount = 0;
    party->capacity = capacity;

    for (i = 0; i < MAX_PARTY_SIZE; i++) {
        Pokemon_Init(&party->pokemon[i]);
    }
}

BOOL Party_AddPokemon(Party *party, Pokemon *pokemon)
{
    if (party->currentCount >= party->capacity) {
        return FALSE;
    }

    party->pokemon[party->currentCount] = *pokemon;
    party->currentCount++;

    return TRUE;
}

BOOL Party_RemovePokemonBySlotIndex(Party *party, int slot)
{
    int i;

    PARTY_ASSERT_SLOT(party, slot);
    GF_ASSERT(party->currentCount > 0);

    for (i = slot; i < party->currentCount - 1; i++) {
        party->pokemon[i] = party->pokemon[i + 1];
    }

    Pokemon_Init(&party->pokemon[i]);
    party->currentCount--;

    return TRUE;
}

int Party_GetCapacity(const Party *party)
{
    return party->capacity;
}

int Party_GetCurrentCount(const Party *party)
{
    return party->currentCount;
}

Pokemon *Party_GetPokemonBySlotIndex(const Party *party, int slot)
{
    PARTY_ASSERT_SLOT(party, slot);
    return (Pokemon *)&party->pokemon[slot];
}

void sub_0207A128(Party *party, int slot, Pokemon *pokemon)
{
    int v0;

    PARTY_ASSERT_SLOT(party, slot);

    v0 = Pokemon_GetValue(&(party->pokemon[slot]), MON_DATA_SPECIES_EXISTS, NULL) - Pokemon_GetValue(pokemon, MON_DATA_SPECIES_EXISTS, NULL);
    party->pokemon[slot] = *pokemon;
    party->currentCount += v0;
}

BOOL Party_SwapSlots(Party *party, int slotA, int slotB)
{
    Pokemon *tempPokemon;

    PARTY_ASSERT_SLOT(party, slotA);
    PARTY_ASSERT_SLOT(party, slotB);

    tempPokemon = Heap_AllocFromHeap(0, sizeof(Pokemon));
    *tempPokemon = party->pokemon[slotA];

    party->pokemon[slotA] = party->pokemon[slotB];
    party->pokemon[slotB] = *tempPokemon;

    Heap_FreeToHeap(tempPokemon);

    return FALSE;
}

void Party_Copy(const Party *src, Party *dest)
{
    *dest = *src;
}

BOOL Party_HasSpecies(const Party *party, int species)
{
    int i;

    for (i = 0; i < party->currentCount; i++) {
        if (Pokemon_GetValue((Pokemon *)&party->pokemon[i], MON_DATA_SPECIES, NULL) == species) {
            break;
        }
    }

    return i != party->currentCount;
}

Party *Party_GetFromSavedata(SaveData *saveData)
{
    return SaveData_SaveTable(saveData, SAVE_TABLE_ENTRY_PARTY);
}

Party *Party_GetLimited(FieldSystem *fieldSystem)
{
    int v0;
    u8 v1;
    u32 v2;
    //FieldBattleDTO *v3;
    SaveData *v4 = fieldSystem->saveData;
    Party *v5 = Party_GetFromSavedata(v4);
    Pokemon *v6;

    Party *v7 = Party_New(11);


    UnkStruct_0204AFC4 *param0 = fieldSystem->unk_AC;

    //v3 = FieldBattleDTO_New(param0->unk_04, ov104_0223A700(param0->unk_0F));
    //v4 = param1->unk_08;

    //FieldBattleDTO_InitFromGameState(v3, NULL, param1->unk_08, param1->unk_1C, param1->unk_0C, param1->unk_10, param1->unk_20);

    //v3->background = 18;
    //v3->terrain = TERRAIN_BATTLE_TOWER;

    v6 = Pokemon_New(param0->unk_04);

    //Party_InitWithCapacity(v3->parties[0], param0->unk_0E);

    for (v0 = 0; v0 < param0->unk_0E; v0++) {
        Pokemon_Copy(Party_GetPokemonBySlotIndex(v5, param0->unk_2A[v0]), v6);
        //FieldBattleDTO_AddPokemonToBattler(v3, v6, 0);
        Party_AddPokemon(v7, v6);
    }

    Heap_FreeToHeap(v6);

    return v7;
}
