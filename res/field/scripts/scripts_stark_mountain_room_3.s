#include "macros/scrcmd.inc"

    .data

    ScriptEntry _0016
    ScriptEntry _0089
    ScriptEntry _00D0
    ScriptEntry _00E6
    ScriptEntry _019C
    .short 0xFD13

_0016:
    SetVar 0x4000, 0x409E
    CallIfGe 0x409E, 1, _0031
    Call _0037
    End

_0031:
    SetFlag 0x1DB
    Return

_0037:
    GoToIfSet 0x120, _0083
    CheckGameCompleted 0x4000
    GoToIfEq 0x4000, 0, _0083
    ScrCmd_22D 2, 0x4000
    GoToIfEq 0x4000, 0, _0083
    GoToIfUnset 0x125, _0083
    GoToIfNe 0x409E, 1, _0083
    ClearFlag 0x1DD
    Return

_0083:
    SetFlag 0x1DD
    Return

_0089:
    GoToIfSet 142, _0096
    End

_0096:
    SetFlag 0x1DD
    ScrCmd_065 1
    ClearFlag 142
    End

    .byte 14
    .byte 0
    .byte 1
    .byte 0
    .byte 35
    .byte 0
    .byte 1
    .byte 0
    .byte 33
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0

    .balign 4, 0
_00B4:
    MoveAction_032
    EndMovement

    .byte 32
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 63
    .byte 0
    .byte 1
    .byte 0
    .byte 13
    .byte 0
    .byte 9
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0

_00D0:
    BufferPlayerName 1
    PlayFanfare SEQ_SE_CONFIRM
    LockAll
    FacePlayer
    Message 18
    WaitABXPadPress
    CloseMessage
    ReleaseAll
    End

_00E6:
    LockAll
    PlayFanfare SEQ_SE_CONFIRM
    WaitFanfare SEQ_SE_CONFIRM
    CallIfUnset 215, _0174
    SetVar 0x409E, 2
    ScrCmd_04C 0x1E5, 0
    Message 15
    CloseMessage
    SetFlag 142
    StartLegendaryBattle SPECIES_HEATRAN, 50
    ClearFlag 142
    CheckWonBattle 0x800C
    GoToIfEq 0x800C, 0, _0168
    CheckLostBattle 0x800C
    CallIfEq 0x800C, 0, _017A
    CheckDidNotCapture 0x800C
    GoToIfEq 0x800C, 1, _015D
    GoTo _0155
    End

_0155:
    SetFlag 0x120
    ReleaseAll
    End

_015D:
    Message 16
    WaitABXPadPress
    CloseMessage
    ReleaseAll
    End

_0168:
    SetVar 0x409E, 1
    ScrCmd_0EB
    ReleaseAll
    End

_0174:
    SetFlag 215
    Return

_017A:
    SetFlag 0x983
    Return

    .byte 12
    .byte 0
    .byte 6
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 13
    .byte 0
    .byte 6
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 63
    .byte 0
    .byte 4
    .byte 0
    .byte 33
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0

_019C:
    LockAll
    Message 0
    CloseMessage
    WaitFanfare SEQ_SE_CONFIRM
    PlayFanfare SEQ_SE_DP_FW089
    ScrCmd_29F 0
    ScrCmd_04A 0x65C
    ApplyMovement 2, _03A4
    ApplyMovement 0xFF, _0388
    WaitMovement
    ApplyMovement 2, _00B4
    WaitMovement
    Message 1
    CloseMessage
    ApplyMovement 5, _03B0
    WaitMovement
    ScrCmd_065 3
    Message 2
    Message 3
    ScrCmd_04C 0x1C5, 0
    ScrCmd_04D
    CloseMessage
    ClearFlag 0x232
    ScrCmd_064 0
    ApplyMovement 0, _0490
    WaitMovement
    ApplyMovement 5, _03C0
    ApplyMovement 9, _0434
    ApplyMovement 10, _043C
    WaitMovement
    Message 4
    CloseMessage
    WaitTime 15, 0x800C
    Message 5
    CloseMessage
    ScrCmd_065 0
    ScrCmd_065 6
    ClearFlag 0x231
    ScrCmd_064 4
    CallCommonScript 0x807
    Message 6
    CloseMessage
    ApplyMovement 4, _04A8
    ApplyMovement 9, _0408
    ApplyMovement 10, _0418
    WaitMovement
    Message 7
    CloseMessage
    ApplyMovement 9, _0444
    WaitMovement
    Message 8
    ApplyMovement 10, _044C
    WaitMovement
    Message 9
    CloseMessage
    WaitTime 15, 0x800C
    ApplyMovement 9, _0454
    ApplyMovement 10, _045C
    WaitMovement
    Message 10
    CloseMessage
    ApplyMovement 9, _0464
    ApplyMovement 10, _047C
    ApplyMovement 0xFF, _0390
    WaitMovement
    ApplyMovement 5, _03D8
    WaitMovement
    Message 11
    Message 12
    CloseMessage
    ClearFlag 0x230
    ScrCmd_064 7
    ApplyMovement 7, _04C8
    WaitMovement
    ScrCmd_064 8
    ApplyMovement 8, _04FC
    WaitMovement
    Message 13
    CloseMessage
    ApplyMovement 5, _03E0
    ApplyMovement 4, _04B4
    WaitMovement
    ApplyMovement 5, _03EC
    ApplyMovement 7, _04E0
    ApplyMovement 8, _0510
    WaitMovement
    ScrCmd_065 5
    ScrCmd_065 7
    ScrCmd_065 8
    BufferPlayerName 0
    Message 14
    CloseMessage
    SetVar 0x40A0, 2
    SetFlag 0x1DB
    SetFlag 0x231
    SetVar 0x409E, 1
    SetFlag 214
    ClearFlag 0x1A3
    ClearFlag 0x1D9
    ClearFlag 0x1D6
    ClearFlag 0x22B
    ClearFlag 0x22D
    ClearFlag 0x22E
    FadeScreen 6, 1, 0, 0
    WaitFadeScreen
    Warp MAP_HEADER_STARK_MOUNTAIN_OUTSIDE, 0, 0x2EF, 233, 0
    FadeScreen 6, 1, 1, 0
    WaitFadeScreen
    End

    .balign 4, 0
