#include "unk_02084B70.h"

#include <nitro.h>
#include <string.h>

#include "struct_defs/struct_0207F248.h"

#include "applications/pokemon_summary_screen/main.h"
#include "field/field_system.h"

#include "bag.h"
#include "bg_window.h"
#include "core_sys.h"
#include "font.h"
#include "heap.h"
#include "item.h"
#include "map_header.h"
#include "menu.h"
#include "message.h"
#include "move_table.h"
#include "party.h"
#include "pokemon.h"
#include "render_window.h"
#include "strbuf.h"
#include "string_list.h"
#include "string_template.h"
#include "text.h"
#include "unk_02005474.h"
#include "unk_0207E0B8.h"
#include "unk_020819DC.h"
#include "unk_02082C2C.h"
#include "unk_02096420.h"

static int PartyMenu_ItemUseFunc_StatusHealEtc(void *param0);
static int PartyMenu_ItemUseFunc_EVDown(void *param0);
static int PartyMenu_ItemUseFunc_HPRestore(void *param0);
static int PartyMenu_ItemUseFunc_HPRestoreAnimLoop(void *param0);
static int PartyMenu_ItemUseFunc_LevelUp(void *param0);
static int PartyMenu_ItemUseFunc_LevelUpLearnMovesLoop(void *param0);
static void PartyMenu_LearnMoveToSlot(GameWindowLayout *param0, Pokemon *param1, u32 param2);
static int PartyMenu_ItemUseFunc_TMHMPromptForgetMove(void *param0);
static int PartyMenu_ItemUseFunc_TMHMAskStopTryingToLearn(void *param0);
static int PartyMenu_ItemUseFunc_TMHMDidNotLearnMove(void *param0);
static int PartyMenu_ItemUseFunc_TMHMAskAgainToForget(void *param0);
static u16 PartyMenu_GetCurrentMapSec(GameWindowLayout *param0);
static int PartyMenu_ItemUseFunc_LevelUpPromptForgetMove(void *param0);
static int PartyMenu_ItemUseFunc_LevelUpAskStopTryingToLearn(void *param0);
static int PartyMenu_ItemUseFunc_LevelUpDidNotLearnMove(void *param0);
static int PartyMenu_ItemUseFunc_LevelUpAskAgainToForget(void *param0);
static u8 nextStateCheck(GameWindowLayout *v0, BOOL mode);

static u8 sub_02084B70(u16 param0)
{
    ItemData *v0;
    s32 v1;

    v0 = Item_Load(param0, 0, 12);

    if (Item_Get(v0, 14) != 1) {
        Heap_FreeToHeap(v0);
        return 28;
    }

    if ((Item_Get(v0, 27) != 0) || (Item_Get(v0, 28) != 0) || (Item_Get(v0, 29) != 0) || (Item_Get(v0, 30) != 0) || (Item_Get(v0, 31) != 0) || (Item_Get(v0, 32) != 0) || (Item_Get(v0, 33) != 0)) {
        Heap_FreeToHeap(v0);
        return 0;
    }

    if (Item_Get(v0, 24) != 0) {
        Heap_FreeToHeap(v0);
        return 1;
    }

    if (Item_Get(v0, 25) != 0) {
        Heap_FreeToHeap(v0);
        return 2;
    }

    v1 = Item_Get(v0, 15);
    v1 += (Item_Get(v0, 16) << 1);
    v1 += (Item_Get(v0, 17) << 2);
    v1 += (Item_Get(v0, 18) << 3);
    v1 += (Item_Get(v0, 19) << 4);
    v1 += (Item_Get(v0, 20) << 5);

    switch (v1) {
    case 0x1:
        Heap_FreeToHeap(v0);
        return 3;
    case 0x2:
        Heap_FreeToHeap(v0);
        return 4;
    case 0x4:
        Heap_FreeToHeap(v0);
        return 5;
    case 0x8:
        Heap_FreeToHeap(v0);
        return 6;
    case 0x10:
        Heap_FreeToHeap(v0);
        return 7;
    case 0x20:
        Heap_FreeToHeap(v0);
        return 8;
    case 0x3f:
        if (Item_Get(v0, 38) != 0) {
            Heap_FreeToHeap(v0);
            return 11;
        } else {
            Heap_FreeToHeap(v0);
            return 9;
        }
    }

    if (Item_Get(v0, 21) != 0) {
        Heap_FreeToHeap(v0);
        return 10;
    }

    if (Item_Get(v0, 38) != 0) {
        Heap_FreeToHeap(v0);
        return 11;
    }

    v1 = Item_Get(v0, 48);

    if (v1 > 0) {
        Heap_FreeToHeap(v0);
        return 12;
    }

    if (v1 < 0) {
        Heap_FreeToHeap(v0);
        return 18;
    }

    v1 = Item_Get(v0, 49);

    if (v1 > 0) {
        Heap_FreeToHeap(v0);
        return 13;
    }

    if (v1 < 0) {
        Heap_FreeToHeap(v0);
        return 19;
    }

    v1 = Item_Get(v0, 50);

    if (v1 > 0) {
        Heap_FreeToHeap(v0);
        return 14;
    }

    if (v1 < 0) {
        Heap_FreeToHeap(v0);
        return 20;
    }

    v1 = Item_Get(v0, 51);

    if (v1 > 0) {
        Heap_FreeToHeap(v0);
        return 15;
    }

    if (v1 < 0) {
        Heap_FreeToHeap(v0);
        return 21;
    }

    v1 = Item_Get(v0, 52);

    if (v1 > 0) {
        Heap_FreeToHeap(v0);
        return 16;
    }

    if (v1 < 0) {
        Heap_FreeToHeap(v0);
        return 22;
    }

    v1 = Item_Get(v0, 53);

    if (v1 > 0) {
        Heap_FreeToHeap(v0);
        return 17;
    }

    if (v1 < 0) {
        Heap_FreeToHeap(v0);
        return 23;
    }

    if (Item_Get(v0, 26) != 0) {
        Heap_FreeToHeap(v0);
        return 24;
    }

    if (Item_Get(v0, 34) != 0) {
        Heap_FreeToHeap(v0);
        return 25;
    }

    if (Item_Get(v0, 35) != 0) {
        Heap_FreeToHeap(v0);
        return 26;
    }

    if ((Item_Get(v0, 36) != 0) || (Item_Get(v0, 37) != 0)) {
        Heap_FreeToHeap(v0);
        return 27;
    }

    Heap_FreeToHeap(v0);
    return 28;
}

