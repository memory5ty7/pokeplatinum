#include "macros/btlcmd.inc"

    .data

_000:
    UpdateMonData OPCODE_BITWISE_XOR, BTLSCR_MSG_BATTLER_TEMP, BATTLEMON_MOVE_EFFECTS_MASK, MOVE_EFFECT_POWER_TRICK
    UpdateMonDataFromVar OPCODE_GET, BTLSCR_MSG_BATTLER_TEMP, BATTLEMON_ATTACK, BTLVAR_CALC_TEMP
    UpdateMonDataFromVar OPCODE_GET, BTLSCR_MSG_BATTLER_TEMP, BATTLEMON_DEFENSE, BTLVAR_SCRIPT_TEMP
    UpdateMonDataFromVar OPCODE_SET, BTLSCR_MSG_BATTLER_TEMP, BATTLEMON_ATTACK, BTLVAR_SCRIPT_TEMP
    UpdateMonDataFromVar OPCODE_SET, BTLSCR_MSG_BATTLER_TEMP, BATTLEMON_DEFENSE, BTLVAR_CALC_TEMP

    UpdateMonDataFromVar OPCODE_GET, BTLSCR_MSG_BATTLER_TEMP, BATTLEMON_SP_ATTACK, BTLVAR_CALC_TEMP
    UpdateMonDataFromVar OPCODE_GET, BTLSCR_MSG_BATTLER_TEMP, BATTLEMON_SP_DEFENSE, BTLVAR_SCRIPT_TEMP
    UpdateMonDataFromVar OPCODE_SET, BTLSCR_MSG_BATTLER_TEMP, BATTLEMON_SP_ATTACK, BTLVAR_SCRIPT_TEMP
    UpdateMonDataFromVar OPCODE_SET, BTLSCR_MSG_BATTLER_TEMP, BATTLEMON_SP_DEFENSE, BTLVAR_CALC_TEMP

    // {0} switched its Attack and Defense!
    PrintMessage pl_msg_00000368_01009, TAG_NICKNAME, BTLSCR_MSG_BATTLER_TEMP
    Wait 
    WaitButtonABTime 30
    End 
