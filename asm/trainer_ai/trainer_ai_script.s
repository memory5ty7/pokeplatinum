    .ifndef ASM_BATTLE_SCRIPT_INC
    .set ASM_BATTLE_SCRIPT_INC, 1

#include "constants/battle.h"
#include "constants/moves.h"
#include "constants/items.h"
#include "constants/battle/trainer_ai.h"
#include "generated/abilities.h"
#include "generated/genders.h"
#include "generated/pokemon_types.h"
#include "macros/aicmd.inc"

    .text

    .global gTrainerAITable
gTrainerAITable:

FlagTable:
    LabelDistance Basic_Main,          FlagTable ; AI_FLAG_BASIC
    LabelDistance Terminate,           FlagTable ; AI_FLAG_EVAL_ATTACK
    LabelDistance Terminate,           FlagTable ; AI_FLAG_EXPERT
    LabelDistance Terminate,           FlagTable ; AI_FLAG_SETUP_FIRST_TURN
    LabelDistance Terminate,           FlagTable ; AI_FLAG_RISKY
    LabelDistance Terminate,           FlagTable ; AI_FLAG_DAMAGE_PRIORITY
    LabelDistance Terminate,           FlagTable ; AI_FLAG_BATON_PASS
    LabelDistance Terminate,           FlagTable ; AI_FLAG_TAG_STRATEGY
    LabelDistance Terminate,           FlagTable ; AI_FLAG_CHECK_HP
    LabelDistance Terminate,           FlagTable ; AI_FLAG_WEATHER
    LabelDistance Terminate,           FlagTable ; AI_FLAG_HARRASSMENT
    LabelDistance Golmon_Main,         FlagTable ; AI_FLAG_GOLMON
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_12
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_13
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_14
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_15
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_16
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_17
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_18
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_19
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_20
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_21
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_22
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_23
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_24
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_25
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_26
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_27
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_28
    LabelDistance RoamingPokemon_Main, FlagTable ; AI_FLAG_ROAMING_POKEMON
    LabelDistance Safari_Main,         FlagTable ; AI_FLAG_SAFARI
    LabelDistance CatchTutorial_Main,  FlagTable ; AI_FLAG_CATCH_TUTORIAL

Basic_Main:
    ; Ignore this flag on partner battlers.
    IfTargetIsPartner Basic_Partner

    FlagMoveDamageScore FALSE
    IfLoadedEqualTo AI_NO_COMPARISON_MADE, Basic_Other

    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HALVE_DEFENSE, Basic_HighestDamage
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DOUBLE_POWER_EACH_TURN_LOCK_INTO, Basic_HighestDamage

    GoTo Basic_CheckForImmunity

Basic_CheckForImmunity:

    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, ScoreMinus10
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_MOLD_BREAKER, Basic_NoImmunityAbility
    LoadBattlerAbility AI_BATTLER_DEFENDER

    IfLoadedEqualTo ABILITY_VOLT_ABSORB, Basic_CheckElectricAbsorption
    IfLoadedEqualTo ABILITY_MOTOR_DRIVE, Basic_CheckElectricAbsorption

    IfLoadedEqualTo ABILITY_WATER_ABSORB, Basic_CheckWaterAbsorption
    IfLoadedEqualTo ABILITY_DRY_SKIN, Basic_CheckWaterAbsorption

    IfLoadedEqualTo ABILITY_SAP_SIPPER, Basic_CheckGrassAbsorption

    IfLoadedEqualTo ABILITY_FLASH_FIRE, Basic_CheckFireAbsorption

    IfLoadedEqualTo ABILITY_WONDER_GUARD, Basic_CheckWonderGuard

    LoadTypeFrom LOAD_MOVE_TYPE
    IfTempEqualTo TYPE_GROUND, Basic_CheckGroundAbsorption
    
    GoTo Basic_NoImmunityAbility

Basic_CheckElectricAbsorption:
    LoadTypeFrom LOAD_MOVE_TYPE
    IfTempEqualTo TYPE_ELECTRIC, ScoreMinus12
    GoTo Basic_NoImmunityAbility

Basic_CheckGrassAbsorption:
    LoadTypeFrom LOAD_MOVE_TYPE
    IfTempEqualTo TYPE_GRASS, ScoreMinus12
    GoTo Basic_NoImmunityAbility

Basic_CheckWaterAbsorption:
    LoadTypeFrom LOAD_MOVE_TYPE
    IfTempEqualTo TYPE_WATER, ScoreMinus12
    GoTo Basic_NoImmunityAbility

Basic_CheckFireAbsorption:
    LoadTypeFrom LOAD_MOVE_TYPE
    IfTempEqualTo TYPE_FIRE, ScoreMinus12
    GoTo Basic_NoImmunityAbility

Basic_CheckWonderGuard:
    IfMoveEffectivenessEquals TYPE_MULTI_DOUBLE_DAMAGE, Basic_NoImmunityAbility
    IfMoveEffectivenessEquals TYPE_MULTI_QUADRUPLE_DAMAGE, Basic_NoImmunityAbility
    GoTo ScoreMinus12

Basic_CheckGroundAbsorption:
    IsBattlerGrounded AI_BATTLER_DEFENDER, Basic_NoImmunityAbility
    GoTo ScoreMinus12

Basic_NoImmunityAbility:
    FlagMoveDamageScore FALSE
    IfLoadedEqualTo AI_NO_COMPARISON_MADE, Basic_CheckSoundproof

Basic_CheckSoundproof:
    ; Check for immunity to sound-based moves
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedNotEqualTo ABILITY_SOUNDPROOF, CheckKill
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_MOLD_BREAKER, CheckKill
    IfMoveEqualTo MOVE_BUG_BUZZ, ScoreMinus10
    IfMoveEqualTo MOVE_CHATTER, ScoreMinus10
    IfMoveEqualTo MOVE_BOOMBURST, ScoreMinus10
    GoTo CheckKill

CheckKill:
    IfCurrentMoveKills ROLL_FOR_DAMAGE, Basic_KillRandom
    GoTo Basic_HighestDamage

Basic_KillRandom:
    IfRandomLessThan 205, Basic_KillRandom2
    AddToMoveScore 8
    GoTo Basic_CheckSpeedKill

Basic_KillRandom2:
    AddToMoveScore 6
    GoTo Basic_CheckSpeedKill   

Basic_CheckSpeedKill:
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_BEAST_BOOST, Basic_MoxieBoost
    IfLoadedEqualTo ABILITY_MOXIE, Basic_MoxieBoost
    GoTo Basic_CheckSpeedKill2

