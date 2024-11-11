#include "overlay006/ov6_02246184.h"

#include <nitro.h>
#include <string.h>

#include "struct_decls/struct_party_decl.h"

#include "field/field_system.h"
#include "overlay006/struct_ov6_02246204_decl.h"
#include "overlay006/struct_ov6_02246254.h"

#include "graphics.h"
#include "heap.h"
#include "map_header.h"
#include "message.h"
#include "party.h"
#include "pokemon.h"
#include "save_player.h"
#include "strbuf.h"
#include "trainer_info.h"
#include "unk_0202F180.h"
#include "unk_020559DC.h"
#include "unk_02092494.h"

typedef struct {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0C;
    u32 unk_10;
    u32 unk_14;
    u32 unk_18;
    u32 unk_1C;
    u32 unk_20;
    u32 unk_24;
    u32 unk_28;
    u32 unk_2C;
    u32 unk_30;
    u32 unk_34;
    u32 unk_38;
    u32 unk_3C;
    u32 unk_40;
    u32 unk_44;
    u32 unk_48;
    u32 unk_4C;
} UnkStruct_ov6_0224630C;

typedef struct UnkStruct_ov6_02246204_t {
    UnkStruct_ov6_0224630C *unk_00;
    Pokemon *unk_04;
    TrainerInfo *unk_08;
    u32 unk_0C;
    u32 unk_10;
};

static Strbuf *ov6_022462E4(u32 param0, u32 param1);
static void ov6_0224630C(Pokemon *param0, UnkStruct_ov6_0224630C *param1, u32 param2, u32 param3, u32 param4, u32 param5);

UnkStruct_ov6_02246204 *ov6_02246184(u32 param0, u32 param1)
{
    UnkStruct_ov6_02246204 *v0;
    Strbuf *v1;
    u16 v2[128];

    GF_ASSERT(param1 < 4);

    v0 = Heap_AllocFromHeap(param0, sizeof(UnkStruct_ov6_02246204));
    memset(v0, 0, sizeof(UnkStruct_ov6_02246204));

    v0->unk_00 = LoadMemberFromNARC(115, param1, 0, param0, 0);
    v0->unk_10 = param0;
    v0->unk_0C = param1;
    v0->unk_04 = Pokemon_New(param0);
    v0->unk_08 = TrainerInfo_New(param0);

    TrainerInfo_Init(v0->unk_08);
    v1 = ov6_022462E4(param0, 4 + param1);

    Strbuf_ToChars(v1, v2, 128);
    Strbuf_Free(v1);
    TrainerInfo_SetName(v0->unk_08, v2);
    TrainerInfo_SetGender(v0->unk_08, v0->unk_00->unk_40);

    return v0;
}

void ov6_02246204(UnkStruct_ov6_02246204 *param0)
{
    Heap_FreeToHeap(param0->unk_00);
    Heap_FreeToHeap(param0->unk_04);
    Heap_FreeToHeap(param0->unk_08);
    Heap_FreeToHeap(param0);
}

u32 ov6_02246224(const UnkStruct_ov6_02246204 *param0)
{
    return param0->unk_00->unk_00;
}

u32 ov6_0224622C(const UnkStruct_ov6_02246204 *param0)
{
    return param0->unk_00->unk_4C;
}

void ov6_02246234(FieldSystem *fieldSystem, UnkStruct_ov6_02246204 *param1, int param2)
{
    Party *v0 = Party_GetFromSavedata(fieldSystem->saveData);

    sub_0207A128(v0, param2, param1->unk_04);
    sub_0202F180(fieldSystem->saveData, param1->unk_04);
}