static void sub_02084E58(GameWindowLayout *param0, u16 param1, u32 param2)
{
    Pokemon *v0;
    Strbuf *v1;

    v0 = Party_GetPokemonBySlotIndex(param0->unk_5A4->unk_00, param0->unk_B11);
    StringTemplate_SetNickname(param0->unk_6A0, 0, Pokemon_GetBoxPokemon(v0));

    switch (sub_02084B70(param1)) {
    case 4:
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 65);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        break;
    case 3:
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 91);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        break;
    case 5:
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 67);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        break;
    case 6:
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 68);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        break;
    case 7:
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 66);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        break;
    case 8:
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 73);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        break;
    case 10:
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 74);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        break;
    case 9:
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 71);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        break;
    case 12:
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 75);
        StringTemplate_SetPokemonStatName(param0->unk_6A0, 1, 0);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        break;
    case 13:
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 75);
        StringTemplate_SetPokemonStatName(param0->unk_6A0, 1, 1);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        break;
    case 14:
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 75);
        StringTemplate_SetPokemonStatName(param0->unk_6A0, 1, 2);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        break;
    case 15:
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 75);
        StringTemplate_SetPokemonStatName(param0->unk_6A0, 1, 3);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        break;
    case 16:
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 75);
        StringTemplate_SetPokemonStatName(param0->unk_6A0, 1, 4);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        break;
    case 17:
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 75);
        StringTemplate_SetPokemonStatName(param0->unk_6A0, 1, 5);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        break;
    case 18:
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 110 + param2);
        StringTemplate_SetPokemonStatName(param0->unk_6A0, 1, 0);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        break;
    case 19:
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 110 + param2);
        StringTemplate_SetPokemonStatName(param0->unk_6A0, 1, 1);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        break;
    case 20:
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 110 + param2);
        StringTemplate_SetPokemonStatName(param0->unk_6A0, 1, 2);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        break;
    case 21:
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 110 + param2);
        StringTemplate_SetPokemonStatName(param0->unk_6A0, 1, 3);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        break;
    case 22:
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 110 + param2);
        StringTemplate_SetPokemonStatName(param0->unk_6A0, 1, 4);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        break;
    case 23:
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 110 + param2);
        StringTemplate_SetPokemonStatName(param0->unk_6A0, 1, 5);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        break;
    case 25:
    case 26:
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 72);
        StringTemplate_SetMoveName(param0->unk_6A0, 0, param2);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        break;
    case 27:
        MessageLoader_GetStrbuf(param0->unk_69C, 69, param0->unk_6A4);
        break;
    default:
        MessageLoader_GetStrbuf(param0->unk_69C, 105, param0->unk_6A4);
    }
}

void PartyMenu_SetItemUseFuncFromBagSelection(GameWindowLayout *param0)
{
    switch (sub_02084B70(param0->unk_5A4->unk_24)) {
    case 0:
    case 28:
        break;
    case 1:
        break;
    case 2:
        param0->unk_B00 = PartyMenu_ItemUseFunc_LevelUp;
        break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 25:
    case 26:
    case 27:
        param0->unk_B00 = PartyMenu_ItemUseFunc_StatusHealEtc;
        break;
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
        param0->unk_B00 = PartyMenu_ItemUseFunc_EVDown;
        break;
    case 11:
        param0->unk_B00 = PartyMenu_ItemUseFunc_HPRestore;
        break;
    }
}

int PartyMenu_ItemUseFunc_WaitTextPrinterThenExit(void *param0)
{
    GameWindowLayout *v0 = (GameWindowLayout *)param0;

    if (Text_IsPrinterActive(v0->unk_B10) != 0) {
        return PARTY_MENU_STATE_ITEM_USE_CB;
    }

    if (gCoreSys.pressedKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        v0->unk_5A4->unk_23 = 0;
        return PARTY_MENU_STATE_BEGIN_EXIT;
    }

    return PARTY_MENU_STATE_ITEM_USE_CB;
}

static int PartyMenu_ItemUseFunc_StatusHealEtc(void *param0)
{
    GameWindowLayout *v0 = (GameWindowLayout *)param0;

    sub_02096F14(v0->unk_5A4->unk_00, v0->unk_5A4->unk_24, v0->unk_B11, 0, PartyMenu_GetCurrentMapSec(v0), 12);
    sub_0207EF14(v0, v0->unk_B11);
    sub_020821F8(v0, v0->unk_B11);
    sub_020822BC(v0, v0->unk_B11);
    sub_02083014(v0, v0->unk_B11, v0->unk_704[v0->unk_B11].unk_0E_0);
    sub_02084E58(v0, v0->unk_5A4->unk_24, 0);
    sub_02082708(v0, 0xffffffff, 1);
    Sound_PlayEffect(1516);

    v0->unk_B00 = PartyMenu_ItemUseFunc_WaitTextPrinterThenExit;

    return PARTY_MENU_STATE_ITEM_USE_CB;
}

