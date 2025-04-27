#include "macros/btlcmd.inc"

    .data

_000:
    // {0}’s {1} makes the opposing team too nervous to eat Berries!
    PrintMessage pl_msg_00000368_01282, TAG_ABILITY_NICKNAME, BTLSCR_MSG_BATTLER_TEMP, BTLSCR_MSG_TEMP
    Wait 
    WaitButtonABTime 30
    End 