#include "macros/btlcmd.inc"

    .data

_000:
    PlayBattleAnimation BTLSCR_ATTACKER, BATTLE_ANIMATION_HELD_ITEM
    Wait 
    PrintMessage pl_msg_00000368_00597, TAG_NICKNAME_ITEM, BTLSCR_ATTACKER, BTLSCR_ATTACKER
    Wait 
    WaitButtonABTime 30
    RemoveItem BTLSCR_ATTACKER
    End 
