#ifndef POKEPLATINUM_UNK_02084B70_H
#define POKEPLATINUM_UNK_02084B70_H

#include "struct_defs/struct_0207F248.h"

#include "pokemon.h"

void PartyMenu_SetItemUseFuncFromBagSelection(GameWindowLayout *param0);
int PartyMenu_ItemUseFunc_WaitTextPrinterThenExit(void *param0);
BOOL ItemId_IsReviveAll(u16 param0);
int PartyMenu_Subtask_SacredAsh(GameWindowLayout *param0);
int PartyMenu_ItemUseFunc_LevelUpDoLearnMove(GameWindowLayout *param0);
u8 PartyMenu_CheckCanLearnTMHMMove(GameWindowLayout *param0, Pokemon *param1);
int PartyMenu_HandleUseTMHMonMon(GameWindowLayout *param0);
int PartyMenu_ItemUseFunc_TMHMDoLearnMove(GameWindowLayout *param0);
int PartyMenu_Subtask_TMHMLearnMove(GameWindowLayout *param0);
void PartyMenu_SelectMoveForPpRestoreOrPpUp(GameWindowLayout *param0, u8 param1);
int PartyMenu_Subtask_SelectMove(GameWindowLayout *param0);
void PartyMenu_HandleAttachMailFromMailbox(GameWindowLayout *param0);

#endif // POKEPLATINUM_UNK_02084B70_H
