#include "macros/btlcmd.inc"

    .data

_000:
    UpdateVarFromVar OPCODE_SET, BTLVAR_DEFENDER, BTLVAR_MSG_BATTLER_TEMP
    // {0} floats in the air with its {1}!
    PrintMessage pl_msg_00000368_00875, TAG_NICKNAME_ITEM, BTLSCR_DEFENDER, BTLSCR_DEFENDER
    Wait 
    WaitButtonABTime 30
    End 