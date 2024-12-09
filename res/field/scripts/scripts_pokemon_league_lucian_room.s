#include "macros/scrcmd.inc"

    .data

    ScriptEntry _000A
    ScriptEntry _00B7
    .short 0xFD13

_000A:
    PlayFanfare SEQ_SE_CONFIRM
    LockAll
    FacePlayer
    GoToIfSet 179, _00AC
    ScrCmd_0EA 0x108
    Message 0
    CloseMessage
    CallIfUnset 214, _007A
    CallIfSet 214, _0082
    CheckWonBattle 0x800C
    GoToIfEq 0x800C, 0, _00A6
    SetFlag 179
    PlayFanfare SEQ_SE_DP_KI_GASYAN
    ScrCmd_065 1
    CallIfUnset 214, _008A
    CallIfSet 214, _0098
    Message 1
    WaitABXPadPress
    CloseMessage
    ReleaseAll
    End

_007A:
    StartTrainerBattle trainer_elite_four_lucian
    Return

_0082:
    StartTrainerBattle trainer_elite_four_lucian_rematch
    Return

_008A:
    ScrCmd_1CD 11, trainer_elite_four_lucian, 0, 0, 0
    Return

_0098:
    ScrCmd_1CD 11, trainer_elite_four_lucian_rematch, 0, 0, 0
    Return

_00A6:
    ScrCmd_0EB
    ReleaseAll
    End

_00AC:
    Message 2
    WaitABXPadPress
    CloseMessage
    ReleaseAll
    End

_00B7:
    LockAll
    ApplyMovement 0xFF, _00DC
    WaitMovement
    PlayFanfare SEQ_SE_DP_KI_GASYAN
    ClearFlag 0x285
    ScrCmd_064 2
    SetVar 0x4001, 1
    ReleaseAll
    End

    .balign 4, 0
_00DC:
    MoveAction_00C 2
    EndMovement
