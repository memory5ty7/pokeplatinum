#include "macros/btlcmd.inc"
#include "macros/btlcmd.inc"

    .data

_000:
    UpdateVar OPCODE_SET, BTLVAR_SIDE_EFFECT_FLAGS_INDIRECT, MOVE_SIDE_EFFECT_TO_DEFENDER|MOVE_SUBSCRIPT_PTR_HEAL_BLOCK_START
    CalcCrit 
    CalcDamage 
    End 