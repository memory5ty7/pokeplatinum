#include "applications/pokemon_summary_screen/subscreen.h"

#include <nitro.h>
#include <string.h>

#include "consts/sdat.h"

#include "applications/pokemon_summary_screen/main.h"
#include "applications/pokemon_summary_screen/sprite.h"

#include "bg_window.h"
#include "core_sys.h"
#include "touch_screen.h"
#include "unk_02005474.h"

typedef struct PSSSubscreenButton {
    u8 page;
    u8 paletteNum;
    u8 x;
    u8 y;
} PSSSubscreenButton;

static void sub_0223D0BC(PokemonSummaryScreen *summaryScreen);
static int CheckSubscreenRectanglePress(PokemonSummaryScreen *summaryScreen);

static const PSSSubscreenButton sSubscreenButtons_Normal[] = {
    { PSS_PAGE_INFO, 1, 1, 4 },
    { PSS_PAGE_MEMO, 1, 2, 10 },
    { PSS_PAGE_SKILLS, 2, 5, 15 },
    { PSS_PAGE_BATTLE_MOVES, 3, 10, 18 },
    { PSS_PAGE_CONDITION, 2, 17, 18 },
    { PSS_PAGE_CONTEST_MOVES, 3, 22, 15 },
    { PSS_PAGE_RIBBONS, 4, 25, 10 },
    { PSS_PAGE_EXIT, 4, 26, 4 },
    { PSS_PAGE_NONE, -1, -1, -1 }
};

static const PSSSubscreenButton sSubscreenButtons_NoContestInfo[] = {
    { PSS_PAGE_INFO, 1, 2, 9 },
    { PSS_PAGE_MEMO, 1, 6, 15 },
    { PSS_PAGE_SKILLS, 2, 14, 18 },
    { PSS_PAGE_BATTLE_MOVES, 3, 22, 15 },
    { PSS_PAGE_EXIT, 4, 26, 9 },
    { PSS_PAGE_NONE, -1, -1, -1 }
};

static const PSSSubscreenButton sSubscreenButtons_UnusedMovesOnly[] = {
    { PSS_PAGE_BATTLE_MOVES, 3, 10, 18 },
    { PSS_PAGE_CONTEST_MOVES, 3, 17, 18 },
    { PSS_PAGE_NONE, -1, -1, -1 }
};

static const PSSSubscreenButton *sSubscreenButtonTypes[] = {
    [PSS_SUBSCREEN_TYPE_NORMAL] = sSubscreenButtons_Normal,
    [PSS_SUBSCREEN_TYPE_NO_CONTEST_INFO] = sSubscreenButtons_NoContestInfo,
    [PSS_SUBSCREEN_TYPE_UNUSED_MOVES_ONLY] = sSubscreenButtons_UnusedMovesOnly
};

static const TouchScreenRect sSubscreenRectangles_Normal[] = {
    { 32, 71, 8, 47 },
    { 80, 119, 16, 55 },
    { 120, 159, 40, 79 },
    { 144, 183, 80, 119 },
    { 144, 183, 136, 175 },
    { 120, 159, 176, 215 },
    { 80, 119, 200, 239 },
    { 32, 71, 208, 247 },
    { -1, 0, 0, 0 }
};

static const TouchScreenRect sSubscreenRectangles_NoContestInfo[] = {
    { 72, 111, 12, 51 },
    { 120, 159, 44, 83 },
    { 144, 183, 108, 147 },
    { 120, 159, 172, 211 },
    { 72, 111, 204, 243 },
    { -1, 0, 0, 0 }
};

static const TouchScreenRect sSubscreenRectangles_UnusedMovesOnly[] = {
    { 144, 183, 80, 119 },
    { 144, 183, 136, 175 },
    { -1, 0, 0, 0 }
};

static const TouchScreenRect *sSubscreenRectangles[] = {
    [PSS_SUBSCREEN_TYPE_NORMAL] = sSubscreenRectangles_Normal,
    [PSS_SUBSCREEN_TYPE_NO_CONTEST_INFO] = sSubscreenRectangles_NoContestInfo,
    [PSS_SUBSCREEN_TYPE_UNUSED_MOVES_ONLY] = sSubscreenRectangles_UnusedMovesOnly
};

void PokemonSummaryScreen_SetSubscreenType(PokemonSummaryScreen *summaryScreen)
{
    switch (summaryScreen->data->mode) {
    case PSS_MODE_NORMAL:
    case PSS_MODE_LOCK_MOVES:
        if (summaryScreen->data->showContest != FALSE) {
            summaryScreen->subscreenType = PSS_SUBSCREEN_TYPE_NORMAL;
        } else {
            summaryScreen->subscreenType = PSS_SUBSCREEN_TYPE_NO_CONTEST_INFO;
            Bg_ScheduleScroll(summaryScreen->bgConfig, BG_LAYER_SUB_0, BG_OFFSET_UPDATE_SET_X, 4);
        }
        break;
    case PSS_MODE_SELECT_MOVE:
    case PSS_MODE_POFFIN:
    default:
        summaryScreen->subscreenType = PSS_SUBSCREEN_TYPE_NO_BUTTONS;
    }

    PokemonSummaryScreen_UpdateSubscreenButtonGfx(summaryScreen);
}