_0388:
    MoveAction_012 3
    EndMovement

    .balign 4, 0
_0390:
    MoveAction_063 2
    MoveAction_035
    MoveAction_063 2
    MoveAction_032
    EndMovement

    .balign 4, 0
_03A4:
    MoveAction_063
    MoveAction_012 2
    EndMovement

    .balign 4, 0
_03B0:
    MoveAction_012 2
    MoveAction_063 2
    MoveAction_033
    EndMovement

    .balign 4, 0
_03C0:
    MoveAction_038
    MoveAction_063
    MoveAction_039
    MoveAction_063
    MoveAction_037
    EndMovement

    .balign 4, 0
_03D8:
    MoveAction_033
    EndMovement

    .balign 4, 0
_03E0:
    MoveAction_063 2
    MoveAction_013 3
    EndMovement

    .balign 4, 0
_03EC:
    MoveAction_013
    MoveAction_015
    MoveAction_013 3
    MoveAction_014
    MoveAction_013
    MoveAction_069
    EndMovement

    .balign 4, 0
_0408:
    MoveAction_063 3
    MoveAction_062
    MoveAction_032
    EndMovement

    .balign 4, 0
_0418:
    MoveAction_063
    MoveAction_033
    MoveAction_063
    MoveAction_034
    MoveAction_062
    MoveAction_032
    EndMovement

    .balign 4, 0
_0434:
    MoveAction_035
    EndMovement

    .balign 4, 0
_043C:
    MoveAction_035
    EndMovement

    .balign 4, 0
_0444:
    MoveAction_035
    EndMovement

    .balign 4, 0
_044C:
    MoveAction_034
    EndMovement

    .balign 4, 0
_0454:
    MoveAction_039 3
    EndMovement

    .balign 4, 0
_045C:
    MoveAction_038 3
    EndMovement

    .balign 4, 0
_0464:
    MoveAction_019 2
    MoveAction_017 5
    MoveAction_018
    MoveAction_017
    MoveAction_069
    EndMovement

    .balign 4, 0
_047C:
    MoveAction_017 5
    MoveAction_018
    MoveAction_017
    MoveAction_069
    EndMovement

    .balign 4, 0
_0490:
    MoveAction_018 2
    MoveAction_016 3
    MoveAction_032
    MoveAction_017 3
    MoveAction_019 2
    EndMovement

    .balign 4, 0
_04A8:
    MoveAction_014 3
    MoveAction_012 3
    EndMovement

    .balign 4, 0
_04B4:
    MoveAction_014
    MoveAction_012
    MoveAction_015
    MoveAction_013 2
    EndMovement

    .balign 4, 0
_04C8:
    MoveAction_012 2
    MoveAction_015
    MoveAction_012 3
    MoveAction_014 2
    MoveAction_032
    EndMovement

    .balign 4, 0
_04E0:
    MoveAction_015
    MoveAction_013
    MoveAction_015
    MoveAction_013 3
    MoveAction_014
    MoveAction_013 2
    EndMovement

    .balign 4, 0
_04FC:
    MoveAction_015
    MoveAction_012 4
    MoveAction_014
    MoveAction_032
    EndMovement

    .balign 4, 0
_0510:
    MoveAction_015
    MoveAction_013 3
    MoveAction_014
    MoveAction_013 2
    MoveAction_069
    EndMovement
