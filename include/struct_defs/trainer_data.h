#ifndef POKEPLATINUM_STRUCT_TRAINER_DATA_H
#define POKEPLATINUM_STRUCT_TRAINER_DATA_H

/*
 * Note: *most* source files should not include this header directly, and should
 * instead including `struct_defs/trainer.h`. This header is held separately for
 * use by data-packing routines.
 */

#include "constants/moves.h"

#define TRAINER_MON_FORM_SHIFT 10

#define MAX_TRAINER_ITEMS 4
#define MAX_DV            255

enum TrainerDataType {
    TRDATATYPE_BASE = 0,
    TRDATATYPE_WITH_MOVES,
    TRDATATYPE_WITH_ITEM,
    TRDATATYPE_WITH_MOVES_AND_ITEM,
};

typedef struct TrainerHeader {
    u8 monDataType;
    u8 trainerType;
    u8 sprite;
    u8 partySize;
    u16 items[MAX_TRAINER_ITEMS];
    u32 aiMask;
    u32 battleType;
} TrainerHeader;

typedef struct TrainerMonBase {
    u8 nature;
    u8 abilitySlot;
    u8 status;
    u8 ivs;
    u8 preDamage;
    u8 gender;
    u8 level;
    u16 species;
    u16 cbSeal;
} TrainerMonBase;

typedef struct TrainerMonWithMoves {
    u8 nature;
    u8 abilitySlot;
    u8 status;
    u8 ivs;
    u8 preDamage;
    u8 gender;
    u8 level;
    u16 species;
    u16 moves[LEARNED_MOVES_MAX];
    u16 cbSeal;
} TrainerMonWithMoves;

typedef struct TrainerMonWithItem {
    u8 nature;
    u8 abilitySlot;
    u8 status;
    u8 ivs;
    u8 preDamage;
    u8 gender;
    u8 level;
    u16 species;
    u16 item;
    u16 cbSeal;
} TrainerMonWithItem;

typedef struct TrainerMonWithMovesAndItem {
    u8 nature;
    u8 abilitySlot;
    u8 status;
    u8 ivs;
    u8 preDamage;
    u8 gender;
    u8 level;
    u16 species;
    u16 item;
    u16 moves[LEARNED_MOVES_MAX];
    u16 cbSeal;
} TrainerMonWithMovesAndItem;

#endif // POKEPLATINUM_STRUCT_TRAINER_DATA_H