static int PartyMenu_ItemUseFunc_EVDown(void *param0)
{
    GameWindowLayout *v0;
    Pokemon *v1;
    u8 v2[7];

    v0 = (GameWindowLayout *)param0;
    v1 = Party_GetPokemonBySlotIndex(v0->unk_5A4->unk_00, v0->unk_B11);

    v2[0] = Pokemon_GetValue(v1, MON_DATA_HP_EV, NULL);
    v2[1] = Pokemon_GetValue(v1, MON_DATA_ATK_EV, NULL);
    v2[2] = Pokemon_GetValue(v1, MON_DATA_DEF_EV, NULL);
    v2[3] = Pokemon_GetValue(v1, MON_DATA_SPEED_EV, NULL);
    v2[4] = Pokemon_GetValue(v1, MON_DATA_SPATK_EV, NULL);
    v2[5] = Pokemon_GetValue(v1, MON_DATA_SPDEF_EV, NULL);
    v2[6] = Pokemon_GetValue(v1, MON_DATA_FRIENDSHIP, NULL);

    sub_02096F14(v0->unk_5A4->unk_00, v0->unk_5A4->unk_24, v0->unk_B11, 0, PartyMenu_GetCurrentMapSec(v0), 12);
    sub_0207EF14(v0, v0->unk_B11);
    sub_020821F8(v0, v0->unk_B11);
    sub_020822BC(v0, v0->unk_B11);
    sub_02083014(v0, v0->unk_B11, v0->unk_704[v0->unk_B11].unk_0E_0);

    if ((v2[0] != Pokemon_GetValue(v1, MON_DATA_HP_EV, NULL)) || (v2[1] != Pokemon_GetValue(v1, MON_DATA_ATK_EV, NULL)) || (v2[2] != Pokemon_GetValue(v1, MON_DATA_DEF_EV, NULL)) || (v2[3] != Pokemon_GetValue(v1, MON_DATA_SPEED_EV, NULL)) || (v2[4] != Pokemon_GetValue(v1, MON_DATA_SPATK_EV, NULL)) || (v2[5] != Pokemon_GetValue(v1, MON_DATA_SPDEF_EV, NULL))) {
        if (v2[6] != Pokemon_GetValue(v1, MON_DATA_FRIENDSHIP, NULL)) {
            sub_02084E58(v0, v0->unk_5A4->unk_24, 0);
        } else {
            sub_02084E58(v0, v0->unk_5A4->unk_24, 1);
        }
    } else {
        sub_02084E58(v0, v0->unk_5A4->unk_24, 2);
    }

    sub_02082708(v0, 0xffffffff, 1);
    v0->unk_B00 = PartyMenu_ItemUseFunc_WaitTextPrinterThenExit;

    return PARTY_MENU_STATE_ITEM_USE_CB;
}

static int PartyMenu_ItemUseFunc_HPRestore(void *param0)
{
    GameWindowLayout *v0;
    Pokemon *v1;
    Strbuf *v2;
    u32 v3;
    u32 v4;

    v0 = (GameWindowLayout *)param0;

    sub_02096F14(v0->unk_5A4->unk_00, v0->unk_5A4->unk_24, v0->unk_B11, 0, PartyMenu_GetCurrentMapSec(v0), 12);

    v1 = Party_GetPokemonBySlotIndex(v0->unk_5A4->unk_00, v0->unk_B11);
    v3 = Pokemon_GetValue(v1, MON_DATA_CURRENT_HP, NULL);

    if (v0->unk_704[v0->unk_B11].unk_06 == 0) {
        v2 = MessageLoader_GetNewStrbuf(v0->unk_69C, 70);
    } else if (v0->unk_704[v0->unk_B11].unk_06 == v3) {
        v2 = MessageLoader_GetNewStrbuf(v0->unk_69C, 71);
    } else {
        v2 = MessageLoader_GetNewStrbuf(v0->unk_69C, 64);
    }

    StringTemplate_SetNickname(v0->unk_6A0, 0, Pokemon_GetBoxPokemon(v1));
    StringTemplate_SetNumber(v0->unk_6A0, 1, v3 - v0->unk_704[v0->unk_B11].unk_06, 3, 0, 1);
    StringTemplate_Format(v0->unk_6A0, v0->unk_6A4, v2);
    Strbuf_Free(v2);

    v4 = PokemonSummaryScreen_StatusIconAnimIdx(v1);
    sub_02083014(v0, v0->unk_B11, v4);

    if (v4 == 7) {
        v0->unk_704[v0->unk_B11].unk_0E_0 = 7;
        sub_02081FFC(v0, v0->unk_B11);
    }

    sub_0207F8F8(v0, v0->unk_B11);
    v0->unk_B00 = PartyMenu_ItemUseFunc_HPRestoreAnimLoop;
    Sound_PlayEffect(1516);

    return PARTY_MENU_STATE_ITEM_USE_CB;
}

static int PartyMenu_ItemUseFunc_HPRestoreAnimLoop(void *param0)
{
    GameWindowLayout *v0 = (GameWindowLayout *)param0;
    Pokemon *v1;
    u32 v2;

    v1 = Party_GetPokemonBySlotIndex(v0->unk_5A4->unk_00, v0->unk_B11);
    v2 = Pokemon_GetValue(v1, MON_DATA_CURRENT_HP, NULL);

    if (v0->unk_704[v0->unk_B11].unk_06 != v2) {
        v0->unk_704[v0->unk_B11].unk_06++;
    }

    sub_02082098(v0, v0->unk_B11);
    Window_FillTilemap(&v0->unk_04[3 + v0->unk_B11 * 5], 0);
    sub_02082058(v0, v0->unk_B11);
    sub_02082104(v0, v0->unk_B11);

    if (v0->unk_704[v0->unk_B11].unk_06 == v2) {
        sub_02082708(v0, 0xffffffff, 1);
        v0->unk_B00 = PartyMenu_ItemUseFunc_WaitTextPrinterThenExit;
    }

    return PARTY_MENU_STATE_ITEM_USE_CB;
}

BOOL ItemId_IsReviveAll(u16 param0)
{
    if (Item_LoadParam(param0, 24, 12) != 0) {
        return 1;
    }

    return 0;
}

static u8 PartyMenu_SacredAshGetNextMonId(GameWindowLayout *param0, u8 param1)
{
    u8 v0;

    if (param1 >= 6) {
        return 0xff;
    }

    for (v0 = param1; v0 < 6; v0++) {
        if ((param0->unk_704[v0].unk_29 != 0) && (param0->unk_704[v0].unk_06 == 0)) {
            return v0;
        }
    }

    return 0xff;
}