Basic_MoxieBoost:
    AddToMoveScore 1
    GoTo Basic_CheckSpeedKill2    

Basic_CheckSpeedKill2:
    IfSpeedCompareEqualTo COMPARE_SPEED_FASTER, Basic_FastKill
    IfSpeedCompareEqualTo COMPARE_SPEED_TIE, Basic_FastKill
    GoTo Basic_SlowKill

Basic_FastKill:
    AddToMoveScore 6
    GoTo Basic_Other

Basic_SlowKill:
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_PRIORITY_1, Basic_FastKill
    AddToMoveScore 3
    GoTo Basic_Other

Basic_HighestDamage:
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_PRIORITY_1, Basic_Priority
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_WHIRLPOOL, Basic_Trapping
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_BIND_HIT, Basic_Trapping
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HALVE_DEFENSE, Basic_Explosion
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DOUBLE_POWER_EACH_TURN_LOCK_INTO, Basic_Rollout
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HIT_IN_3_TURNS, Basic_FutureSight

    IfLoadedEqualTo AI_MOVE_IS_HIGHEST_DAMAGE, Basic_HighestDamageRandom

    GoTo Basic_AdditionalDamage
 
Basic_Priority:
    IfSpeedCompareEqualTo COMPARE_SPEED_FASTER, Basic_End
    IfSpeedCompareEqualTo COMPARE_SPEED_TIE, Basic_End
    IfEnemyKills AI_BATTLER_DEFENDER, USE_MAX_DAMAGE, Basic_Priority1
    GoTo Basic_End

Basic_Priority1:
    AddToMoveScore 11
    GoTo Basic_End
     
Basic_Trapping:
    IfRandomLessThan 205, Basic_Trapping1
    AddToMoveScore 8
    GoTo Basic_End

Basic_Trapping1:
    AddToMoveScore 6
    GoTo Basic_End

Basic_Rollout:
    AddToMoveScore 7
    GoTo Basic_End

Basic_FutureSight:
    IfEnemyKills AI_BATTLER_DEFENDER, USE_MAX_DAMAGE, Basic_FutureSight2
    GoTo Basic_FutureSight3

Basic_FutureSight2:
    IfSpeedCompareNotEqualTo COMPARE_SPEED_FASTER, Basic_FutureSight3
    AddToMoveScore 8
    GoTo Basic_End

Basic_FutureSight3:
    AddToMoveScore 6
    GoTo Basic_End

Basic_Explosion:
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, ScoreMinus20

    CountAlivePartyBattlers AI_BATTLER_ATTACKER
    IfLoadedEqualTo 0, Basic_Explosion4

    IfHPPercentLessThan AI_BATTLER_ATTACKER, 10, Basic_Explosion1
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 33, Basic_Explosion2
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 66, Basic_Explosion3
    IfRandomLessThan 13, ScorePlus7   

    GoTo Basic_ExplosionEnd

Basic_Explosion1:
    AddToMoveScore 10
    GoTo Basic_ExplosionEnd

Basic_Explosion2:
    IfRandomLessThan 76, Basic_ExplosionEnd
    AddToMoveScore 8
    GoTo Basic_ExplosionEnd

Basic_Explosion3:
    IfRandomLessThan 128, Basic_ExplosionEnd
    AddToMoveScore 7
    GoTo Basic_ExplosionEnd

Basic_Explosion4:
    CountAlivePartyBattlers AI_BATTLER_DEFENDER
    IfLoadedGreaterThan 0, ScoreMinus20
    AddToMoveScore -1
    GoTo Basic_ExplosionEnd

Basic_ExplosionEnd:
    GoTo Basic_AdditionalDamage

Basic_HighestDamageRandom:
    IfRandomLessThan 205, Basic_HighestDamageRandom1
    AddToMoveScore 8
    GoTo Basic_AdditionalDamage

Basic_HighestDamageRandom1:
    AddToMoveScore 6
    GoTo Basic_AdditionalDamage

Basic_AdditionalDamage:
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HIGH_CRITICAL, Basic_Crit
    LoadBattlerPreviousMove AI_BATTLER_ATTACKER
    LoadEffectOfLoadedMove 
    IfLoadedEqualTo BATTLE_EFFECT_HIT_FIRST_IF_TARGET_ATTACKING, Basic_SuckerPunch
    IfMoveEqualTo MOVE_PURSUIT, Basic_Pursuit
    IfMoveEqualTo MOVE_FAKE_OUT, Basic_FakeOut

    ; Not Highest Damage Additional Bonuses
    IfLoadedNotEqualTo AI_MOVE_IS_HIGHEST_DAMAGE, Basic_NotHighestAdditional

    GoTo Basic_Other

Basic_Crit:
    IfMoveEffectivenessEquals TYPE_MULTI_DOUBLE_DAMAGE, Basic_CritSuperEffective
    IfMoveEffectivenessEquals TYPE_MULTI_QUADRUPLE_DAMAGE, Basic_CritSuperEffective
    GoTo Basic_Other

Basic_CritSuperEffective:
    IfRandomLessThan 128, ScorePlus1
    GoTo Basic_Other

Basic_SuckerPunch:
    IfRandomLessThan 128, ScoreMinus20
    GoTo Basic_Other

Basic_Pursuit:
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Basic_Pursuit2
    AddToMoveScore 3
    GoTo Basic_Pursuit2

Basic_Pursuit2:
    IfCurrentMoveKills ROLL_FOR_DAMAGE, ScorePlus10
    IfHPPercentLessThan AI_BATTLER_DEFENDER, 20, ScorePlus10
    IfHPPercentLessThan AI_BATTLER_DEFENDER, 40, Basic_PursuitRandom
    GoTo Basic_Other

Basic_FakeOut:
    LoadIsFirstTurnInBattle AI_BATTLER_ATTACKER
    IfLoadedEqualTo TRUE, Basic_FakeOut2
    AddToMoveScore -20
    GoTo Basic_Other

Basic_FakeOut2:
    LoadBattlerAbility AI_BATTLER_DEFENDER   
    IfLoadedEqualTo ABILITY_INNER_FOCUS, Basic_Other
    IfLoadedEqualTo ABILITY_SHIELD_DUST, Basic_Other  
    GoTo ScorePlus9

Basic_PursuitRandom:
    IfRandomLessThan 128, ScorePlus8
    GoTo Basic_Other

Basic_NotHighestAdditional:
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_LOWER_SPEED_HIT, Basic_LowerSpeed
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_LOWER_ATTACK_HIT, Basic_LowerAtk
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_LOWER_SP_ATK_HIT, Basic_LowerSpAtk
    GoTo Basic_Other

