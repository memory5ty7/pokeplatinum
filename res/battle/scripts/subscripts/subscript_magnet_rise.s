#include "macros/btlcmd.inc"

    .data

_000:
    UpdateMonData OPCODE_FLAG_ON, BTLSCR_MSG_BATTLER_TEMP, BATTLEMON_MOVE_EFFECTS_MASK, MOVE_EFFECT_MAGNET_RISE
    UpdateMonData OPCODE_SET, BTLSCR_MSG_BATTLER_TEMP, BATTLEMON_MAGNET_RISE_TURNS, 6
    PrintMessage pl_msg_00000368_01033, TAG_NICKNAME, BTLSCR_MSG_BATTLER_TEMP
    Wait 
    WaitButtonABTime 30
    End 