#include "macros/btlanimcmd.inc"

.data

L_0:
    LoadParticleResource 5, luster_purge_spa
    PlaySoundEffectL SEQ_SE_DP_W257
    Delay 10
    Func_FadeBg FADE_BG_TYPE_BASE, 1, 0, 16, BATTLE_COLOR_WHITE
    Func_FadeBattlerSprite BATTLE_ANIM_BATTLER_PLAYER_1, 0, 1, BATTLE_COLOR_WHITE, 16, 30
    Func_FadeBattlerSprite BATTLE_ANIM_BATTLER_ENEMY_1, 0, 1, BATTLE_COLOR_WHITE, 16, 30
    Func_FadeBattlerSprite BATTLE_ANIM_BATTLER_PLAYER_2, 0, 1, BATTLE_COLOR_WHITE, 16, 30
    Func_FadeBattlerSprite BATTLE_ANIM_BATTLER_ENEMY_2, 0, 1, BATTLE_COLOR_WHITE, 16, 30
    Delay 20
    PlaySoundEffectL SEQ_SE_DP_W053
    Delay 20
    Func_FadeBg FADE_BG_TYPE_BASE, 1, 16, 0, BATTLE_COLOR_WHITE
    WaitForAnimTasks
    UnloadParticleSystem 5

    InitPokemonSpriteManager
    LoadPokemonSpriteDummyResources 0
    Func_HideBattler BATTLE_ANIM_ATTACKER, TRUE
    AddPokemonSprite BATTLER_ROLE_ATTACKER, FALSE, BATTLE_ANIM_MON_SPRITE_0, 0
    LoadPokemonSpriteDummyResources 4
    AddPokemonSprite BATTLER_ROLE_ATTACKER_PARTNER, FALSE, BATTLE_ANIM_MON_SPRITE_4, 4
    BtlAnimCmd_082 2, 0, 4
    WaitForAnimTasks
    PlaySoundEffectL SEQ_SE_DP_W100
    Func_PixelatePokemonSprite BATTLE_ANIM_MON_SPRITE_0, 1, 0, 0
    WaitForAnimTasks
    RemovePokemonSprite BATTLE_ANIM_MON_SPRITE_0
    BtlAnimCmd_068 0
    LoadPokemonSpriteDummyResources 1
    AddPokemonSprite BATTLER_ROLE_ATTACKER, FALSE, BATTLE_ANIM_MON_SPRITE_1, 1
    Func_PixelatePokemonSprite BATTLE_ANIM_MON_SPRITE_1, -1, 15, 15
    Delay 12
    Func_HideBattler BATTLE_ANIM_ATTACKER, FALSE
    Delay 1
    FreePokemonSpriteManager
    RemovePokemonSprite BATTLE_ANIM_MON_SPRITE_0
    RemovePokemonSprite BATTLE_ANIM_MON_SPRITE_1
    BtlAnimCmd_083 0
    RemovePokemonSprite BATTLE_ANIM_MON_SPRITE_4
    End
 