Basic_LowerAtk:
    IfMoveClassKnown AI_BATTLER_DEFENDER, CLASS_PHYSICAL, Basic_CheckCancelStatDrop
    GoTo Basic_CheckCancelStatDrop2

Basic_LowerSpAtk:
    IfMoveClassKnown AI_BATTLER_DEFENDER, CLASS_SPECIAL, Basic_CheckCancelStatDrop
    GoTo Basic_CheckCancelStatDrop2

Basic_CheckCancelStatDrop:
    LoadBattlerAbility AI_BATTLER_DEFENDER   
    IfLoadedEqualTo ABILITY_CLEAR_BODY, Basic_CheckCancelStatDrop2
    IfLoadedEqualTo ABILITY_WHITE_SMOKE, Basic_CheckCancelStatDrop2
    LoadHeldItem AI_BATTLER_DEFENDER
    IfLoadedEqualTo ITEM_CLEAR_AMULET, Basic_CheckCancelStatDrop2
    AddToMoveScore 6
    GoTo Basic_Other

Basic_CheckCancelStatDrop2:
    AddToMoveScore 5
    GoTo Basic_Other

Basic_LowerSpeed:
    IfMoveEqualTo MOVE_ICY_WIND, Basic_CheckDoubles
    GoTo Basic_LowerSpeed1

Basic_CheckDoubles:
    LoadBattleType 
    IfLoadedMask BATTLE_TYPE_DOUBLES, Basic_CheckDoubles1
    GoTo Basic_LowerSpeed1

Basic_CheckDoubles1:
    AddToMoveScore 1
    GoTo Basic_LowerSpeed1

Basic_LowerSpeed1:    
    LoadBattlerAbility AI_BATTLER_DEFENDER   
    IfLoadedEqualTo ABILITY_CLEAR_BODY, Basic_CheckSpeed2
    IfLoadedEqualTo ABILITY_WHITE_SMOKE, Basic_CheckSpeed2
    LoadHeldItem AI_BATTLER_DEFENDER
    IfLoadedEqualTo ITEM_CLEAR_AMULET, Basic_CheckSpeed2
    GoTo Basic_CheckSpeed

Basic_CheckSpeed:
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Basic_CheckSpeed2
    AddToMoveScore 6
    GoTo Basic_Other

Basic_CheckSpeed2:
    AddToMoveScore 5
    GoTo Basic_Other

; NON DAMAGING MOVES

Basic_Other:
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STEALTH_ROCK, Basic_CheckStealthRock
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_TOXIC_SPIKES, Basic_CheckToxicSpikes
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SET_SPIKES, Basic_CheckSpikes
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_PROTECT, Basic_Protect
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_PASS_STATS_AND_STATUS, Basic_BatonPass
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DOUBLE_SPEED_3_TURNS, Basic_Tailwind
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_TRICK_ROOM, Basic_TrickRoom
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SET_REFLECT, Basic_Screens
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SET_LIGHT_SCREEN, Basic_Screens
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SET_SUBSTITUTE, Basic_Substitute
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_FAINT_AND_ATK_SP_ATK_DOWN_2, Basic_Memento
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STATUS_PARALYZE, Basic_Paralyze
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STATUS_BURN, Basic_Burn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SWITCH_HELD_ITEMS, Basic_Trick
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STATUS_SLEEP, Basic_Sleep
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STATUS_SLEEP_NEXT_TURN, Basic_Sleep
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STATUS_POISON, Basic_Toxic
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STATUS_BADLY_POISON, Basic_Toxic
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ATK_UP_2, Basic_OffensiveSetup
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ATK_UP, Basic_OffensiveSetup
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DEF_UP, Basic_DefensiveSetup
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DEF_UP_2, Basic_DefensiveSetup
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_DEF_UP, Basic_DefensiveSetup1 
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_DEF_UP_2, Basic_DefensiveSetup1
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DEF_SPD_UP, Basic_DefensiveSetup2
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STOCKPILE, Basic_DefensiveSetup2
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ATK_DEF_UP, Basic_MixedSetup
    IfMoveEqualTo MOVE_CALM_MIND, Basic_SpecialSetup1
    IfMoveEqualTo MOVE_BULK_UP, Basic_SpecialSetup2
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SPEED_UP_2, Basic_SpeedSetup
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_ATK_UP_2, Basic_SpOffensiveSetup
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_ATK_SP_DEF_UP, Basic_MixedSpSetup
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_QUIVER_DANCE, Basic_MixedSpSetup
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_MAX_ATK_LOSE_HALF_MAX_HP, Basic_BellyDrum
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_CRIT_UP_2, Basic_CritSetup
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_KO_MON_THAT_DEFEATED_USER, Basic_DestinyBond
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_TAUNT, Basic_Taunt
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ENCORE, Basic_Encore
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_BOOST_ALLY_POWER_BY_50_PERCENT, Basic_HelpingHand
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_MAKE_GLOBAL_TARGET, Basic_HelpingHand
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_MAKE_SHARED_MOVES_UNUSEABLE, Basic_Imprison
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_COUNTER, Basic_Counter
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_MIRROR_COAT, Basic_MirrorCoat
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RESTORE_HALF_HP, Basic_Recovery
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HEAL_HALF_REMOVE_FLYING_TYPE, Basic_Recovery
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SHELL_SMASH, Basic_ShellSmash
    PopOrEnd

Basic_OffensiveSetup:
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_UNAWARE, Basic_CheckSetupException
    GoTo Basic_OffensiveSetup1

Basic_OffensiveSetup1:
    AddToMoveScore 6
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_SLEEP, Basic_OffensiveSetup2
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_FREEZE, Basic_OffensiveSetup21
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_RECHARGING, Basic_OffensiveSetup2
    GoTo Basic_OffensiveSetup3

Basic_OffensiveSetup21:
    IfMonCanDefrost AI_BATTLER_DEFENDER, Basic_OffensiveSetup3
    GoTo Basic_OffensiveSetup2

Basic_OffensiveSetup2:
    AddToMoveScore 3
    GoTo Basic_OffensiveSetup31

Basic_OffensiveSetup3:
    EnemyTurnsToKill ROLL_FOR_DAMAGE
    IfLoadedGreaterThan 3, Basic_OffensiveSetup30
    GoTo Basic_OffensiveSetup35

Basic_OffensiveSetup30:
    AddToMoveScore 1
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Basic_OffensiveSetup35
    AddToMoveScore 1
    GoTo Basic_OffensiveSetup35

