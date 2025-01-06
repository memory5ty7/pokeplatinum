#include "macros/btlcmd.inc"

    .data

_000:
    RemoveItem BTLSCR_ATTACKER
    PrintMessage pl_msg_00000368_01272, TAG_NONE
    Wait 
    WaitButtonABTime 30
    End 
