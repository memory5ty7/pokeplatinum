#ifndef POKEPLATINUM_OV21_021D0D80_H
#define POKEPLATINUM_OV21_021D0D80_H

#include <nnsys.h>

#include "overlay021/struct_ov21_021D0D80.h"
#include "overlay021/struct_ov21_021D0F60_decl.h"
#include "overlay021/struct_ov21_021D13FC.h"
#include "overlay021/struct_ov21_021D22F8.h"
#include "overlay021/struct_ov21_021D3320.h"
#include "overlay021/struct_ov21_021D3FE0_decl.h"
#include "overlay021/struct_ov21_021D423C_decl.h"
#include "overlay021/struct_ov21_021D4C0C_decl.h"
#include "overlay021/struct_ov21_021D4CA0.h"
#include "overlay021/struct_ov21_021E68F4.h"

#include "bg_window.h"
#include "cell_actor.h"
#include "heap.h"
#include "overlay_manager.h"
#include "strbuf.h"
#include "touch_screen.h"

typedef struct PokedexLoadingScreen {
    BgConfig *bgConfig;
    NNSG2dScreenData *screenData;
    int layer;
    int topStart;
    int bottomStart;
    int topDist;
    int bottomDist;
    int counter;
    int duration;
} PokedexLoadingScreen;

int ov21_021D0D80(OverlayManager *param0, int *param1);
int ov21_021D0E3C(OverlayManager *param0, int *param1);
int ov21_021D0EC8(OverlayManager *param0, int *param1);
UnkStruct_ov21_021D0F60 *ov21_021D0F60(enum HeapId heapID, const UnkStruct_ov21_021D0D80 *param1);
BOOL ov21_021D10B8(UnkStruct_ov21_021D0F60 *param0);
void ov21_021D12B0(int param0);
void ov21_021D12C4(void);
void ov21_021D12D8(UnkStruct_ov21_021D0F60 *param0);
UnkStruct_ov21_021D3FE0 *ov21_021D12EC(UnkStruct_ov21_021D0F60 *param0);
UnkStruct_ov21_021D3FE0 *ov21_021D1300(UnkStruct_ov21_021D0F60 *param0);
UnkStruct_ov21_021D3FE0 *ov21_021D1314(UnkStruct_ov21_021D0F60 *param0);
UnkStruct_ov21_021D3FE0 *ov21_021D1328(UnkStruct_ov21_021D0F60 *param0);
UnkStruct_ov21_021D423C *ov21_021D133C(UnkStruct_ov21_021D0F60 *param0);
UnkStruct_ov21_021D423C *ov21_021D1350(UnkStruct_ov21_021D0F60 *param0);
UnkStruct_ov21_021D423C *ov21_021D1364(UnkStruct_ov21_021D0F60 *param0);
UnkStruct_ov21_021D423C *ov21_021D1378(UnkStruct_ov21_021D0F60 *param0);
int *ov21_021D138C(UnkStruct_ov21_021D0F60 *param0);
int *ov21_021D13A0(UnkStruct_ov21_021D0F60 *param0);
int *ov21_021D13B4(UnkStruct_ov21_021D0F60 *param0);
int *ov21_021D13C8(UnkStruct_ov21_021D0F60 *param0);
BOOL *ov21_021D13DC(UnkStruct_ov21_021D0F60 *param0);
UnkStruct_ov21_021D3320 *ov21_021D13EC(UnkStruct_ov21_021D0F60 *param0);
UnkStruct_ov21_021D13FC *ov21_021D13FC(UnkStruct_ov21_021D0F60 *param0);
UnkStruct_ov21_021E68F4 *ov21_021D1410(UnkStruct_ov21_021D0F60 *param0, int param1);
UnkStruct_ov21_021E68F4 *ov21_021D1430(UnkStruct_ov21_021D0F60 *param0, int param1);
void ov21_021D144C(CellActor *param0, int param1);
void ov21_021D1498(CellActor *param0, UnkStruct_ov21_021D4CA0 *param1, int param2);
void ov21_021D1524(CellActor *param0, UnkStruct_ov21_021D4CA0 *param1, int param2, int param3, int param4);
void ov21_021D154C(TouchScreenHitTable *hitTable, int param1, int param2, int param3, int param4);
void Pokedex_SetLoadingScreenParams(PokedexLoadingScreen *param0, BgConfig *param1, int param2, NNSG2dScreenData *param3, int param4, int param5, int param6, int param7, int param8);
BOOL Pokedex_LoadingScreenMove(PokedexLoadingScreen *param0);
void ov21_021D1650(Window *param0, int param1, int param2, int param3);
Window *ov21_021D16D8(UnkStruct_ov21_021D13FC *param0, const UnkStruct_ov21_021D3320 *param1, int param2, int param3);
Window *ov21_021D170C(UnkStruct_ov21_021D4C0C *param0, int param1, int param2);
Window *ov21_021D172C(UnkStruct_ov21_021D4C0C *param0, int param1, int param2);
void ov21_021D1778(UnkStruct_ov21_021D13FC *param0, const UnkStruct_ov21_021D3320 *param1, int param2, int param3, fx32 param4, fx32 param5);
void ov21_021D1848(UnkStruct_ov21_021D13FC *param0, int param1, int param2);
void ov21_021D1858(UnkStruct_ov21_021D22F8 *param0, int param1, int param2);
void ov21_021D1890(UnkStruct_ov21_021D13FC *param0, UnkStruct_ov21_021D3320 *param1, int param2, int param3, int param4, int param5);
u32 ov21_021D19D8(UnkStruct_ov21_021D13FC *param0, const UnkStruct_ov21_021D3320 *param1, int param2, int param3, int param4, int param5, int param6, int param7);
u32 ov21_021D1A34(UnkStruct_ov21_021D13FC *param0, const UnkStruct_ov21_021D3320 *param1, int param2, int param3, int param4, int param5, int param6);
u32 ov21_021D1A78(UnkStruct_ov21_021D13FC *param0, const UnkStruct_ov21_021D3320 *param1, int param2, int param3, int param4, int param5, int param6);
u32 ov21_021D1AC0(UnkStruct_ov21_021D13FC *param0, const UnkStruct_ov21_021D3320 *param1, int param2, int param3, int param4, int param5, int param6);
u32 ov21_021D1B08(UnkStruct_ov21_021D13FC *param0, const UnkStruct_ov21_021D3320 *param1, int param2, int param3, int param4, int param5, int param6);
u32 ov21_021D1B4C(UnkStruct_ov21_021D13FC *param0, const UnkStruct_ov21_021D3320 *param1, int param2, int param3, int param4, int param5, int param6);
u32 ov21_021D1B94(UnkStruct_ov21_021D13FC *param0, const UnkStruct_ov21_021D3320 *param1, int param2, int param3, int param4, int param5, int param6);
u32 ov21_021D1BD8(UnkStruct_ov21_021D13FC *param0, const UnkStruct_ov21_021D3320 *param1, int param2, int param3, int param4, int param5, int param6);
u32 ov21_021D1C30(UnkStruct_ov21_021D13FC *param0, const UnkStruct_ov21_021D3320 *param1, int param2, int param3, int param4, int param5, int param6);
u32 ov21_021D1C88(UnkStruct_ov21_021D13FC *param0, const UnkStruct_ov21_021D3320 *param1, int param2, int param3, int param4, int param5, int param6);
Strbuf *GetPokedexMessage(int entryID, enum HeapId heapID);

#endif // POKEPLATINUM_OV21_021D0D80_H
