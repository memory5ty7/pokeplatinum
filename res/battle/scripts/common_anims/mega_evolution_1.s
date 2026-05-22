#include "macros/btlanimcmd.inc"

.data

L_0:  
    Delay 20
    PlaySoundEffectR SEQ_SE_DP_W053
    Func_FadeBg FADE_BG_TYPE_BASE, 1, 16, 0, BATTLE_COLOR_WHITE
    WaitForAnimTasks
    StopSoundEffect SEQ_SE_DP_W053
    PlayPokemonCry 0, -117, 100
    WaitForPokemonCries 0
    End