int PartyMenu_Subtask_SacredAsh(GameWindowLayout *param0)
{
    Pokemon *v0;
    Strbuf *v1;
    u32 v2;
    u8 v3;

    switch (param0->unk_B0E) {
    case 0:
        param0->unk_B11 = PartyMenu_SacredAshGetNextMonId(param0, 0);

        if (param0->unk_B11 == 0xff) {
            MessageLoader_GetStrbuf(param0->unk_69C, 105, param0->unk_6A4);
            sub_02082708(param0, 0xffffffff, 1);
            param0->unk_B00 = PartyMenu_ItemUseFunc_WaitTextPrinterThenExit;
            sub_0208327C(param0, 0, 1);
            param0->unk_B11 = 7;

            return PARTY_MENU_STATE_ITEM_USE_CB;
        }

        if (param0->unk_B11 != 0) {
            sub_0207F8F8(param0, 0);
        }
    case 1:

        v0 = Party_GetPokemonBySlotIndex(param0->unk_5A4->unk_00, param0->unk_B11);
        ApplyItemEffectsToPokemon(v0, param0->unk_5A4->unk_24, 0, PartyMenu_GetCurrentMapSec(param0), 12);

        v2 = Pokemon_GetValue(v0, MON_DATA_CURRENT_HP, NULL);
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 70);

        StringTemplate_SetNickname(param0->unk_6A0, 0, Pokemon_GetBoxPokemon(v0));
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);

        param0->unk_704[param0->unk_B11].unk_0E_0 = 7;

        sub_02083014(param0, param0->unk_B11, param0->unk_704[param0->unk_B11].unk_0E_0);
        sub_02081FFC(param0, param0->unk_B11);
        sub_0207F8F8(param0, param0->unk_B11);
        sub_0208327C(param0, param0->unk_B11, 1);
        Sound_PlayEffect(1516);

        param0->unk_B0E++;
        break;
    case 2:
        v0 = Party_GetPokemonBySlotIndex(param0->unk_5A4->unk_00, param0->unk_B11);
        v2 = Pokemon_GetValue(v0, MON_DATA_CURRENT_HP, NULL);

        param0->unk_704[param0->unk_B11].unk_06++;

        sub_02082098(param0, param0->unk_B11);
        Window_FillTilemap(&param0->unk_04[3 + param0->unk_B11 * 5], 0);
        sub_02082058(param0, param0->unk_B11);
        sub_02082104(param0, param0->unk_B11);

        if (param0->unk_704[param0->unk_B11].unk_06 == v2) {
            sub_02082708(param0, 0xffffffff, 1);
            param0->unk_B0E++;
        }
        break;
    case 3:
        if (Text_IsPrinterActive(param0->unk_B10) != 0) {
            break;
        }

        if (gCoreSys.pressedKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            Sound_PlayEffect(1500);
            v3 = param0->unk_B11;
            param0->unk_B11 = PartyMenu_SacredAshGetNextMonId(param0, param0->unk_B11 + 1);

            if (param0->unk_B11 != 0xff) {
                Window_EraseMessageBox(&param0->unk_04[34], 0);
                sub_0207F8F8(param0, v3);
                param0->unk_B0E = 1;
            } else {
                Bag_TryRemoveItem(param0->unk_5A4->unk_04, param0->unk_5A4->unk_24, 1, 12);
                param0->unk_5A4->unk_23 = 0;
                param0->unk_B11 = 7;
                return PARTY_MENU_STATE_BEGIN_EXIT;
            }
        }
        break;
    }

    return PARTY_MENU_STATE_SACRED_ASH;
}

static int PartyMenu_ItemUseFunc_LevelUp(void *param0)
{
    GameWindowLayout *v0;
    Pokemon *v1;
    Strbuf *v2;
    u32 v3;
    u32 v4;

    v0 = (GameWindowLayout *)param0;
    v1 = Party_GetPokemonBySlotIndex(v0->unk_5A4->unk_00, v0->unk_B11);

    v0->unk_B14[0] = (u16)Pokemon_GetValue(v1, MON_DATA_MAX_HP, NULL);
    v0->unk_B14[1] = (u16)Pokemon_GetValue(v1, MON_DATA_ATK, NULL);
    v0->unk_B14[2] = (u16)Pokemon_GetValue(v1, MON_DATA_DEF, NULL);
    v0->unk_B14[3] = (u16)Pokemon_GetValue(v1, MON_DATA_SP_ATK, NULL);
    v0->unk_B14[4] = (u16)Pokemon_GetValue(v1, MON_DATA_SP_DEF, NULL);
    v0->unk_B14[5] = (u16)Pokemon_GetValue(v1, MON_DATA_SPEED, NULL);

    sub_02096F14(v0->unk_5A4->unk_00, v0->unk_5A4->unk_24, v0->unk_B11, 0, PartyMenu_GetCurrentMapSec(v0), 12);

    v0->unk_704[v0->unk_B11].unk_0A = Pokemon_GetValue(v1, MON_DATA_LEVEL, NULL);
    v0->unk_704[v0->unk_B11].unk_06 = Pokemon_GetValue(v1, MON_DATA_CURRENT_HP, NULL);
    v0->unk_704[v0->unk_B11].unk_08 = Pokemon_GetValue(v1, MON_DATA_MAX_HP, NULL);

    v2 = MessageLoader_GetNewStrbuf(v0->unk_69C, 193);

    StringTemplate_SetNickname(v0->unk_6A0, 0, Pokemon_GetBoxPokemon(v1));
    StringTemplate_SetNumber(v0->unk_6A0, 1, v0->unk_704[v0->unk_B11].unk_0A, 3, 0, 1);
    StringTemplate_Format(v0->unk_6A0, v0->unk_6A4, v2);
    Strbuf_Free(v2);

    v4 = PokemonSummaryScreen_StatusIconAnimIdx(v1);
    sub_02083014(v0, v0->unk_B11, v4);

    if (v4 == 7) {
        v0->unk_704[v0->unk_B11].unk_0E_0 = 7;
        sub_02081FFC(v0, v0->unk_B11);
    }

    sub_0207F8F8(v0, v0->unk_B11);

    v0->unk_B00 = PartyMenu_ItemUseFunc_HPRestoreAnimLoop;

    sub_020821F8(v0, v0->unk_B11);
    sub_020822BC(v0, v0->unk_B11);
    sub_02082708(v0, 0xffffffff, 1);

    v0->unk_B00 = PartyMenu_ItemUseFunc_LevelUpLearnMovesLoop;
    v0->unk_B13 = 0;

    return PARTY_MENU_STATE_ITEM_USE_CB;
}

