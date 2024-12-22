#include "macros/btlcmd.inc"

    .data

_000:
    CompareMonDataToValue OPCODE_NEQ, BTLSCR_DEFENDER, BATTLEMON_STATUS, MON_CONDITION_NONE, _011
    UpdateVar OPCODE_SET, BTLVAR_POWER_MULTI, 10
    GoTo _015

_011:
    UpdateVar OPCODE_SET, BTLVAR_POWER_MULTI, 20

_015:
    CalcCrit 
    CalcDamage 
    End