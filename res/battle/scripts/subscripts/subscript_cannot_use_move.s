#include "macros/btlcmd.inc"

    .data

_000:
    UpdateVar OPCODE_FLAG_ON, BTLVAR_MOVE_STATUS_FLAGS, MOVE_STATUS_FAILED
    // Ability popup for defender
    
    // {0} cannot use {1}!
    PrintMessage pl_msg_00000368_01054, TAG_NICKNAME_MOVE, BATTLER_CATEGORY_ATTACKER, BATTLER_CATEGORY_ATTACKER
    Wait 
    WaitButtonABTime 30
    End