Basic_OffensiveSetup35:
    IfSpeedCompareNotEqualTo COMPARE_SPEED_SLOWER, Basic_OffensiveSetup31
    EnemyTurnsToKill ROLL_FOR_DAMAGE
    IfLoadedGreaterThan 2, Basic_OffensiveSetup31
    AddToMoveScore -5
    GoTo Basic_OffensiveSetup31

Basic_OffensiveSetup31:
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 8, Basic_End
    AddToMoveScore -1
    GoTo Basic_End

Basic_CheckSetupException:
    IfMoveEqualTo MOVE_SWORDS_DANCE, Basic_OffensiveSetup1
    IfMoveEqualTo MOVE_HOWL, Basic_OffensiveSetup1
    AddToMoveScore -20
    GoTo Basic_End

Basic_DefensiveSetup:
    AddToMoveScore 6
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_DEFENSE, 8, Basic_DefensiveSetup001
    AddToMoveScore -1
    GoTo Basic_DefensiveSetup001

Basic_DefensiveSetup001:
    IfMoveClassKnown AI_BATTLER_DEFENDER, CLASS_PHYSICAL, Basic_DefensiveSetup01
    AddToMoveScore -1
    GoTo Basic_DefensiveGlobal

Basic_DefensiveSetup01:
    IfMoveClassKnown AI_BATTLER_DEFENDER, CLASS_SPECIAL, Basic_DefensiveGlobal
    AddToMoveScore 1
    GoTo Basic_DefensiveGlobal

Basic_DefensiveSetup1:
    AddToMoveScore 6
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_DEFENSE, 8, Basic_DefensiveSetup101
    AddToMoveScore -1
    GoTo Basic_DefensiveSetup101

Basic_DefensiveSetup101:
    IfMoveClassKnown AI_BATTLER_DEFENDER, CLASS_SPECIAL, Basic_DefensiveSetup11
    AddToMoveScore -1
    GoTo Basic_DefensiveGlobal
    
Basic_DefensiveSetup11:
    IfMoveClassKnown AI_BATTLER_DEFENDER, CLASS_PHYSICAL, Basic_DefensiveGlobal
    AddToMoveScore 1
    GoTo Basic_DefensiveGlobal

Basic_DefensiveSetup2:
    AddToMoveScore 6
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_DEFENSE, 8, Basic_DefensiveSetup3
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_DEFENSE, 8, Basic_DefensiveSetup3
    GoTo Basic_DefensiveGlobal

Basic_DefensiveSetup3:
    AddToMoveScore 2
    GoTo Basic_DefensiveGlobal

Basic_DefensiveGlobal:
    IfRandomLessThan 242, Basic_DefensiveGlobal1
    GoTo Basic_End

Basic_DefensiveGlobal1:
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_BODY_PRESS, Basic_DefensiveGlobal11
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_STORED_POWER, Basic_DefensiveGlobal11
    GoTo Basic_DefensiveGlobal2

Basic_DefensiveGlobal11:
    AddToMoveScore 2
    GoTo Basic_DefensiveGlobal2

Basic_DefensiveGlobal2:
    IfSpeedCompareNotEqualTo COMPARE_SPEED_SLOWER, Basic_DefensiveGlobal3
    EnemyTurnsToKill ROLL_FOR_DAMAGE
    IfLoadedGreaterThan 2, Basic_DefensiveGlobal3
    AddToMoveScore -5
    GoTo Basic_DefensiveGlobal3

Basic_DefensiveGlobal3:
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_FREEZE, Basic_DefensiveGlobal31
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_SLEEP, Basic_DefensiveGlobal4
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_RECHARGING, Basic_DefensiveGlobal4
    GoTo Basic_End

Basic_DefensiveGlobal31:
    IfMonCanDefrost AI_BATTLER_DEFENDER, Basic_End
    GoTo Basic_DefensiveGlobal4

Basic_DefensiveGlobal4:
    AddToMoveScore 2
    GoTo Basic_End
 
Basic_SpecialSetup1:
    GoTo Basic_End

Basic_SpecialSetup2:
    GoTo Basic_End

Basic_SpeedSetup:
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, ScorePlus7
    GoTo ScoreMinus20

Basic_SpOffensiveSetup:
    GoTo Basic_SpOffensiveSetup1

Basic_SpOffensiveSetup1:
    AddToMoveScore 6
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_SLEEP, Basic_SpOffensiveSetup2
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_FREEZE, Basic_SpOffensiveSetup25
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_RECHARGING, Basic_SpOffensiveSetup2
    GoTo Basic_SpOffensiveSetup3

Basic_SpOffensiveSetup2:
    AddToMoveScore 3
    GoTo Basic_SpOffensiveSetup22

Basic_SpOffensiveSetup22:
    EnemyTurnsToKill ROLL_FOR_DAMAGE
    IfLoadedLessThan 4, Basic_SpOffensiveSetup3
    AddToMoveScore 1
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Basic_SpOffensiveSetup3
    AddToMoveScore 1   
    GoTo Basic_SpOffensiveSetup3

Basic_SpOffensiveSetup25:
    IfMonCanDefrost AI_BATTLER_DEFENDER, Basic_SpOffensiveSetup3
    GoTo Basic_SpOffensiveSetup2

Basic_SpOffensiveSetup3:
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 8, Basic_SpOffensiveSetup4
    AddToMoveScore -1
    GoTo Basic_SpOffensiveSetup4

Basic_SpOffensiveSetup4:
    IfSpeedCompareNotEqualTo COMPARE_SPEED_SLOWER, Basic_End
    EnemyTurnsToKill ROLL_FOR_DAMAGE
    IfLoadedGreaterThan 2, Basic_End
    AddToMoveScore -5
    GoTo Basic_End

Basic_MixedSetup:
    IfMoveClassKnown AI_BATTLER_ATTACKER, CLASS_PHYSICAL, Basic_OffensiveSetup
    GoTo Basic_DefensiveSetup

Basic_MixedSpSetup:
    IfMoveClassKnown AI_BATTLER_ATTACKER, CLASS_SPECIAL, Basic_OffensiveSetup
    GoTo Basic_DefensiveSetup1

Basic_BellyDrum:
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_SLEEP, ScorePlus9
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_FREEZE, Basic_BellyDrum2
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_RECHARGING, ScorePlus9
    GoTo Basic_BellyDrum3

Basic_BellyDrum2:
    IfMonCanDefrost AI_BATTLER_DEFENDER, Basic_BellyDrum3   
    GoTo ScorePlus9

