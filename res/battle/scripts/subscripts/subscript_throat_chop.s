#include "macros/btlcmd.inc"

    .data

_000:
    CompareMonDataToValue OPCODE_NEQ, BTLSCR_DEFENDER, BATTLEMON_THROAT_CHOP_TURNS, 0, _end
    UpdateMonData OPCODE_SET, BTLSCR_DEFENDER, BATTLEMON_THROAT_CHOP_TURNS, 2
    Call BATTLE_SUBSCRIPT_ATTACK_MESSAGE_AND_ANIMATION
    // {0} received an encore!
    PrintMessage pl_msg_00000368_00372, TAG_NICKNAME, BTLSCR_DEFENDER
    Wait 
    WaitButtonABTime 30

_end:
    End 
