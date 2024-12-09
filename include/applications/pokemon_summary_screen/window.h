#ifndef POKEPLATINUM_POKEMON_SUMMARY_SCREEN_WINDOW_H
#define POKEPLATINUM_POKEMON_SUMMARY_SCREEN_WINDOW_H

#include "applications/pokemon_summary_screen/main.h"

enum SummaryPoffinFeedMsg {
    SUMMARY_MSG_COOLNESS_ENHANCED = 0,
    SUMMARY_MSG_BEAUTY_ENHANCED,
    SUMMARY_MSG_CUTENESS_ENHANCED,
    SUMMARY_MSG_SMARTNESS_ENHANCED,
    SUMMARY_MSG_TOUGHNESS_ENHANCED,

    SUMMARY_MSG_NOTHING_CHANGED = 0xFE,
    SUMMARY_MSG_MON_WONT_EAT_MORE = 0xFF,
};

void PokemonSummaryScreen_DrawStaticWindows(PokemonSummaryScreen *summaryScreen);
void PokemonSummaryScreen_AddExtraWindows(PokemonSummaryScreen *summaryScreen);
void PokemonSummaryScreen_RemoveExtraWindows(PokemonSummaryScreen *summaryScreen);
void PokemonSummaryScreen_RemoveWindows(PokemonSummaryScreen *summaryScreen);
void PokemonSummaryScreen_PrintNicknameAndGender(PokemonSummaryScreen *summaryScreen);
void PokemonSummaryScreen_PrintLevel(PokemonSummaryScreen *summaryScreen);
void PokemonSummaryScreen_PrintItemName(PokemonSummaryScreen *summaryScreen);
void PokemonSummaryScreen_ClearAndPrintButtonPrompt(PokemonSummaryScreen *summaryScreen, u32 entryID);
void PokemonSummaryScreen_DrawExtraWindows(PokemonSummaryScreen *summaryScreen);
void PokemonSummaryScreen_PrintRibbonIndexAndMax(PokemonSummaryScreen *summaryScreen);
void PokemonSummaryScreen_PrintRibbonNameAndDesc(PokemonSummaryScreen *summaryScreen);
void PokemonSummaryScreen_PrintBattleMoveAttributes(PokemonSummaryScreen *summaryScreen, u32 move);
void PokemonSummaryScreen_ClearBattleAttributeWindows(PokemonSummaryScreen *summaryScreen);
void PokemonSummaryScreen_ShowMove5OrCancel(PokemonSummaryScreen *summaryScreen);
void PokemonSummaryScreen_HideMoveCancelText(PokemonSummaryScreen *summaryScreen);
void PokemonSummaryScreen_SwapMoveNameAndPP(PokemonSummaryScreen *summaryScreen);
void PokemonSummaryScreen_PrintHMMovesCantBeForgotten(PokemonSummaryScreen *summaryScreen);
void PokemonSummaryScreen_PrintContestMoveAttributes(PokemonSummaryScreen *summaryScreen, u32 move);
void PokemonSummaryScreen_ClearContestAttributeWindows(PokemonSummaryScreen *summaryScreen);
void PokemonSummaryScreen_PrintPoffinFeedMsg(PokemonSummaryScreen *summaryScreen, enum SummaryPoffinFeedMsg msg);

#endif // POKEPLATINUM_POKEMON_SUMMARY_SCREEN_WINDOW_H
