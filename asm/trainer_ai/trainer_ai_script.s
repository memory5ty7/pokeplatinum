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
    LabelDistance EvalAttack_Main,     FlagTable ; AI_FLAG_EVAL_ATTACK
    LabelDistance Expert_Main,         FlagTable ; AI_FLAG_EXPERT
    LabelDistance SetupFirstTurn_Main, FlagTable ; AI_FLAG_SETUP_FIRST_TURN
    LabelDistance Risky_Main,          FlagTable ; AI_FLAG_RISKY
    LabelDistance DamagePriority_Main, FlagTable ; AI_FLAG_DAMAGE_PRIORITY
    LabelDistance BatonPass_Main,      FlagTable ; AI_FLAG_BATON_PASS
    LabelDistance TagStrategy_Main,    FlagTable ; AI_FLAG_TAG_STRATEGY
    LabelDistance CheckHP_Main,        FlagTable ; AI_FLAG_CHECK_HP
    LabelDistance Weather_Main,        FlagTable ; AI_FLAG_WEATHER
    LabelDistance Harrassment_Main,    FlagTable ; AI_FLAG_HARRASSMENT
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_11
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
    IfTargetIsPartner Terminate

    FlagMoveDamageScore FALSE
    IfLoadedEqualTo AI_NO_COMPARISON_MADE, Basic_Other

    GoTo CheckKill

CheckKill:
    IfCurrentMoveKills USE_MAX_DAMAGE, Basic_CheckSpeedKill
    GoTo Basic_HighestDamage

Basic_CheckSpeedKill:
    IfSpeedCompareEqualTo COMPARE_SPEED_FASTER, Basic_FastKill
    GoTo Basic_SlowKill

Basic_FastKill:
    AddToMoveScore 6
    GoTo Basic_HighestDamage

Basic_SlowKill:
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_PRIORITY_1, Basic_FastKill
    AddToMoveScore 3
    GoTo Basic_HighestDamage

Basic_HighestDamage:
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_PRIORITY_1, Basic_Priority
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_WHIRLPOOL, Basic_Trapping
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_BIND_HIT, Basic_Trapping
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HALVE_DEFENSE, Basic_Explosion
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HIT_IN_3_TURNS, Basic_Other

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
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DOUBLE_POWER_EACH_TURN_LOCK_INTO, Basic_Rollout
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
    IfCurrentMoveKills USE_MAX_DAMAGE, ScorePlus10
    IfHPPercentLessThan AI_BATTLER_DEFENDER, 20, ScorePlus10
    IfHPPercentLessThan AI_BATTLER_DEFENDER, 40, Basic_PursuitRandom
    IfSpeedCompareEqualTo COMPARE_SPEED_FASTER, ScorePlus3
    GoTo Basic_Other

Basic_FakeOut:
    LoadIsFirstTurnInBattle AI_BATTLER_ATTACKER
    IfLoadedEqualTo TRUE, Basic_FakeOut2
    GoTo Basic_Other

Basic_FakeOut2:
    LoadBattlerAbility AI_BATTLER_DEFENDER   
    IfLoadedNotEqualTo ABILITY_INNER_FOCUS, ScorePlus9  
    GoTo Basic_Other

Basic_PursuitRandom:
    IfRandomLessThan 128, ScorePlus8
    GoTo Basic_Other

Basic_NotHighestAdditional:
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_LOWER_SPEED_HIT, Basic_LowerSpeed
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
    IfLoadedEqualTo ABILITY_CLEAR_BODY, Basic_CheckSpeed
    IfLoadedEqualTo ABILITY_WHITE_SMOKE, Basic_CheckSpeed
    LoadHeldItem AI_BATTLER_DEFENDER
    IfLoadedEqualTo ITEM_CLEAR_AMULET, Basic_CheckSpeed
    AddToMoveScore 5
    GoTo Basic_Other

Basic_CheckSpeed:
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Basic_Other
    AddToMoveScore 5
    GoTo Basic_Other

Basic_CheckSpeed2:
    AddToMoveScore 6
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
    IfMoveEqualTo MOVE_CALM_MIND, Basic_SpecialSetup1
    IfMoveEqualTo MOVE_BULK_UP, Basic_SpecialSetup2
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SPEED_UP_2, Basic_SpeedSetup
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_ATK_UP_2, Basic_SpOffensiveSetup
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_MAX_ATK_LOSE_HALF_MAX_HP, Basic_BellyDrum
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_CRIT_UP_2, Basic_CritSetup
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_KO_MON_THAT_DEFEATED_USER, Basic_DestinyBond
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_TAUNT, Basic_Taunt
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ENCORE, Basic_Encore
    PopOrEnd

Basic_OffensiveSetup:
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_UNAWARE, Basic_CheckSetupException
    GoTo Basic_OffensiveSetup1