Basic_BellyDrum3:
    EnemyTurnsToKill ROLL_FOR_DAMAGE
    IfLoadedGreaterThan 2, ScorePlus8
    GoTo ScorePlus4

Basic_CritSetup:
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_SHELL_ARMOR, ScoreMinus20
    IfLoadedEqualTo ABILITY_BATTLE_ARMOR, ScoreMinus20
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_SUPER_LUCK, ScorePlus7
    IfLoadedEqualTo ABILITY_SNIPER, ScorePlus7
    LoadHeldItem AI_BATTLER_ATTACKER
    IfLoadedEqualTo ITEM_SCOPE_LENS, ScorePlus7
    AddToMoveScore 6
    GoTo Basic_End

Basic_DestinyBond:
    IfSpeedCompareEqualTo COMPARE_SPEED_FASTER, Basic_DestinyBond2
    IfRandomLessThan 128, ScorePlus6
    GoTo ScorePlus5

Basic_DestinyBond2:
    IfEnemyKills AI_BATTLER_DEFENDER, USE_MAX_DAMAGE, Basic_DestinyBond3
    GoTo Basic_End

Basic_DestinyBond3:
    IfRandomLessThan 208, ScorePlus7
    GoTo ScorePlus6

Basic_Taunt:
    GoTo Basic_End
    ; RNB LEFTOVER
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_TRICK_ROOM, Basic_TauntTR
    GoTo Basic_Taunt1

Basic_TauntTR:
    IfFieldConditionsMask FIELD_CONDITION_TRICK_ROOM, ScorePlus9
    GoTo Basic_Taunt1

Basic_Taunt1:
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_DEFOG, Basic_TauntDefog
    GoTo ScorePlus5

Basic_TauntDefog:
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, ScorePlus5
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_REFLECT, ScorePlus9
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_LIGHT_SCREEN, ScorePlus9
    GoTo ScorePlus5

Basic_Encore:
    IfBattlerUnderEffect AI_BATTLER_DEFENDER, CHECK_ENCORE, ScoreMinus20
    LoadBattlerPreviousMove AI_BATTLER_DEFENDER
    IfLoadedEqualTo MOVE_NONE, ScoreMinus20

    IfSpeedCompareEqualTo COMPARE_SPEED_FASTER, Basic_Encore1
    IfRandomLessThan 128, ScorePlus6
    GoTo ScorePlus5

Basic_Encore1:
    LoadDefenderLastUsedMoveClass
    IfLoadedEqualTo CLASS_STATUS, ScorePlus7
    GoTo Basic_End

Basic_HelpingHand:
    AddToMoveScore 6
    GoTo Basic_End 

Basic_Imprison:
    ImprisonCheck AI_BATTLER_ATTACKER, AI_BATTLER_DEFENDER, Basic_Imprison2
    AddToMoveScore -20
    GoTo Basic_End

Basic_Imprison2:
    AddToMoveScore 9
    GoTo Basic_End

Basic_Counter:
    IfEnemyKills AI_BATTLER_DEFENDER, ROLL_FOR_DAMAGE, Basic_Counter1
    GoTo Basic_Counter3

Basic_Counter1:
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_STURDY, Basic_Counter2
    LoadHeldItem AI_BATTLER_ATTACKER
    IfLoadedEqualTo ITEM_FOCUS_SASH, Basic_Counter2
    GoTo Basic_UnoReverse

Basic_Counter2:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 100, Basic_UnoReverse
    IfMoveClassKnown AI_BATTLER_DEFENDER, CLASS_SPECIAL, Basic_UnoReverse
    IfMoveClassKnown AI_BATTLER_DEFENDER, CLASS_STATUS, Basic_UnoReverse
    AddToMoveScore 2
    GoTo Basic_UnoReverse

Basic_Counter3:
    IfMoveClassKnown AI_BATTLER_DEFENDER, CLASS_SPECIAL, Basic_UnoReverse
    IfMoveClassKnown AI_BATTLER_DEFENDER, CLASS_STATUS, Basic_UnoReverse
    IfRandomLessThan 51, Basic_UnoReverse
    AddToMoveScore 2
    GoTo Basic_UnoReverse

Basic_MirrorCoat:
    IfEnemyKills AI_BATTLER_DEFENDER, ROLL_FOR_DAMAGE, Basic_MirrorCoat1
    GoTo Basic_MirrorCoat3

Basic_MirrorCoat1:
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_STURDY, Basic_MirrorCoat2
    LoadHeldItem AI_BATTLER_ATTACKER
    IfLoadedEqualTo ITEM_FOCUS_SASH, Basic_MirrorCoat2
    GoTo Basic_UnoReverse

Basic_MirrorCoat2:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 100, Basic_UnoReverse
    IfMoveClassKnown AI_BATTLER_DEFENDER, CLASS_PHYSICAL, Basic_UnoReverse
    IfMoveClassKnown AI_BATTLER_DEFENDER, CLASS_STATUS, Basic_UnoReverse
    AddToMoveScore 2
    GoTo Basic_UnoReverse

Basic_MirrorCoat3:
    IfMoveClassKnown AI_BATTLER_DEFENDER, CLASS_PHYSICAL, Basic_UnoReverse
    IfMoveClassKnown AI_BATTLER_DEFENDER, CLASS_STATUS, Basic_UnoReverse
    IfRandomLessThan 51, Basic_UnoReverse
    AddToMoveScore 2
    GoTo Basic_UnoReverse

Basic_UnoReverse:
    AddToMoveScore 6
    IfSpeedCompareEqualTo COMPARE_SPEED_FASTER, Basic_UnoReverse2
    GoTo Basic_UnoReverse3

Basic_UnoReverse2:
    IfRandomGreaterThan 64, Basic_UnoReverse3
    AddToMoveScore -1
    GoTo Basic_UnoReverse3

Basic_UnoReverse3:
    IfMoveClassKnown AI_BATTLER_DEFENDER, CLASS_STATUS, Basic_UnoReverse4
    GoTo Basic_End

Basic_UnoReverse4:
    IfRandomGreaterThan 64, Basic_End
    AddToMoveScore -1
    GoTo Basic_End

Basic_CheckStealthRock:
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_STEALTH_ROCK, ScoreMinus20
    GoTo Basic_CheckHazards

Basic_CheckToxicSpikes:
    LoadSpikesLayers AI_BATTLER_DEFENDER, SIDE_CONDITION_TOXIC_SPIKES
    IfLoadedEqualTo 2, ScoreMinus20
    IfLoadedNotEqualTo 1, Basic_CheckHazards
    AddToMoveScore -1
    GoTo Basic_CheckHazards

