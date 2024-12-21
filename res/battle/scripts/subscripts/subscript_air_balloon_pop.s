#include "macros/btlcmd.inc"

    .data

_000:
    // {0}’s {1} popped!
    PrintMessage pl_msg_00000368_01270, TAG_NICKNAME_ITEM, BTLSCR_DEFENDER, BTLSCR_DEFENDER
    Wait 
    WaitButtonABTime 30
    RemoveItem BTLSCR_DEFENDER
    End 