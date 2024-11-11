#include "overlay006/ov6_02247660.h"

#include <nitro.h>
#include <string.h>

#include "field/field_system.h"

#include "heap.h"
#include "inlines.h"
#include "narc.h"
#include "record_mixed_rng.h"
#include "unk_02039C80.h"
#include "unk_0205F180.h"

BOOL ov6_02247660(FieldSystem *fieldSystem)
{
    u8 v0;
    int v1;
    int v2;
    u8 v3;
    u8 v4;
    void *v5;
    u16 *v6;
    int v7;
    int v8[4];
    int v9[4];
    int v10[4];
    int v11;
    BOOL v12;
    u32 v13;
    int v14;
    int v15, v16;

    if (inline_020564D0(2) == 0) {
        return 0;
    }

    v13 = RecordMixedRNG_GetRand(SaveData_GetRecordMixedRNG(fieldSystem->saveData));

    sub_020615AC(fieldSystem->playerAvatar, &v15, &v16);

    v14 = sub_02039E10(fieldSystem->unk_2C);
    v12 = 0;

    v8[0] = (v13 >> 24) & 0xff;
    v8[1] = (v13 >> 16) & 0xff;
    v8[2] = (v13 >> 8) & 0xff;
    v8[3] = v13 & 0xff;

    v5 = NARC_AllocAtEndAndReadWholeMemberByIndexPair(NARC_INDEX_ARC__ENCDATA_EX, 1, 4);
    v7 = ((int *)v5)[0];
    v1 = 0;

    for (v0 = 0; v0 < v7; v0++) {
        v1 += ((int *)v5)[1 + v0];
    }

    v6 = &((u16 *)v5)[2 + v7 * 2];
    v2 = v1 / 4;

    GF_ASSERT(v2 < 256);

    v3 = v1 % 4;
    v4 = 0;

    for (v0 = 0; v0 < 4; v0++) {
        v9[v0] = v2 * v0 + (v8[v0] % v2) + v4;
        v10[v0] = v6[v9[v0]];

        if (v3 != 0) {
            v4++;
            v3--;
        }
    }

    v11 = (v14 * 32 * v16) + v15;

    for (v0 = 0; v0 < 4; v0++) {
        if (v10[v0] == v11) {
            v12 = 1;
            break;
        }
    }

    Heap_FreeToHeap(v5);

    return v12;
}

void ov6_02247794(u8 *param0, u8 *param1)
{
    (*param0) = 20;
    (*param1) = 10;
}

void ov6_022477A0(int *param0)
{
    int *v0;

    v0 = NARC_AllocAtEndAndReadWholeMemberByIndexPair(NARC_INDEX_ARC__ENCDATA_EX, 0, 4);
    (*param0) = (*v0);

    Heap_FreeToHeap(v0);
}