Basic_CheckSpikes:
    LoadSpikesLayers AI_BATTLER_DEFENDER, SIDE_CONDITION_SPIKES
    IfLoadedEqualTo 3, ScoreMinus20
    IfLoadedEqualTo 2, Basic_CheckSpikes1
    IfLoadedNotEqualTo 1, Basic_CheckHazards
    AddToMoveScore -1
    GoTo Basic_CheckHazards

Basic_CheckSpikes1:
    AddToMoveScore -2
    GoTo Basic_CheckHazards

Basic_CheckHazards:
    CountAlivePartyBattlers AI_BATTLER_DEFENDER
    IfLoadedEqualTo 0, ScoreMinus20
    GoTo Basic_CheckHazards1

Basic_CheckHazards1:
    LoadIsFirstTurnInBattle AI_BATTLER_ATTACKER
    IfLoadedEqualTo TRUE, Basic_FirstTurnHazards
    GoTo Basic_NotFirstTurnHazards

Basic_FirstTurnHazards:
    IfRandomLessThan 64, Basic_FirstTurnHazards1
    AddToMoveScore 9
    GoTo Basic_HazardsEnd

Basic_FirstTurnHazards1:
    AddToMoveScore 8
    GoTo Basic_HazardsEnd

Basic_NotFirstTurnHazards:
    IfRandomLessThan 64, Basic_NotFirstTurnHazards1
    AddToMoveScore 7
    GoTo Basic_HazardsEnd

Basic_NotFirstTurnHazards1:
    AddToMoveScore 6
    GoTo Basic_HazardsEnd

Basic_HazardsEnd:
    PopOrEnd

Basic_Protect:
    AddToMoveScore 6
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_LEECH_SEED, ScoreMinus2
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_YAWN, ScoreMinus2
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_PERISH_SONG, ScoreMinus2

    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY_POISON, ScoreMinus2 
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_BURN, ScoreMinus2
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_ATTRACT, ScoreMinus2
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_CURSE, ScoreMinus2

    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_LEECH_SEED, ScorePlus1
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_YAWN, ScorePlus1
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_PERISH_SONG, ScorePlus1

    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_ANY_POISON, ScorePlus1
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_BURN, ScorePlus1
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_ATTRACT, ScorePlus1
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_CURSE, ScorePlus1

    LoadIsFirstTurnInBattle AI_BATTLER_ATTACKER
    IfLoadedEqualTo TRUE, Basic_CheckProtectDouble

    LoadBattlerPreviousMove AI_BATTLER_ATTACKER
    LoadEffectOfLoadedMove 
    IfLoadedEqualTo BATTLE_EFFECT_PROTECT, Basic_RandomProtect

Basic_CheckProtectDouble:
    LoadBattleType 
    IfLoadedMask BATTLE_TYPE_DOUBLES, ScoreMinus1

Basic_RandomProtect:
    LoadBattlerPreviousMove2 AI_BATTLER_ATTACKER
    LoadEffectOfLoadedMove 
    IfLoadedEqualTo BATTLE_EFFECT_PROTECT, Basic_Protect2

    IfRandomLessThan 128, ScoreMinus20

Basic_Protect2:
    GoTo ScoreMinus20

Basic_BatonPass:
    CountAlivePartyBattlers AI_BATTLER_ATTACKER
    IfLoadedEqualTo 0, ScoreMinus20
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_SUBSTITUTE, ScorePlus14
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 8, ScorePlus14
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_DEFENSE, 8, ScorePlus14
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 8, ScorePlus14
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_DEFENSE, 8, ScorePlus14
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_SPEED, 8, ScorePlus14
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_EVASION, 8, ScorePlus14 

Basic_Tailwind:
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, ScorePlus9
    GoTo ScorePlus5

Basic_TrickRoom:
    IfFieldConditionsMask FIELD_CONDITION_TRICK_ROOM, ScoreMinus20
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, ScorePlus10
    GoTo ScorePlus5

Basic_Screens:
    AddToMoveScore 6
    LoadHeldItem AI_BATTLER_ATTACKER
    IfLoadedEqualTo ITEM_LIGHT_CLAY, ScorePlus1
    IfRandomLessThan 128, ScorePlus1
    
Basic_Substitute:
    AddToMoveScore 6
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_SLEEP, Basic_Substitute1
    GoTo Basic_Substitute2

Basic_Substitute1:
    AddToMoveScore 1
    GoTo Basic_Substitute2

Basic_Substitute2:
    IfRandomLessThan 128, Basic_Substitute3
    AddToMoveScore 1
    GoTo Basic_Substitute3

Basic_Substitute3:
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_LEECH_SEED, Basic_Substitute4
    GoTo Basic_Substitute5

Basic_Substitute4:
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Basic_Substitute5
    AddToMoveScore 2
    GoTo Basic_Substitute5

Basic_Substitute5:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 51, ScoreMinus20
    GoTo Basic_End


Basic_Memento:
    CountAlivePartyBattlers AI_BATTLER_ATTACKER
    IfLoadedEqualTo 0, ScoreMinus20

    IfHPPercentLessThan AI_BATTLER_ATTACKER, 10, Basic_Memento1
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 33, Basic_Memento2
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 66, Basic_Memento3
    IfRandomLessThan 13, ScorePlus13
    GoTo ScorePlus6

Basic_Memento1:
    AddToMoveScore 16
    GoTo Basic_MementoEnd

Basic_Memento2:
    IfRandomLessThan 76, ScorePlus6
    AddToMoveScore 14
    GoTo Basic_MementoEnd

Basic_Memento3:
    IfRandomLessThan 128, ScorePlus6
    AddToMoveScore 13
    GoTo Basic_MementoEnd

Basic_MementoEnd:
    PopOrEnd 

Basic_Paralyze:
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MOTOR_DRIVE, ScoreMinus20
    IfLoadedEqualTo ABILITY_VOLT_ABSORB, ScoreMinus20 

    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, ScoreMinus20
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_LIMBER, ScoreMinus20
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, ScoreMinus20

    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_ANY, ScoreMinus20
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_SAFEGUARD, ScoreMinus20

    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_HEX, Basic_Paralyze3
    IfMoveKnown AI_BATTLER_ATTACKER_PARTNER, MOVE_HEX, Basic_Paralyze3
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_FLINCH_HIT, Basic_Paralyze3
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_FLINCH_BURN_HIT, Basic_Paralyze3
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_FLINCH_FREEZE_HIT, Basic_Paralyze3
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_FLINCH_PARALYZE_HIT, Basic_Paralyze3
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_FLINCH_MINIMIZE_DOUBLE_HIT, Basic_Paralyze3

    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_CONFUSION, Basic_Paralyze3
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_ATTRACT, Basic_Paralyze3
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Basic_ParalyzeCheckSpeed
    GoTo Basic_Paralyze4