Basic_OffensiveSetup1:
    AddToMoveScore 6
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_SLEEP, Basic_OffensiveSetup2
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_FREEZE, Basic_OffensiveSetup2 
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_RECHARGING, Basic_OffensiveSetup2
    GoTo Basic_OffensiveSetup3

Basic_OffensiveSetup2:
    AddToMoveScore 3
    GoTo Basic_OffensiveSetup3

Basic_OffensiveSetup3:
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
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_DEFENSE, 8, Basic_End
    AddToMoveScore -1
    GoTo Basic_End

Basic_DefensiveSetup1:
    AddToMoveScore 6
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_DEFENSE, 8, Basic_End
    AddToMoveScore -1
    GoTo Basic_End

Basic_DefensiveSetup2:
    AddToMoveScore 6
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_DEFENSE, 8, Basic_DefensiveSetup3
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_DEFENSE, 8, Basic_DefensiveSetup3
    GoTo Basic_End

Basic_DefensiveSetup3:
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
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_FREEZE, Basic_SpOffensiveSetup2 
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_RECHARGING, Basic_SpOffensiveSetup2
    GoTo Basic_SpOffensiveSetup3

Basic_SpOffensiveSetup2:
    AddToMoveScore 3
    GoTo Basic_SpOffensiveSetup3

Basic_SpOffensiveSetup3:
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 8, Basic_End
    AddToMoveScore -1
    GoTo Basic_End

Basic_BellyDrum:
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_SLEEP, ScorePlus9
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_FREEZE, ScorePlus9
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_RECHARGING, ScorePlus9
    AddToMoveScore 4
    GoTo Basic_End

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
    IfSpeedCompareEqualTo COMPARE_SPEED_FASTER, Basic_End
    IfRandomLessThan 128, ScorePlus6
    GoTo ScorePlus5

Basic_Taunt:
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
    IfSpeedCompareEqualTo COMPARE_SPEED_FASTER, Basic_Encore1
    IfRandomLessThan 128, ScorePlus6
    GoTo ScorePlus5

Basic_Encore1:
    LoadDefenderLastUsedMoveClass
    IfLoadedEqualTo CLASS_STATUS, ScorePlus7
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
    IfRandomLessThan 64, Basic_FirstTurnHazards
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
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY_POISON, ScoreMinus2 
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_BURN, ScoreMinus2
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_ATTRACT, ScoreMinus2
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_CURSE, ScoreMinus2

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
    IfRandomLessThan 128, ScoreMinus20

Basic_BatonPass:
    CountAlivePartyBattlers AI_BATTLER_ATTACKER
    IfLoadedEqualTo 0, ScoreMinus20
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_SUBSTITUTE, ScorePlus14
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 8, ScorePlus14
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_DEFENSE, 8, ScorePlus14
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 8, ScorePlus14
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_DEFENSE, 8, ScorePlus14
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
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_SLEEP, ScorePlus2
    IfRandomLessThan 128, ScorePlus1
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 51, ScoreMinus20

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

    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_CONFUSION, Basic_Paralyze3
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_ATTRACT, Basic_Paralyze3
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Basic_Paralyze1 
    AddToMoveScore 7
    GoTo Basic_Paralyze1

Basic_Paralyze1:
    IfRandomLessThan 128, Basic_Paralyze2
    GoTo Basic_Paralyze_End

Basic_Paralyze2:
    AddToMoveScore -1
    GoTo Basic_Paralyze_End

Basic_Paralyze3:
    AddToMoveScore 8
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
    PopOrEnd

Basic_Trick:
    IfHeldItemEqualTo AI_BATTLER_ATTACKER, ITEM_TOXIC_ORB, Basic_Trick1
    IfHeldItemEqualTo AI_BATTLER_ATTACKER, ITEM_FLAME_ORB, Basic_Trick1 
    IfHeldItemEqualTo AI_BATTLER_ATTACKER, ITEM_BLACK_SLUDGE, Basic_Trick1 
    IfHeldItemEqualTo AI_BATTLER_ATTACKER, ITEM_IRON_BALL, ScorePlus7
    IfHeldItemEqualTo AI_BATTLER_ATTACKER, ITEM_LAGGING_TAIL, ScorePlus7
    IfHeldItemEqualTo AI_BATTLER_ATTACKER, ITEM_STICKY_BARB, ScorePlus7
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
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_DREAM_EATER, Basic_Sleep2
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_DREAM_EATER, Basic_Sleep2
    GoTo Basic_End

Basic_Sleep2:
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_SLEEP_TALK, Basic_End
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_SNORE, Basic_End
    AddToMoveScore 1
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
    IfAttackerHasNoDamagingMoves Basic_Toxic1
    AddToMoveScore 1
    GoTo Basic_End

Basic_Toxic1:
    AddToMoveScore 2
    GoTo Basic_End

Basic_End:
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