static int PartyMenu_ItemUseFunc_LevelUpLearnMovesLoop(void *param0)
{
    GameWindowLayout *v0;
    Pokemon *v1;
    Strbuf *v2;

    v0 = param0;

    switch (v0->unk_B13) {
    case 0:
        if (Text_IsPrinterActive(v0->unk_B10) == 0) {
            if (gCoreSys.pressedKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
                Sound_PlayEffect(1500);
                sub_020829DC(v0);
                v0->unk_B13 = 1;
            }
        }
        break;
    case 1:
        if (gCoreSys.pressedKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            Sound_PlayEffect(1500);
            sub_02082B58(v0);
            v0->unk_B13 = 2;
        }
        break;
    case 2:
        if (gCoreSys.pressedKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            Sound_PlayEffect(1500);
            sub_02082C10(v0);
            v0->unk_B13 = 3;
            v0->unk_5A4->unk_34 = 0;
        }
        break;
    case 3:
        v1 = Party_GetPokemonBySlotIndex(v0->unk_5A4->unk_00, v0->unk_B11);

        switch (Pokemon_LevelUpMove(v1, &v0->unk_5A4->unk_34, &v0->unk_5A4->unk_26)) {
        case 0x0:
            v0->unk_B13 = 6;
            break;
        case 0xffff:
            StringTemplate_SetNickname(v0->unk_6A0, 0, Pokemon_GetBoxPokemon(v1));
            StringTemplate_SetMoveName(v0->unk_6A0, 1, v0->unk_5A4->unk_26);

            v2 = MessageLoader_GetNewStrbuf(v0->unk_69C, 52);

            StringTemplate_Format(v0->unk_6A0, v0->unk_6A4, v2);
            Strbuf_Free(v2);
            sub_02082708(v0, 0xffffffff, 0);

            v0->unk_B04.unk_00 = PartyMenu_ItemUseFunc_LevelUpPromptForgetMove;
            v0->unk_B04.unk_04 = PartyMenu_ItemUseFunc_LevelUpAskStopTryingToLearn;
            v0->unk_B0E = PARTY_MENU_STATE_YES_NO_INIT;

            return PARTY_MENU_STATE_WAIT_TEXT_PRINTER;
        case 0xfffe:
            break;
        default:
            StringTemplate_SetNickname(v0->unk_6A0, 0, Pokemon_GetBoxPokemon(v1));
            StringTemplate_SetMoveName(v0->unk_6A0, 1, v0->unk_5A4->unk_26);

            v2 = MessageLoader_GetNewStrbuf(v0->unk_69C, 194);

            StringTemplate_Format(v0->unk_6A0, v0->unk_6A4, v2);
            Strbuf_Free(v2);
            sub_02082708(v0, 0xffffffff, 0);
            v0->unk_B13 = 4;
            break;
        }
        break;
    case 4:
        if (Text_IsPrinterActive(v0->unk_B10) == 0) {
            if (gCoreSys.pressedKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
                Sound_PlayEffect(1500);
                v0->unk_B13 = 3;
            }
        }
        break;
    case 5:
        if (gCoreSys.pressedKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            v1 = Party_GetPokemonBySlotIndex(v0->unk_5A4->unk_00, v0->unk_B11);
            PartyMenu_LearnMoveToSlot(v0, v1, v0->unk_5A4->unk_28);
            v2 = MessageLoader_GetNewStrbuf(v0->unk_69C, 61);

            StringTemplate_SetMoveName(v0->unk_6A0, 1, v0->unk_5A4->unk_26);
            StringTemplate_Format(v0->unk_6A0, v0->unk_6A4, v2);
            Strbuf_Free(v2);
            sub_02082708(v0, 0xffffffff, 0);

            v0->unk_B13 = 4;
        }
        break;
    case 6: {
        Pokemon *v3;
        FieldSystem *fieldSystem;
        int v5;

        v3 = Party_GetPokemonBySlotIndex(v0->unk_5A4->unk_00, v0->unk_B11);
        fieldSystem = v0->unk_5A4->unk_1C;
        v5 = MapHeader_GetMapEvolutionMethod(fieldSystem->location->mapId);
        v0->unk_5A4->unk_38 = sub_02076B94(v0->unk_5A4->unk_00, v3, 0, v5, &v0->unk_5A4->unk_3C);

        if (v0->unk_5A4->unk_38 != 0) {
            v0->unk_5A4->unk_23 = 9;
            return PARTY_MENU_STATE_BEGIN_EXIT;
        } else {
            v0->unk_5A4->unk_23 = 0;
            return nextStateCheck(v0, TRUE);
        }
    }
        return nextStateCheck(v0, TRUE);
    }

    return PARTY_MENU_STATE_ITEM_USE_CB;
}

static u8 nextStateCheck(GameWindowLayout *v0, BOOL mode) {
        if (Bag_GetItemQuantity(v0->unk_5A4->unk_04, v0->unk_5A4->unk_24, 12) == 0) {
            if (mode == TRUE) {
                return PARTY_MENU_STATE_BEGIN_EXIT;
            } else {
                return PARTY_MENU_STATE_AFTER_MESSAGE_BEGIN_EXIT;
            }
        } else {
            Window_EraseMessageBox(&v0->unk_04[34], 0);
            if ((v0->unk_5A4->unk_20 == 5) || (v0->unk_5A4->unk_20 == 16)) {
                if (ItemId_IsReviveAll(v0->unk_5A4->unk_24) == 0) {
                    sub_020826E0(v0, 32, 1);
                }
            } else if (v0->unk_5A4->unk_20 == 6) {
                sub_020826E0(v0, 33, 1);
            } else if ((v0->unk_5A4->unk_20 == 9) || (v0->unk_5A4->unk_20 == 14)) {
                sub_020826E0(v0, 31, 1);
            } else if ((v0->unk_5A4->unk_20 == 7) || (v0->unk_5A4->unk_20 == 8) || (v0->unk_5A4->unk_20 == 11) || (v0->unk_5A4->unk_20 == 12)) {
                sub_0200D414(v0->unk_5B0[6], 1);
            } else if ((v0->unk_5A4->unk_20 == 2) || (v0->unk_5A4->unk_20 == 17)) {
                sub_020826E0(v0, 34, 1);
            } else if (v0->unk_5A4->unk_20 == 15) {
                sub_020826E0(v0, 34, 1);
            } else if (v0->unk_5A4->unk_20 == 22) {
                sub_020826E0(v0, 34, 1);
            } else if (v0->unk_5A4->unk_20 == 23) {
                sub_020826E0(v0, 34, 1);
            } else if (v0->unk_5A4->unk_20 == 21) {
                sub_020826E0(v0, 197, 1);
            } else if (v0->unk_5A4->unk_20 != 10) {
                sub_020826E0(v0, 29, 1);
            } else {
                sub_0200D414(v0->unk_5B0[6], 1);
            }
            sub_0207F8F8(v0, v0->unk_B11);
            v0->unk_B0E = 1;
            return PARTY_MENU_STATE_USE_ITEM_SELECT_MON;
        }
}