Basic_ParalyzeCheckSpeed:
   IfSpeedCompareAfterParaEqualTo COMPARE_SPEED_SLOWER, Basic_Paralyze4
   GoTo Basic_Paralyze3

Basic_Paralyze1:
    IfRandomLessThan 128, Basic_Paralyze2
    GoTo Basic_Paralyze_End

Basic_Paralyze2:
    AddToMoveScore -1
    GoTo Basic_Paralyze_End

Basic_Paralyze3:
    AddToMoveScore 8
    GoTo Basic_Paralyze1

Basic_Paralyze4:
    AddToMoveScore 7
    GoTo Basic_Paralyze1

Basic_Paralyze_End:
    PopOrEnd

Basic_Burn:
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_WATER_VEIL, ScoreMinus20
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, ScoreMinus20
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_ANY, ScoreMinus20
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_FIRE, ScoreMinus20
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_FIRE, ScoreMinus20
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_SAFEGUARD, ScoreMinus20

    AddToMoveScore 6

    IfRandomLessThan 95, Basic_Burn1
    GoTo Basic_End

Basic_Burn1:
    IfMoveClassKnown AI_BATTLER_DEFENDER, CLASS_PHYSICAL, Basic_Burn2
    GoTo Basic_Burn3

Basic_Burn2:
    AddToMoveScore 1
    GoTo Basic_Burn3

Basic_Burn3:
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_HEX, ScorePlus1
    IfMoveKnown AI_BATTLER_ATTACKER_PARTNER, MOVE_HEX, ScorePlus1
    GoTo Basic_End

Basic_Trick:
    IfHeldItemEqualTo AI_BATTLER_ATTACKER, ITEM_TOXIC_ORB, Basic_Trick1
    IfHeldItemEqualTo AI_BATTLER_ATTACKER, ITEM_FLAME_ORB, Basic_Trick1 
    IfHeldItemEqualTo AI_BATTLER_ATTACKER, ITEM_BLACK_SLUDGE, Basic_Trick1 
    IfHeldItemEqualTo AI_BATTLER_ATTACKER, ITEM_IRON_BALL, ScorePlus7
    IfHeldItemEqualTo AI_BATTLER_ATTACKER, ITEM_LAGGING_TAIL, ScorePlus7
    IfHeldItemEqualTo AI_BATTLER_ATTACKER, ITEM_STICKY_BARB, ScorePlus7
    IfHeldItemEqualTo AI_BATTLER_ATTACKER, ITEM_FLOAT_STONE, ScorePlus7
    GoTo ScorePlus5

Basic_Trick1:
    IfRandomLessThan 128, ScorePlus7 
    GoTo ScorePlus6

Basic_Sleep:
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_ANY, ScoreMinus20
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_SAFEGUARD, ScoreMinus20
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_INSOMNIA, ScoreMinus20
    IfLoadedEqualTo ABILITY_VITAL_SPIRIT, ScoreMinus20
    AddToMoveScore 6
    IfRandomLessThan 64, Basic_Sleep1
    GoTo Basic_End

Basic_Sleep1:
    AddToMoveScore 1
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_DREAM_EATER, Basic_Sleep2
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_NIGHTMARE, Basic_Sleep2
    GoTo Basic_Sleep3

Basic_Sleep2:
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_SLEEP_TALK, Basic_Sleep3
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_SNORE, Basic_Sleep3
    AddToMoveScore 1
    GoTo Basic_Sleep3

Basic_Sleep3:
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_HEX, ScorePlus1
    IfMoveKnown AI_BATTLER_ATTACKER_PARTNER, MOVE_HEX, ScorePlus1
    GoTo Basic_End

Basic_Toxic:
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_STEEL, ScoreMinus20
    IfLoadedEqualTo TYPE_POISON, ScoreMinus20
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_STEEL, ScoreMinus20
    IfLoadedEqualTo TYPE_POISON, ScoreMinus20
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_IMMUNITY, ScoreMinus20
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, ScoreMinus20
    IfLoadedEqualTo ABILITY_POISON_HEAL, ScoreMinus20

    AddToMoveScore 6

    IfRandomLessThan 159, Basic_End
    IfDefenderHasDamagingMoves Basic_Toxic1
    AddToMoveScore 1
    GoTo Basic_Toxic1

Basic_Toxic1:
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_HEX, ScorePlus2
    IfMoveKnown AI_BATTLER_ATTACKER_PARTNER, MOVE_HEX, ScorePlus2
    AddToMoveScore 1
    GoTo Basic_End

Basic_Recovery:
    IfHPPercentEqualTo AI_BATTLER_ATTACKER, 100, ScoreMinus20
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 85, ScoreMinus6
    IfShouldRecover AI_BATTLER_ATTACKER, ScorePlus7
    AddToMoveScore 5
    GoTo Basic_End

Basic_SunRecovery:
    IfHPPercentEqualTo AI_BATTLER_ATTACKER, 100, ScoreMinus20
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 85, ScoreMinus6

    LoadCurrentWeather 
    IfLoadedEqualTo AI_WEATHER_SUNNY, Basic_SunRecovery1
    IfShouldRecover AI_BATTLER_ATTACKER, ScorePlus7
    AddToMoveScore 5
    GoTo Basic_SunRecovery2

Basic_SunRecovery1:
    IfShouldRecover AI_BATTLER_ATTACKER, ScorePlus7
    GoTo Basic_SunRecovery2

Basic_SunRecovery2:
    IfShouldRecover AI_BATTLER_ATTACKER, ScorePlus7
    AddToMoveScore 5
    GoTo Basic_End

Basic_Rest:
    IfShouldRecover AI_BATTLER_ATTACKER, Basic_Rest2
    AddToMoveScore 5
    GoTo Basic_End