static void UpdateSubscreenButtonTilemap(PokemonSummaryScreen *summaryScreen, const PSSSubscreenButton *button, u8 animID)
{
    u16 *bgSub0Tilemap = (u16 *)Bg_GetTilemapBuffer(summaryScreen->bgConfig, BG_LAYER_SUB_0);
    u16 baseTile = (button->page & 1) * 15 + (button->page / 2) * (30 * 5) + animID * 5 + 30;
    u8 y, x;

    for (y = 0; y < 5; y++) {
        for (x = 0; x < 5; x++) {
            bgSub0Tilemap[(y + button->y) * 32 + x + button->x] = baseTile + y * 30 + x + (button->paletteNum << 12);
        }
    }
}

void PokemonSummaryScreen_UpdateSubscreenButtonGfx(PokemonSummaryScreen *summaryScreen)
{
    if (summaryScreen->subscreenType == PSS_SUBSCREEN_TYPE_NO_BUTTONS) {
        return;
    }

    const PSSSubscreenButton *buttonList = sSubscreenButtonTypes[summaryScreen->subscreenType];
    u8 button = 0;

    while (TRUE) {
        if (buttonList[button].page == PSS_PAGE_NONE) {
            break;
        }

        UpdateSubscreenButtonTilemap(summaryScreen, &buttonList[button], 0);
        button++;
    }

    Bg_ScheduleTilemapTransfer(summaryScreen->bgConfig, BG_LAYER_SUB_0);
}

u8 PokemonSummaryScreen_CheckSubscreenButtonPress(PokemonSummaryScreen *summaryScreen)
{
    int button = CheckSubscreenRectanglePress(summaryScreen);

    if (button != TOUCHSCREEN_INPUT_NONE) {
        return button;
    }

    return PSS_SUBSCREEN_BUTTON_NONE;
}

static int CheckSubscreenRectanglePress(PokemonSummaryScreen *summaryScreen)
{
    if (summaryScreen->subscreenType == PSS_SUBSCREEN_TYPE_NO_BUTTONS) {
        return TOUCHSCREEN_INPUT_NONE;
    }

    return sub_02022664(sSubscreenRectangles[summaryScreen->subscreenType]);
}

static int sub_02092224(PokemonSummaryScreen *summaryScreen)
{
    int v0;

    if (summaryScreen->subscreenType == PSS_SUBSCREEN_TYPE_NO_BUTTONS) {
        return TOUCHSCREEN_INPUT_NONE;
    }

    v0 = sub_02022644(sSubscreenRectangles[summaryScreen->subscreenType]);

    if (v0 != TOUCHSCREEN_INPUT_NONE) {
        u16 v1 = 0xfffe;

        if (Bg_DoesPixelAtXYMatchVal(summaryScreen->bgConfig, BG_LAYER_SUB_0, gCoreSys.touchX, gCoreSys.touchY, &v1) == 0) {
            return TOUCHSCREEN_INPUT_NONE;
        }
    }

    return v0;
}

u8 PokemonSummaryScreen_DrawSubscreenButtonAnim(PokemonSummaryScreen *summaryScreen)
{
    const PSSSubscreenButton *buttonList = sSubscreenButtonTypes[summaryScreen->subscreenType];

    switch (summaryScreen->buttonState) {
    case 0:
        Sound_PlayEffect(SEQ_SE_DP_BUTTON9);
        PokemonSummaryScreen_UpdateSubscreenButtonGfx(summaryScreen);
        UpdateSubscreenButtonTilemap(summaryScreen, &buttonList[summaryScreen->buttonCurrent], 2);
        Bg_ScheduleTilemapTransfer(summaryScreen->bgConfig, BG_LAYER_SUB_0);
        sub_0208F600(summaryScreen);
        summaryScreen->buttonCount = 0;
        summaryScreen->buttonState = 1;
        break;

    case 1:
        summaryScreen->buttonState = 2;
        break;

    case 2:
        if (summaryScreen->buttonCount != 3) {
            summaryScreen->buttonCount++;
            break;
        }

        sub_0208F684(summaryScreen);

        if (sub_02092224(summaryScreen) != summaryScreen->buttonCurrent) {
            if (summaryScreen->monData.isEgg != FALSE
                && buttonList[summaryScreen->buttonCurrent].page != PSS_PAGE_MEMO
                && buttonList[summaryScreen->buttonCurrent].page != PSS_PAGE_EXIT) {
                UpdateSubscreenButtonTilemap(summaryScreen, &buttonList[summaryScreen->buttonCurrent], 0);
            } else {
                UpdateSubscreenButtonTilemap(summaryScreen, &buttonList[summaryScreen->buttonCurrent], 1);
            }

            Bg_ScheduleTilemapTransfer(summaryScreen->bgConfig, BG_LAYER_SUB_0);
            return TRUE;
        }
    }

    return FALSE;
}

void PokemonSummaryScreen_CalcSubscreenButtonCirclePos(PokemonSummaryScreen *summaryScreen, s16 *outX, s16 *outY)
{
    const PSSSubscreenButton *buttonList = sSubscreenButtonTypes[summaryScreen->subscreenType];

    *outX = buttonList[summaryScreen->buttonCurrent].x * 8 + (5 * 8) / 2;
    *outY = buttonList[summaryScreen->buttonCurrent].y * 8 + (5 * 8) / 2;
}

u8 PokemonSummaryScreen_GetSubscreenButtonPage(PokemonSummaryScreen *summaryScreen, u8 button)
{
    const PSSSubscreenButton *buttonList = sSubscreenButtonTypes[summaryScreen->subscreenType];
    return buttonList[button].page;
}