int PartyMenu_ItemUseFunc_LevelUpDoLearnMove(GameWindowLayout *param0)
{
    Pokemon *v0;
    Strbuf *v1;

    param0->unk_B00 = PartyMenu_ItemUseFunc_LevelUpLearnMovesLoop;
    param0->unk_B13 = 3;

    v0 = Party_GetPokemonBySlotIndex(param0->unk_5A4->unk_00, param0->unk_B11);
    StringTemplate_SetNickname(param0->unk_6A0, 0, Pokemon_GetBoxPokemon(v0));

    if (param0->unk_5A4->unk_28 == 4) {
        StringTemplate_SetMoveName(param0->unk_6A0, 1, param0->unk_5A4->unk_26);
        return PartyMenu_ItemUseFunc_LevelUpAskStopTryingToLearn(param0);
    }

    StringTemplate_SetMoveName(param0->unk_6A0, 1, Pokemon_GetValue(v0, 54 + param0->unk_5A4->unk_28, NULL));
    v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 60);
    StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
    Strbuf_Free(v1);
    sub_02082708(param0, 0xffffffff, 1);

    param0->unk_B0E = PARTY_MENU_STATE_ITEM_USE_CB;
    param0->unk_B13 = 5;

    return PARTY_MENU_STATE_WAIT_TEXT_PRINTER;
}

static int PartyMenu_ItemUseFunc_LevelUpPromptForgetMove(void *param0)
{
    GameWindowLayout *v0;
    Strbuf *v1;

    v0 = param0;
    v1 = MessageLoader_GetNewStrbuf(v0->unk_69C, 59);

    StringTemplate_Format(v0->unk_6A0, v0->unk_6A4, v1);
    Strbuf_Free(v1);
    sub_02082708(v0, 0xffffffff, 0);

    v0->unk_5A4->unk_23 = PARTY_MENU_ACTION_RETURN_5;
    v0->unk_B0E = PARTY_MENU_STATE_AFTER_MESSAGE_BEGIN_EXIT;

    return PARTY_MENU_STATE_WAIT_TEXT_PRINTER;
}

static int PartyMenu_ItemUseFunc_LevelUpAskStopTryingToLearn(void *param0)
{
    GameWindowLayout *v0;
    Strbuf *v1;

    v0 = param0;
    v1 = MessageLoader_GetNewStrbuf(v0->unk_69C, 55);

    StringTemplate_Format(v0->unk_6A0, v0->unk_6A4, v1);
    Strbuf_Free(v1);
    sub_02082708(v0, 0xffffffff, 1);

    v0->unk_B04.unk_00 = PartyMenu_ItemUseFunc_LevelUpDidNotLearnMove;
    v0->unk_B04.unk_04 = PartyMenu_ItemUseFunc_LevelUpAskAgainToForget;
    v0->unk_B0E = PARTY_MENU_STATE_YES_NO_INIT;

    return PARTY_MENU_STATE_WAIT_TEXT_PRINTER;
}

static int PartyMenu_ItemUseFunc_LevelUpDidNotLearnMove(void *param0)
{
    GameWindowLayout *v0;
    Strbuf *v1;

    v0 = param0;
    v1 = MessageLoader_GetNewStrbuf(v0->unk_69C, 58);

    StringTemplate_Format(v0->unk_6A0, v0->unk_6A4, v1);
    Strbuf_Free(v1);
    sub_02082708(v0, 0xffffffff, 0);

    v0->unk_B0E = PARTY_MENU_STATE_ITEM_USE_CB;
    v0->unk_B13 = 4;

    return PARTY_MENU_STATE_WAIT_TEXT_PRINTER;
}

static int PartyMenu_ItemUseFunc_LevelUpAskAgainToForget(void *param0)
{
    GameWindowLayout *v0;
    Strbuf *v1;

    v0 = param0;
    v1 = MessageLoader_GetNewStrbuf(v0->unk_69C, 52);

    StringTemplate_Format(v0->unk_6A0, v0->unk_6A4, v1);
    Strbuf_Free(v1);
    sub_02082708(v0, 0xffffffff, 0);

    v0->unk_B04.unk_00 = PartyMenu_ItemUseFunc_LevelUpPromptForgetMove;
    v0->unk_B04.unk_04 = PartyMenu_ItemUseFunc_LevelUpAskStopTryingToLearn;
    v0->unk_B0E = PARTY_MENU_STATE_YES_NO_INIT;

    return PARTY_MENU_STATE_WAIT_TEXT_PRINTER;
}

u8 PartyMenu_CheckCanLearnTMHMMove(GameWindowLayout *param0, Pokemon *param1)
{
    u16 v0;
    u8 v1;

    for (v1 = 0; v1 < LEARNED_MOVES_MAX; v1++) {
        v0 = (u16)Pokemon_GetValue(param1, MON_DATA_MOVE1 + v1, NULL);

        if (v0 == param0->unk_5A4->unk_26) {
            return 0xfd;
        }

        if (v0 == 0) {
            break;
        }
    }

    if (Pokemon_CanLearnTM(param1, Item_TMHMNumber(param0->unk_5A4->unk_24)) == 0) {
        return 0xff;
    }

    if (v1 == 4) {
        return 0xfe;
    }

    return v1;
}