void ov6_02246254(FieldSystem *fieldSystem, UnkStruct_ov6_02246204 *param1, int param2, UnkStruct_ov6_02246254 *param3, Pokemon *param4, Pokemon *param5)
{
    Party *v0 = Party_GetFromSavedata(fieldSystem->saveData);
    Pokemon *v1;
    Strbuf *v2;
    u32 v3;
    int v4;

    v1 = Party_GetPokemonBySlotIndex(v0, param2);
    v3 = Pokemon_GetValue(v1, MON_DATA_LEVEL, NULL);

    ov6_0224630C(param1->unk_04, param1->unk_00, v3, param1->unk_0C, param1->unk_10, fieldSystem->location->mapId);

    Pokemon_Copy(v1, param4);
    Pokemon_Copy(param1->unk_04, param5);

    param3->unk_00 = Pokemon_GetBoxPokemon(param4);
    param3->unk_04 = Pokemon_GetBoxPokemon(param5);
    param3->unk_08 = param1->unk_08;
    param3->unk_10 = 1;
    param3->unk_14 = SaveData_Options(fieldSystem->saveData);

    v4 = FieldSystem_GetTimeOfDay(fieldSystem);

    if ((v4 == 0) || (v4 == 1)) {
        param3->unk_0C = 0;
    } else if (v4 == 2) {
        param3->unk_0C = 1;
    } else {
        param3->unk_0C = 2;
    }
}

static Strbuf *ov6_022462E4(u32 fieldSystem, u32 param1)
{
    Strbuf *v0;
    MessageLoader *v1 = MessageLoader_Init(0, 26, 370, fieldSystem);

    v0 = MessageLoader_GetNewStrbuf(v1, param1);
    MessageLoader_Free(v1);
    return v0;
}

static void ov6_0224630C(Pokemon *param0, UnkStruct_ov6_0224630C *param1, u32 param2, u32 param3, u32 param4, u32 param5)
{
    Strbuf *v0;
    u8 v1;
    u32 v2;

    Pokemon_InitWith(param0, param1->unk_00, param2, 32, TRUE, param1->unk_38, OTID_SET, param1->unk_20);

    v0 = ov6_022462E4(param4, param3);

    Pokemon_SetValue(param0, MON_DATA_NICKNAME_STRBUF, v0);
    Strbuf_Free(v0);

    v1 = 1;

    Pokemon_SetValue(param0, MON_DATA_HAS_NICKNAME, &v1);
    Pokemon_SetValue(param0, MON_DATA_HP_IV, &param1->unk_04);
    Pokemon_SetValue(param0, MON_DATA_ATK_IV, &param1->unk_08);
    Pokemon_SetValue(param0, MON_DATA_DEF_IV, &param1->unk_0C);
    Pokemon_SetValue(param0, MON_DATA_SPEED_IV, &param1->unk_10);
    Pokemon_SetValue(param0, MON_DATA_SPATK_IV, &param1->unk_14);
    Pokemon_SetValue(param0, MON_DATA_SPDEF_IV, &param1->unk_18);
    Pokemon_SetValue(param0, MON_DATA_COOL, &param1->unk_24);
    Pokemon_SetValue(param0, MON_DATA_BEAUTY, &param1->unk_28);
    Pokemon_SetValue(param0, MON_DATA_CUTE, &param1->unk_2C);
    Pokemon_SetValue(param0, MON_DATA_SMART, &param1->unk_30);
    Pokemon_SetValue(param0, MON_DATA_TOUGH, &param1->unk_34);
    Pokemon_SetValue(param0, MON_DATA_HELD_ITEM, &param1->unk_3C);

    v0 = ov6_022462E4(param4, 4 + param3);

    Pokemon_SetValue(param0, MON_DATA_OTNAME_STRBUF, v0);
    Strbuf_Free(v0);
    Pokemon_SetValue(param0, MON_DATA_OT_GENDER, &param1->unk_40);
    Pokemon_SetValue(param0, MON_DATA_LANGUAGE, &param1->unk_48);

    v2 = MapHeader_GetMapLabelTextID(param5);

    sub_0209304C(param0, NULL, 1, v2, param4);
    Pokemon_CalcLevelAndStats(param0);

    GF_ASSERT(Pokemon_IsShiny(param0) == 0);
}
