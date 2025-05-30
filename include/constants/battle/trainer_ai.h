#ifndef POKEPLATINUM_CONSTANTS_BATTLE_TRAINER_AI_H
#define POKEPLATINUM_CONSTANTS_BATTLE_TRAINER_AI_H

#include "generated/ai_action_choices.h"
#include "generated/ai_flags.h"
#include "generated/ai_load_type_targets.h"
#include "generated/ai_weather_types.h"

#define AI_INIT_SCORE_MOVE_1 (1 << 0)
#define AI_INIT_SCORE_MOVE_2 (1 << 1)
#define AI_INIT_SCORE_MOVE_3 (1 << 2)
#define AI_INIT_SCORE_MOVE_4 (1 << 3)

#define AI_INIT_SCORE_ALL_MOVES (AI_INIT_SCORE_MOVE_1 | AI_INIT_SCORE_MOVE_2 | AI_INIT_SCORE_MOVE_3 | AI_INIT_SCORE_MOVE_4)

#define AI_STATUS_FLAG_DONE     (1 << 0)
#define AI_STATUS_FLAG_ESCAPE   (1 << 1)
#define AI_STATUS_FLAG_SAFARI   (1 << 2)
#define AI_STATUS_FLAG_BREAK    (1 << 3)
#define AI_STATUS_FLAG_CONTINUE (1 << 4)

#define AI_STATUS_FLAG_DONE_OFF     (AI_STATUS_FLAG_DONE ^ 0xFF)
#define AI_STATUS_FLAG_ESCAPE_OFF   (AI_STATUS_FLAG_ESCAPE ^ 0xFF)
#define AI_STATUS_FLAG_SAFARI_OFF   (AI_STATUS_FLAG_SAFARI ^ 0xFF)
#define AI_STATUS_FLAG_BREAK_OFF    (AI_STATUS_FLAG_BREAK ^ 0xFF)
#define AI_STATUS_FLAG_CONTINUE_OFF (AI_STATUS_FLAG_CONTINUE ^ 0xFF)

#define AI_BATTLER_DEFENDER         0
#define AI_BATTLER_ATTACKER         1
#define AI_BATTLER_DEFENDER_PARTNER (AI_BATTLER_DEFENDER ^ 2)
#define AI_BATTLER_ATTACKER_PARTNER (AI_BATTLER_ATTACKER ^ 2)

#define AI_MAX_STACK_SIZE 8

#define AI_NO_COMPARISON_MADE     0
#define AI_NOT_HIGHEST_DAMAGE     1
#define AI_MOVE_IS_HIGHEST_DAMAGE 2

#define AI_NOT_HAVE 0
#define AI_HAVE     1
#define AI_UNKNOWN  2

// for the script only; readable flags for commands that accept a boolean
// flag determining if they roll for damage or not
#define USE_MAX_DAMAGE  0
#define ROLL_FOR_DAMAGE 1

#define CHECK_DISABLE 0
#define CHECK_ENCORE  1

#define CHECK_HIGHER_THAN_TARGET 0
#define CHECK_LOWER_THAN_TARGET  1
#define CHECK_EQUAL_TO_TARGET    2

#endif