int PartyMenu_HandleUseTMHMonMon(GameWindowLayout *param0)
{
    Pokemon *v0;
    Strbuf *v1;
    u32 v2;

    v0 = Party_GetPokemonBySlotIndex(param0->unk_5A4->unk_00, param0->unk_B11);
    v2 = PartyMenu_CheckCanLearnTMHMMove(param0, v0);

    StringTemplate_SetNickname(param0->unk_6A0, 0, Pokemon_GetBoxPokemon(v0));
    StringTemplate_SetMoveName(param0->unk_6A0, 1, param0->unk_5A4->unk_26);

    switch (v2) {
    case 0:
    case 1:
    case 2:
    case 3:
        PartyMenu_LearnMoveToSlot(param0, v0, v2);
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 61);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        sub_02082708(param0, 0xffffffff, 1);
        param0->unk_5A4->unk_23 = 0;
        param0->unk_B0E = 25;
        break;
    case 0xfd:
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 63);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        sub_02082708(param0, 0xffffffff, 1);
        param0->unk_5A4->unk_23 = 0;
        param0->unk_B0E = 25;
        break;
    case 0xfe:
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 52);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        sub_02082708(param0, 0xffffffff, 1);

        param0->unk_B04.unk_00 = PartyMenu_ItemUseFunc_TMHMPromptForgetMove;
        param0->unk_B04.unk_04 = PartyMenu_ItemUseFunc_TMHMAskStopTryingToLearn;
        param0->unk_B0E = PARTY_MENU_STATE_YES_NO_INIT;
        break;
    case 0xff:
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 62);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        sub_02082708(param0, 0xffffffff, 1);

        param0->unk_5A4->unk_23 = 0;
        param0->unk_B0E = nextStateCheck(param0, FALSE);
        break;
    }

    return PARTY_MENU_STATE_WAIT_TEXT_PRINTER;
}

int PartyMenu_ItemUseFunc_TMHMDoLearnMove(GameWindowLayout *param0)
{
    Pokemon *v0;
    Strbuf *v1;

    v0 = Party_GetPokemonBySlotIndex(param0->unk_5A4->unk_00, param0->unk_B11);
    StringTemplate_SetNickname(param0->unk_6A0, 0, Pokemon_GetBoxPokemon(v0));

    if (param0->unk_5A4->unk_28 == 4) {
        StringTemplate_SetMoveName(param0->unk_6A0, 1, param0->unk_5A4->unk_26);
        return PartyMenu_ItemUseFunc_TMHMAskStopTryingToLearn(param0);
    }

    StringTemplate_SetMoveName(param0->unk_6A0, 1, Pokemon_GetValue(v0, 54 + param0->unk_5A4->unk_28, NULL));
    v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 60);
    StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
    Strbuf_Free(v1);
    sub_02082708(param0, 0xffffffff, 1);

    param0->unk_B0E = 22;
    return PARTY_MENU_STATE_WAIT_TEXT_PRINTER;
}

int PartyMenu_Subtask_TMHMLearnMove(GameWindowLayout *param0)
{
    Pokemon *v0;
    Strbuf *v1;

    if (gCoreSys.pressedKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        v0 = Party_GetPokemonBySlotIndex(param0->unk_5A4->unk_00, param0->unk_B11);

        PartyMenu_LearnMoveToSlot(param0, v0, param0->unk_5A4->unk_28);
        v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 61);
        StringTemplate_SetMoveName(param0->unk_6A0, 1, param0->unk_5A4->unk_26);
        StringTemplate_Format(param0->unk_6A0, param0->unk_6A4, v1);
        Strbuf_Free(v1);
        sub_02082708(param0, 0xffffffff, 0);

        param0->unk_5A4->unk_23 = 0;
        param0->unk_B0E = 25;

        return PARTY_MENU_STATE_WAIT_TEXT_PRINTER;
    }

    return 22;
}

static int PartyMenu_ItemUseFunc_TMHMPromptForgetMove(void *param0)
{
    GameWindowLayout *v0;
    Strbuf *v1;

    v0 = param0;
    v1 = MessageLoader_GetNewStrbuf(v0->unk_69C, 59);

    StringTemplate_Format(v0->unk_6A0, v0->unk_6A4, v1);
    Strbuf_Free(v1);
    sub_02082708(v0, 0xffffffff, 0);

    v0->unk_5A4->unk_23 = 4;
    v0->unk_B0E = 25;

    return PARTY_MENU_STATE_WAIT_TEXT_PRINTER;
}

static int PartyMenu_ItemUseFunc_TMHMAskStopTryingToLearn(void *param0)
{
    GameWindowLayout *v0;
    Strbuf *v1;

    v0 = param0;
    v1 = MessageLoader_GetNewStrbuf(v0->unk_69C, 55);

    StringTemplate_Format(v0->unk_6A0, v0->unk_6A4, v1);
    Strbuf_Free(v1);
    sub_02082708(v0, 0xffffffff, 1);

    v0->unk_B04.unk_00 = PartyMenu_ItemUseFunc_TMHMDidNotLearnMove;
    v0->unk_B04.unk_04 = PartyMenu_ItemUseFunc_TMHMAskAgainToForget;
    v0->unk_B0E = 26;

    return PARTY_MENU_STATE_WAIT_TEXT_PRINTER;
}

static int PartyMenu_ItemUseFunc_TMHMDidNotLearnMove(void *param0)
{
    GameWindowLayout *v0;
    Strbuf *v1;

    v0 = param0;
    v1 = MessageLoader_GetNewStrbuf(v0->unk_69C, 58);

    StringTemplate_Format(v0->unk_6A0, v0->unk_6A4, v1);
    Strbuf_Free(v1);
    sub_02082708(v0, 0xffffffff, 0);

    v0->unk_5A4->unk_23 = PARTY_MENU_ACTION_RETURN_0;
    v0->unk_B0E = nextStateCheck(v0, FALSE);

    return PARTY_MENU_STATE_WAIT_TEXT_PRINTER;
}

static int PartyMenu_ItemUseFunc_TMHMAskAgainToForget(void *param0)
{
    GameWindowLayout *v0;
    Strbuf *v1;

    v0 = param0;
    v1 = MessageLoader_GetNewStrbuf(v0->unk_69C, 52);

    StringTemplate_Format(v0->unk_6A0, v0->unk_6A4, v1);
    Strbuf_Free(v1);
    sub_02082708(v0, 0xffffffff, 0);

    v0->unk_B04.unk_00 = PartyMenu_ItemUseFunc_TMHMPromptForgetMove;
    v0->unk_B04.unk_04 = PartyMenu_ItemUseFunc_TMHMAskStopTryingToLearn;
    v0->unk_B0E = PARTY_MENU_STATE_YES_NO_INIT;

    return PARTY_MENU_STATE_WAIT_TEXT_PRINTER;
}

