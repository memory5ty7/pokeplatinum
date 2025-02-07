#include "unk_020997B8.h"

#include <nitro.h>
#include <string.h>

#include "struct_defs/struct_020997B8.h"

#include "heap.h"
#include "pokemon.h"

UnkStruct_020997B8 *sub_020997B8(u32 param0)
{
    UnkStruct_020997B8 *v0 = Heap_AllocFromHeap(param0, sizeof(UnkStruct_020997B8));

    memset(v0, 0, sizeof(UnkStruct_020997B8));
    return v0;
}

void sub_020997D0(UnkStruct_020997B8 *param0)
{
    Heap_FreeToHeap(param0);
}

u16 *sub_020997D8(Pokemon *param0, u32 param1)
{
    u32 *monLevelUpMoves;
    u16 *v1;
    u16 v2[4];
    u16 monSpecies = Pokemon_GetValue(param0, MON_DATA_SPECIES, NULL);
    u16 monLevel = Pokemon_GetValue(param0, MON_DATA_LEVEL, NULL);
    u8 monForm = Pokemon_GetValue(param0, MON_DATA_FORM, NULL);
    u8 v6, v8;

    for (int i = 0; i < LEARNED_MOVES_MAX; i++) {
        v2[i] = (u16)Pokemon_GetValue(param0, MON_DATA_MOVE1 + i, NULL);
    }

    monLevelUpMoves = Heap_AllocFromHeap(param1, (44 / 2) * 2);
    v1 = Heap_AllocFromHeap(param1, (44 / 2) * 2);

    Pokemon_LoadLevelUpMovesOf(monSpecies, monForm, monLevelUpMoves);

    v8 = 0;

    for (int i = 0; i < (44 / 2); i++) {
        if (monLevelUpMoves[i] == 0xffffffff) {
            v1[v8] = 0xffff;
            break;
        } else if (((monLevelUpMoves[i] & 0xffff0000) >> 16) > monLevel) {
            continue;
        } else {
            monLevelUpMoves[i] &= 0xffff;

            for (v6 = 0; v6 < 4; v6++) {
                if (monLevelUpMoves[i] == v2[v6]) {
                    break;
                }
            }

            if (v6 == 4) {
                for (v6 = 0; v6 < v8; v6++) {
                    if (v1[v6] == monLevelUpMoves[i]) {
                        break;
                    }
                }

                if (v6 == v8) {
                    v1[v8] = monLevelUpMoves[i];
                    v8++;
                }
            }
        }
    }

    Heap_FreeToHeap(monLevelUpMoves);

    return v1;
}

BOOL sub_020998D8(u16 *param0)
{
    if (param0[0] == 0xffff) {
        return 0;
    }

    return 1;
}