Basic_Rest2:
    LoadHeldItem AI_BATTLER_ATTACKER
    IfLoadedEqualTo ITEM_LUM_BERRY, ScorePlus8
    IfLoadedEqualTo ITEM_CHESTO_BERRY, ScorePlus8  

    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_SHED_SKIN, ScorePlus8
    IfLoadedEqualTo ABILITY_EARLY_BIRD, ScorePlus8

    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_SLEEP_TALK, ScorePlus8
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_SNORE, ScorePlus8

    LoadCurrentWeather 
    IfLoadedEqualTo AI_WEATHER_RAINING, Basic_Hydration    

    GoTo ScorePlus7

Basic_Hydration:
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_HYDRATION, ScorePlus8    
    GoTo ScorePlus7

Basic_ShellSmash:
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 7, ScoreMinus20
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 12, ScoreMinus20
    AddToMoveScore 6
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_SLEEP, Basic_ShellSmash2
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_FREEZE, Basic_ShellSmash21
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_RECHARGING, Basic_ShellSmash2
    GoTo Basic_ShellSmash3

Basic_ShellSmash21:
    IfMonCanDefrost AI_BATTLER_DEFENDER, Basic_ShellSmash3
    GoTo Basic_ShellSmash2

Basic_ShellSmash2:
    AddToMoveScore 3
    GoTo Basic_ShellSmash3

Basic_ShellSmash3:
    LoadHeldItem AI_BATTLER_DEFENDER
    IfLoadedEqualTo ITEM_WHITE_HERB, Basic_ShellSmash4

    EnemyTurnsToKill ROLL_FOR_DAMAGE
    IfLoadedLessThan 2, Basic_ShellSmash5
    AddToMoveScore 2
    GoTo Basic_End

Basic_ShellSmash4:
    IfEnemyKills AI_BATTLER_DEFENDER, USE_MAX_DAMAGE, Basic_ShellSmash5
    AddToMoveScore 2
    GoTo Basic_End

Basic_ShellSmash5:
    AddToMoveScore -2
    GoTo Basic_End

Basic_End:
    PopOrEnd

Basic_Partner:
    GoTo ScoreMinus20

Golmon_Main:

    FlagMoveDamageScore FALSE
    IfLoadedEqualTo AI_NO_COMPARISON_MADE, Golmon_End

    TurnsToKill USE_MAX_DAMAGE
    IfLoadedLessThan 2, ScoreMinus1

    PopOrEnd   

Golmon_End:
    PopOrEnd

; FIN DE L'IA CUSTOM AZURE PLATINUM

ScoreMinus1:
    AddToMoveScore -1
    PopOrEnd 

ScoreMinus2:
    AddToMoveScore -2
    PopOrEnd 

ScoreMinus3:
    AddToMoveScore -3
    PopOrEnd 

ScoreMinus5:
    AddToMoveScore -5
    PopOrEnd 

ScoreMinus6: ; unused
    AddToMoveScore -6
    PopOrEnd 

ScoreMinus8:
    AddToMoveScore -8
    PopOrEnd 

ScoreMinus10:
    AddToMoveScore -10
    PopOrEnd 

ScoreMinus12:
    AddToMoveScore -12
    PopOrEnd 

ScoreMinus20:
    AddToMoveScore -20
    PopOrEnd

ScoreMinus30:
    AddToMoveScore -30
    PopOrEnd 

ScorePlus1:
    AddToMoveScore 1
    PopOrEnd 

ScorePlus2:
    AddToMoveScore 2
    PopOrEnd 

ScorePlus3:
    AddToMoveScore 3
    PopOrEnd 

ScorePlus4:
    AddToMoveScore 4
    PopOrEnd 

ScorePlus5:
    AddToMoveScore 5
    PopOrEnd

ScorePlus6:
    AddToMoveScore 6
    PopOrEnd 

ScorePlus7:
    AddToMoveScore 7
    PopOrEnd 

ScorePlus8:
    AddToMoveScore 8
    PopOrEnd 

ScorePlus9:
    AddToMoveScore 9
    PopOrEnd 

ScorePlus10:
    AddToMoveScore 10
    PopOrEnd 

ScorePlus11:
    AddToMoveScore 11
    PopOrEnd 

ScorePlus12:
    AddToMoveScore 12
    PopOrEnd 

ScorePlus13:
    AddToMoveScore 13
    PopOrEnd 

ScorePlus14:
    AddToMoveScore 14
    PopOrEnd 

ScorePlus15:
    AddToMoveScore 15
    PopOrEnd 

ScorePlus16:
    AddToMoveScore 16
    PopOrEnd 

ScorePlus17:
    AddToMoveScore 17
    PopOrEnd 

ScorePlus18:
    AddToMoveScore 18
    PopOrEnd 

ScorePlus19:
    AddToMoveScore 19
    PopOrEnd 

ScorePlus20:
    AddToMoveScore 20
    PopOrEnd 

Expert_Main:
    IfTargetIsPartner Terminate
    PopOrEnd

EvalAttack_Main:
    IfTargetIsPartner Terminate
    PopOrEnd

SetupFirstTurn_Main:
    IfTargetIsPartner Terminate
    PopOrEnd

DamagePriority_Main:
    IfTargetIsPartner Terminate
    PopOrEnd

Risky_Main:
    IfTargetIsPartner Terminate
    PopOrEnd
 
BatonPass_Main:
    IfTargetIsPartner Terminate
    PopOrEnd

TagStrategy_Main:
    IfTargetIsPartner Terminate
    PopOrEnd

CheckHP_Main:
    IfTargetIsPartner Terminate
    PopOrEnd

Weather_Main:
    IfTargetIsPartner Terminate
    PopOrEnd

Harrassment_Main:
    IfTargetIsPartner Terminate
    PopOrEnd

RoamingPokemon_Main:
    ; If the Roamer is trapped, break from this routine
    ; Otherwise, override all other possible moves and Escape
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_BIND, RoamingPokemon_Trapped
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_MEAN_LOOK, RoamingPokemon_Trapped
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_SHADOW_TAG, RoamingPokemon_Trapped
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_LEVITATE, RoamingPokemon_NotTrapped
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_ARENA_TRAP, RoamingPokemon_Trapped

RoamingPokemon_NotTrapped:
    Escape 

RoamingPokemon_Trapped:
    PopOrEnd 

Safari_Main:
    Dummy3E 1
    Dummy3F 
    Escape 

CatchTutorial_Main:
    ; If the target is at 20% or less HP, flee from the battle
    IfHPPercentEqualTo AI_BATTLER_DEFENDER, 20, CatchTutorial_Escape
    IfHPPercentLessThan AI_BATTLER_DEFENDER, 20, CatchTutorial_Escape
    PopOrEnd 

CatchTutorial_Escape:
    Escape 

Terminate:
    PopOrEnd 

    .endif