static void PartyMenu_LearnMoveToSlot(GameWindowLayout *param0, Pokemon *param1, u32 param2)
{
    u32 v0;

    v0 = param0->unk_5A4->unk_26;
    Pokemon_SetValue(param1, MON_DATA_MOVE1 + param2, &v0);

    v0 = 0;
    Pokemon_SetValue(param1, MON_DATA_MOVE1_PP_UPS + param2, &v0);

    v0 = MoveTable_CalcMaxPP(param0->unk_5A4->unk_26, 0);
    Pokemon_SetValue(param1, MON_DATA_MOVE1_CUR_PP + param2, &v0);

    if (param0->unk_5A4->unk_24 != 0) {
        if (Item_IsHMMove(param0->unk_5A4->unk_26) == 0) {
            Bag_TryRemoveItem(param0->unk_5A4->unk_04, param0->unk_5A4->unk_24, 1, 12);
        }

        Pokemon_UpdateFriendship(param1, 4, (u16)PartyMenu_GetCurrentMapSec(param0));
    }
}

static u8 PartyMenu_AddMoveNameToList(GameWindowLayout *param0, u8 param1)
{
    Pokemon *v0;
    Strbuf *v1;
    u16 v2;

    v0 = Party_GetPokemonBySlotIndex(param0->unk_5A4->unk_00, param0->unk_B11);
    v2 = (u16)Pokemon_GetValue(v0, MON_DATA_MOVE1 + param1, NULL);
    v1 = MessageLoader_GetNewStrbuf(param0->unk_69C, 162 + param1);

    StringTemplate_SetMoveName(param0->unk_6A0, 0, v2);
    StringTemplate_Format(param0->unk_6A0, param0->unk_6A8, v1);
    Strbuf_Free(v1);

    if (v2 == 0) {
        StringList_AddFromStrbuf(param0->unk_6FC, param0->unk_6A8, 0xfffffffd);
        return FALSE;
    }

    StringList_AddFromStrbuf(param0->unk_6FC, param0->unk_6A8, param1);
    return TRUE;
}

void PartyMenu_SelectMoveForPpRestoreOrPpUp(GameWindowLayout *param0, u8 param1)
{
    MenuTemplate v0;
    u8 v1;

    if (param1 == 0) {
        sub_020826F4(param0, 41, 1);
    } else {
        sub_020826F4(param0, 40, 1);
    }

    param0->unk_6FC = StringList_New(4, 12);

    v1 = PartyMenu_AddMoveNameToList(param0, 0);
    v1 += PartyMenu_AddMoveNameToList(param0, 1);
    v1 += PartyMenu_AddMoveNameToList(param0, 2);
    v1 += PartyMenu_AddMoveNameToList(param0, 3);

    v0.choices = param0->unk_6FC;
    v0.window = &param0->unk_04[36];
    v0.fontID = FONT_SYSTEM;
    v0.xSize = 1;
    v0.ySize = 4;
    v0.lineSpacing = 0;
    v0.suppressCursor = FALSE;

    if (v1 == 4) {
        v0.loopAround = TRUE;
    } else {
        v0.loopAround = FALSE;
    }

    Window_DrawStandardFrame(&param0->unk_04[36], 1, 1, 14);
    param0->unk_700 = Menu_NewAndCopyToVRAM(&v0, 8, 0, 0, 12, PAD_BUTTON_B);
}

int PartyMenu_Subtask_SelectMove(GameWindowLayout *param0)
{
    u32 v0 = Menu_ProcessInput(param0->unk_700);

    switch (v0) {
    case 0xffffffff:
        break;
    case 0xfffffffe:
        Window_EraseMessageBox(&param0->unk_04[33], 1);
        Window_EraseStandardFrame(&param0->unk_04[36], 1);
        Menu_Free(param0->unk_700, NULL);
        StringList_Free(param0->unk_6FC);
        sub_020826E0(param0, 32, 1);
        return PARTY_MENU_STATE_USE_ITEM_SELECT_MON;
    default:
        Window_EraseMessageBox(&param0->unk_04[33], 1);
        Window_EraseStandardFrame(&param0->unk_04[36], 1);
        Menu_Free(param0->unk_700, NULL);
        StringList_Free(param0->unk_6FC);

        if (sub_02096F14(param0->unk_5A4->unk_00, param0->unk_5A4->unk_24, param0->unk_B11, (u8)v0, PartyMenu_GetCurrentMapSec(param0), 12) == 1) {
            Pokemon *v1 = Party_GetPokemonBySlotIndex(param0->unk_5A4->unk_00, param0->unk_B11);
            sub_02084E58(param0, param0->unk_5A4->unk_24, Pokemon_GetValue(v1, MON_DATA_MOVE1 + v0, NULL));
            Bag_TryRemoveItem(param0->unk_5A4->unk_04, param0->unk_5A4->unk_24, 1, 12);
            Sound_PlayEffect(1516);
        } else {
            MessageLoader_GetStrbuf(param0->unk_69C, 105, param0->unk_6A4);
        }

        sub_02082708(param0, 0xffffffff, 1);

        param0->unk_5A4->unk_23 = 0;
        param0->unk_B0E = 25;

        return PARTY_MENU_STATE_WAIT_TEXT_PRINTER;
    }

    return PARTY_MENU_STATE_SELECT_MOVE;
}

void PartyMenu_HandleAttachMailFromMailbox(GameWindowLayout *param0)
{
    Window_EraseMessageBox(&param0->unk_04[32], 1);

    if (param0->unk_704[param0->unk_B11].unk_0C == 0) {
        MessageLoader_GetStrbuf(param0->unk_69C, 127, param0->unk_6A4);
        sub_02083080(param0, param0->unk_B11);
    } else {
        MessageLoader_GetStrbuf(param0->unk_69C, 128, param0->unk_6A4);
        param0->unk_B11 = 7;
    }

    sub_02082708(param0, 0xffffffff, 1);

    param0->unk_5A4->unk_23 = 0;
    param0->unk_B0E = PARTY_MENU_STATE_AFTER_MESSAGE_BEGIN_EXIT;
}

static u16 PartyMenu_GetCurrentMapSec(GameWindowLayout *param0)
{
    FieldSystem *fieldSystem = param0->unk_5A4->unk_1C;
    return (u16)MapHeader_GetMapLabelTextID(fieldSystem->location->mapId);
}
