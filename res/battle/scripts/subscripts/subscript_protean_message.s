#include "macros/btlcmd.inc"

    .data

_000:
    // {0}’s {1} made it the {2} type!
    PrintMessage pl_msg_00000368_00641, TAG_NICKNAME_ABILITY_TYPE, BTLSCR_ATTACKER, BTLSCR_ATTACKER, BTLSCR_MSG_TEMP
    Wait 
    WaitButtonABTime 30
    End 