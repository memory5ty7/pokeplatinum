#include "macros/btlcmd.inc"

    .data

_000:
    PrintAttackMessage 
    Wait 
    WaitButtonABTime 15
    // {0}’s {1} made {2} useless!
    PrintMessage pl_msg_00000368_00638, TAG_NICKNAME_ABILITY_MOVE, BTLSCR_DEFENDER, BTLSCR_DEFENDER, BTLSCR_ATTACKER
    Wait 
    WaitButtonABTime 30
    End 

