#include "item.h"

#include <nitro.h>
#include <string.h>

#include "constants/gba/items.h"
#include "constants/items.h"
#include "constants/moves.h"

#include "bag.h"
#include "heap.h"
#include "message.h"
#include "narc.h"
#include "strbuf.h"

#include "res/items/item_icon.naix"

#include "vars_flags.h"
#include "constants/savedata/vars_flags.h"

typedef struct ItemArchiveIDs {
    u16 dataID; // Member file index in pl_item_data.narc; TODO: Use NAIX generated from pl_item_data
    u16 iconID; // Member file index of NCGR in item_icon.narc; TODO: Use NAIX generated from item_icon.narc
    u16 paletteID; // Member file index of NCLR in item_icon.narc; TODO: Use NAIX generated from item_icon.narc
    u16 gen3ID; // Transfer ID from gen3
} ItemArchiveIDs;

static s32 ItemPartyParam_Get(ItemPartyParam *partyParam, enum ItemDataParam attributeID);

const ItemArchiveIDs sItemArchiveIDs[] = {
    [ITEM_NONE]         = {  .dataID = 0x0,   .iconID = icon_unused_NCGR,        .paletteID = pltt_unused_NCLR,         .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_MASTER_BALL]  = {  .dataID = 0x1,   .iconID = icon_master_ball_NCGR,   .paletteID = pltt_master_ball_NCLR,    .gen3ID = GBA_ITEM_MASTER_BALL,   },
    [ITEM_ULTRA_BALL]   = {  .dataID = 0x2,   .iconID = icon_ultra_ball_NCGR,    .paletteID = pltt_ultra_ball_NCLR,     .gen3ID = GBA_ITEM_ULTRA_BALL,    },
    [ITEM_GREAT_BALL]   = {  .dataID = 0x3,   .iconID = icon_great_ball_NCGR,    .paletteID = pltt_great_ball_NCLR,     .gen3ID = GBA_ITEM_GREAT_BALL,    },
    [ITEM_POKE_BALL]    = {  .dataID = 0x4,   .iconID = icon_poke_ball_NCGR,     .paletteID = pltt_poke_ball_NCLR,      .gen3ID = GBA_ITEM_POKE_BALL,     },
    [ITEM_SAFARI_BALL]  = {  .dataID = 0x5,   .iconID = icon_safari_ball_NCGR,   .paletteID = pltt_safari_ball_NCLR,    .gen3ID = GBA_ITEM_SAFARI_BALL,   },
    [ITEM_NET_BALL]     = {  .dataID = 0x6,   .iconID = icon_net_ball_NCGR,      .paletteID = pltt_net_ball_NCLR,       .gen3ID = GBA_ITEM_NET_BALL,      },
    [ITEM_DIVE_BALL]    = {  .dataID = 0x7,   .iconID = icon_dive_ball_NCGR,     .paletteID = pltt_dive_ball_NCLR,      .gen3ID = GBA_ITEM_DIVE_BALL,     },
    [ITEM_NEST_BALL]    = {  .dataID = 0x8,   .iconID = icon_nest_ball_NCGR,     .paletteID = pltt_nest_ball_NCLR,      .gen3ID = GBA_ITEM_NEST_BALL,     },
    [ITEM_REPEAT_BALL]  = {  .dataID = 0x9,   .iconID = icon_repeat_ball_NCGR,   .paletteID = pltt_repeat_ball_NCLR,    .gen3ID = GBA_ITEM_REPEAT_BALL,   },
    [ITEM_TIMER_BALL]   = {  .dataID = 0xA,   .iconID = icon_timer_ball_NCGR,    .paletteID = pltt_repeat_ball_NCLR,    .gen3ID = GBA_ITEM_TIMER_BALL,    },
    [ITEM_LUXURY_BALL]  = {  .dataID = 0xB,   .iconID = icon_luxury_ball_NCGR,   .paletteID = pltt_luxury_ball_NCLR,    .gen3ID = GBA_ITEM_LUXURY_BALL,   },
    [ITEM_PREMIER_BALL] = {  .dataID = 0xC,   .iconID = icon_premier_ball_NCGR,  .paletteID = pltt_luxury_ball_NCLR,    .gen3ID = GBA_ITEM_PREMIER_BALL,  },
    [ITEM_DUSK_BALL]    = {  .dataID = 0xD,   .iconID = icon_dusk_ball_NCGR,     .paletteID = pltt_dusk_ball_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_HEAL_BALL]    = {  .dataID = 0xE,   .iconID = icon_heal_ball_NCGR,     .paletteID = pltt_heal_ball_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_QUICK_BALL]   = {  .dataID = 0xF,   .iconID = icon_quick_ball_NCGR,    .paletteID = pltt_quick_ball_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_CHERISH_BALL] = {  .dataID = 0x10,  .iconID = icon_cherish_ball_NCGR,  .paletteID = pltt_cherish_ball_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_POTION]       = {  .dataID = 0x11,  .iconID = icon_potion_NCGR,        .paletteID = pltt_potion_NCLR,         .gen3ID = GBA_ITEM_POTION,        },
    [ITEM_ANTIDOTE]     = {  .dataID = 0x12,  .iconID = icon_antidote_NCGR,      .paletteID = pltt_antidote_NCLR,       .gen3ID = GBA_ITEM_ANTIDOTE,      },
    [ITEM_BURN_HEAL]    = {  .dataID = 0x13,  .iconID = icon_status_heal_NCGR,   .paletteID = pltt_burn_heal_NCLR,      .gen3ID = GBA_ITEM_BURN_HEAL,     },
    [ITEM_ICE_HEAL]     = {  .dataID = 0x14,  .iconID = icon_status_heal_NCGR,   .paletteID = pltt_ice_heal_NCLR,       .gen3ID = GBA_ITEM_ICE_HEAL,      },
    [ITEM_AWAKENING]    = {  .dataID = 0x15,  .iconID = icon_status_heal_NCGR,   .paletteID = pltt_awakening_NCLR,      .gen3ID = GBA_ITEM_AWAKENING,     },
    [ITEM_PARLYZ_HEAL]  = {  .dataID = 0x16,  .iconID = icon_status_heal_NCGR,   .paletteID = pltt_parlyz_heal_NCLR,    .gen3ID = GBA_ITEM_PARALYZE_HEAL, },
    [ITEM_FULL_RESTORE] = {  .dataID = 0x17,  .iconID = icon_full_restore_NCGR,  .paletteID = pltt_full_restore_NCLR,   .gen3ID = GBA_ITEM_FULL_RESTORE,  },
    [ITEM_MAX_POTION]   = {  .dataID = 0x18,  .iconID = icon_full_restore_NCGR,  .paletteID = pltt_max_potion_NCLR,     .gen3ID = GBA_ITEM_MAX_POTION,    },
    [ITEM_HYPER_POTION] = {  .dataID = 0x19,  .iconID = icon_potion_NCGR,        .paletteID = pltt_hyper_potion_NCLR,   .gen3ID = GBA_ITEM_HYPER_POTION,  },
    [ITEM_SUPER_POTION] = {  .dataID = 0x1A,  .iconID = icon_potion_NCGR,        .paletteID = pltt_super_potion_NCLR,   .gen3ID = GBA_ITEM_SUPER_POTION,  },
    [ITEM_FULL_HEAL]    = {  .dataID = 0x1B,  .iconID = icon_full_heal_NCGR,     .paletteID = pltt_full_heal_NCLR,      .gen3ID = GBA_ITEM_FULL_HEAL,     },
    [ITEM_REVIVE]       = {  .dataID = 0x1C,  .iconID = icon_revive_NCGR,        .paletteID = pltt_revive_NCLR,         .gen3ID = GBA_ITEM_REVIVE,        },
    [ITEM_MAX_REVIVE]   = {  .dataID = 0x1D,  .iconID = icon_max_revive_NCGR,    .paletteID = pltt_revive_NCLR,         .gen3ID = GBA_ITEM_MAX_REVIVE,    },
    [ITEM_FRESH_WATER]  = {  .dataID = 0x1E,  .iconID = icon_fresh_water_NCGR,   .paletteID = pltt_fresh_water_NCLR,    .gen3ID = GBA_ITEM_FRESH_WATER,   },
    [ITEM_SODA_POP]     = {  .dataID = 0x1F,  .iconID = icon_soda_pop_NCGR,      .paletteID = pltt_soda_pop_NCLR,       .gen3ID = GBA_ITEM_SODA_POP,      },
    [ITEM_LEMONADE]     = {  .dataID = 0x20,  .iconID = icon_lemonade_NCGR,      .paletteID = pltt_lemonade_NCLR,       .gen3ID = GBA_ITEM_LEMONADE,      },
    [ITEM_MOOMOO_MILK]  = {  .dataID = 0x21,  .iconID = icon_moomoo_milk_NCGR,   .paletteID = pltt_moomoo_milk_NCLR,    .gen3ID = GBA_ITEM_MOOMOO_MILK,   },
    [ITEM_ENERGYPOWDER] = {  .dataID = 0x22,  .iconID = icon_energypowder_NCGR,  .paletteID = pltt_energypowder_NCLR,   .gen3ID = GBA_ITEM_ENERGY_POWDER, },
    [ITEM_ENERGY_ROOT]  = {  .dataID = 0x23,  .iconID = icon_energy_root_NCGR,   .paletteID = pltt_energy_root_NCLR,    .gen3ID = GBA_ITEM_ENERGY_ROOT,   },
    [ITEM_HEAL_POWDER]  = {  .dataID = 0x24,  .iconID = icon_energypowder_NCGR,  .paletteID = pltt_heal_powder_NCLR,    .gen3ID = GBA_ITEM_HEAL_POWDER,   },
    [ITEM_REVIVAL_HERB] = {  .dataID = 0x25,  .iconID = icon_revival_herb_NCGR,  .paletteID = pltt_revival_herb_NCLR,   .gen3ID = GBA_ITEM_REVIVAL_HERB,  },
    [ITEM_ETHER]        = {  .dataID = 0x26,  .iconID = icon_ether_NCGR,         .paletteID = pltt_ether_NCLR,          .gen3ID = GBA_ITEM_ETHER,         },
    [ITEM_MAX_ETHER]    = {  .dataID = 0x27,  .iconID = icon_ether_NCGR,         .paletteID = pltt_max_ether_NCLR,      .gen3ID = GBA_ITEM_MAX_ETHER,     },
    [ITEM_ELIXIR]       = {  .dataID = 0x28,  .iconID = icon_ether_NCGR,         .paletteID = pltt_elixir_NCLR,         .gen3ID = GBA_ITEM_ELIXIR,        },
    [ITEM_MAX_ELIXIR]   = {  .dataID = 0x29,  .iconID = icon_ether_NCGR,         .paletteID = pltt_max_elixir_NCLR,     .gen3ID = GBA_ITEM_MAX_ELIXIR,    },
    [ITEM_LAVA_COOKIE]  = {  .dataID = 0x2A,  .iconID = icon_lava_cookie_NCGR,   .paletteID = pltt_lava_cookie_NCLR,    .gen3ID = GBA_ITEM_LAVA_COOKIE,   },
    [ITEM_BERRY_JUICE]  = {  .dataID = 0x2B,  .iconID = icon_berry_juice_NCGR,   .paletteID = pltt_berry_juice_NCLR,    .gen3ID = GBA_ITEM_BERRY_JUICE,   },
    [ITEM_SACRED_ASH]   = {  .dataID = 0x2C,  .iconID = icon_sacred_ash_NCGR,    .paletteID = pltt_sacred_ash_NCLR,     .gen3ID = GBA_ITEM_SACRED_ASH,    },
    [ITEM_HP_UP]        = {  .dataID = 0x2D,  .iconID = icon_hp_up_NCGR,         .paletteID = pltt_hp_up_NCLR,          .gen3ID = GBA_ITEM_HP_UP,         },
    [ITEM_PROTEIN]      = {  .dataID = 0x2E,  .iconID = icon_vitamin_NCGR,       .paletteID = pltt_protein_NCLR,        .gen3ID = GBA_ITEM_PROTEIN,       },
    [ITEM_IRON]         = {  .dataID = 0x2F,  .iconID = icon_vitamin_NCGR,       .paletteID = pltt_iron_NCLR,           .gen3ID = GBA_ITEM_IRON,          },
    [ITEM_CARBOS]       = {  .dataID = 0x30,  .iconID = icon_vitamin_NCGR,       .paletteID = pltt_carbos_NCLR,         .gen3ID = GBA_ITEM_CARBOS,        },
    [ITEM_CALCIUM]      = {  .dataID = 0x31,  .iconID = icon_vitamin_NCGR,       .paletteID = pltt_calcium_NCLR,        .gen3ID = GBA_ITEM_CALCIUM,       },
    [ITEM_RARE_CANDY]   = {  .dataID = 0x32,  .iconID = icon_rare_candy_NCGR,    .paletteID = pltt_rare_candy_NCLR,     .gen3ID = GBA_ITEM_RARE_CANDY,    },
    [ITEM_PP_UP]        = {  .dataID = 0x33,  .iconID = icon_pp_up_NCGR,         .paletteID = pltt_pp_up_NCLR,          .gen3ID = GBA_ITEM_PP_UP,         },
    [ITEM_ZINC]         = {  .dataID = 0x34,  .iconID = icon_vitamin_NCGR,       .paletteID = pltt_zinc_NCLR,           .gen3ID = GBA_ITEM_ZINC,          },
    [ITEM_PP_MAX]       = {  .dataID = 0x35,  .iconID = icon_pp_max_NCGR,        .paletteID = pltt_pp_max_NCLR,         .gen3ID = GBA_ITEM_PP_MAX,        },
    [ITEM_OLD_GATEAU]   = {  .dataID = 0x36,  .iconID = icon_old_gateau_NCGR,    .paletteID = pltt_old_gateau_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_GUARD_SPEC]   = {  .dataID = 0x37,  .iconID = icon_battle_item_NCGR,   .paletteID = pltt_guard_spec_NCLR,     .gen3ID = GBA_ITEM_GUARD_SPEC,    },
    [ITEM_DIRE_HIT]     = {  .dataID = 0x38,  .iconID = icon_battle_item_NCGR,   .paletteID = pltt_dire_hit_NCLR,       .gen3ID = GBA_ITEM_DIRE_HIT,      },
    [ITEM_X_ATTACK]     = {  .dataID = 0x39,  .iconID = icon_battle_item_NCGR,   .paletteID = pltt_x_attack_NCLR,       .gen3ID = GBA_ITEM_X_ATTACK,      },
    [ITEM_X_DEFENSE]    = {  .dataID = 0x3A,  .iconID = icon_battle_item_NCGR,   .paletteID = pltt_x_defense_NCLR,      .gen3ID = GBA_ITEM_X_DEFEND,      },
    [ITEM_X_SPEED]      = {  .dataID = 0x3B,  .iconID = icon_battle_item_NCGR,   .paletteID = pltt_x_speed_NCLR,        .gen3ID = GBA_ITEM_X_SPEED,       },
    [ITEM_X_ACCURACY]   = {  .dataID = 0x3C,  .iconID = icon_battle_item_NCGR,   .paletteID = pltt_x_accuracy_NCLR,     .gen3ID = GBA_ITEM_X_ACCURACY,    },
    [ITEM_X_SPECIAL]    = {  .dataID = 0x3D,  .iconID = icon_battle_item_NCGR,   .paletteID = pltt_x_special_NCLR,      .gen3ID = GBA_ITEM_X_SPECIAL,     },
    [ITEM_X_SP_DEF]     = {  .dataID = 0x3E,  .iconID = icon_battle_item_NCGR,   .paletteID = pltt_x_sp_def_NCLR,       .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_POKE_DOLL]    = {  .dataID = 0x3F,  .iconID = icon_poke_doll_NCGR,     .paletteID = pltt_poke_doll_NCLR,      .gen3ID = GBA_ITEM_POKE_DOLL,     },
    [ITEM_FLUFFY_TAIL]  = {  .dataID = 0x40,  .iconID = icon_fluffy_tail_NCGR,   .paletteID = pltt_fluffy_tail_NCLR,    .gen3ID = GBA_ITEM_FLUFFY_TAIL,   },
    [ITEM_BLUE_FLUTE]   = {  .dataID = 0x41,  .iconID = icon_flute_NCGR,         .paletteID = pltt_blue_flute_NCLR,     .gen3ID = GBA_ITEM_BLUE_FLUTE,    },
    [ITEM_YELLOW_FLUTE] = {  .dataID = 0x42,  .iconID = icon_flute_NCGR,         .paletteID = pltt_yellow_flute_NCLR,   .gen3ID = GBA_ITEM_YELLOW_FLUTE,  },
    [ITEM_RED_FLUTE]    = {  .dataID = 0x43,  .iconID = icon_flute_NCGR,         .paletteID = pltt_red_flute_NCLR,      .gen3ID = GBA_ITEM_RED_FLUTE,     },
    [ITEM_BLACK_FLUTE]  = {  .dataID = 0x44,  .iconID = icon_flute_NCGR,         .paletteID = pltt_black_flute_NCLR,    .gen3ID = GBA_ITEM_BLACK_FLUTE,   },
    [ITEM_WHITE_FLUTE]  = {  .dataID = 0x45,  .iconID = icon_flute_NCGR,         .paletteID = pltt_white_flute_NCLR,    .gen3ID = GBA_ITEM_WHITE_FLUTE,   },
    [ITEM_SHOAL_SALT]   = {  .dataID = 0x46,  .iconID = icon_energypowder_NCGR,  .paletteID = pltt_shoal_salt_NCLR,     .gen3ID = GBA_ITEM_SHOAL_SALT,    },
    [ITEM_SHOAL_SHELL]  = {  .dataID = 0x47,  .iconID = icon_shoal_shell_NCGR,   .paletteID = pltt_shoal_shell_NCLR,    .gen3ID = GBA_ITEM_SHOAL_SHELL,   },
    [ITEM_RED_SHARD]    = {  .dataID = 0x48,  .iconID = icon_shard_NCGR,         .paletteID = pltt_red_shard_NCLR,      .gen3ID = GBA_ITEM_RED_SHARD,     },
    [ITEM_BLUE_SHARD]   = {  .dataID = 0x49,  .iconID = icon_shard_NCGR,         .paletteID = pltt_blue_shard_NCLR,     .gen3ID = GBA_ITEM_BLUE_SHARD,    },
    [ITEM_YELLOW_SHARD] = {  .dataID = 0x4A,  .iconID = icon_shard_NCGR,         .paletteID = pltt_yellow_shard_NCLR,   .gen3ID = GBA_ITEM_YELLOW_SHARD,  },
    [ITEM_GREEN_SHARD]  = {  .dataID = 0x4B,  .iconID = icon_shard_NCGR,         .paletteID = pltt_green_shard_NCLR,    .gen3ID = GBA_ITEM_GREEN_SHARD,   },
    [ITEM_SUPER_REPEL]  = {  .dataID = 0x4C,  .iconID = icon_repel_NCGR,         .paletteID = pltt_super_repel_NCLR,    .gen3ID = GBA_ITEM_SUPER_REPEL,   },
    [ITEM_MAX_REPEL]    = {  .dataID = 0x4D,  .iconID = icon_repel_NCGR,         .paletteID = pltt_max_repel_NCLR,      .gen3ID = GBA_ITEM_MAX_REPEL,     },
    [ITEM_ESCAPE_ROPE]  = {  .dataID = 0x4E,  .iconID = icon_escape_rope_NCGR,   .paletteID = pltt_escape_rope_NCLR,    .gen3ID = GBA_ITEM_ESCAPE_ROPE,   },
    [ITEM_REPEL]        = {  .dataID = 0x4F,  .iconID = icon_repel_NCGR,         .paletteID = pltt_repel_NCLR,          .gen3ID = GBA_ITEM_REPEL,         },
    [ITEM_SUN_STONE]    = {  .dataID = 0x50,  .iconID = icon_sun_stone_NCGR,     .paletteID = pltt_sun_stone_NCLR,      .gen3ID = GBA_ITEM_SUN_STONE,     },
    [ITEM_MOON_STONE]   = {  .dataID = 0x51,  .iconID = icon_moon_stone_NCGR,    .paletteID = pltt_moon_stone_NCLR,     .gen3ID = GBA_ITEM_MOON_STONE,    },
    [ITEM_FIRE_STONE]   = {  .dataID = 0x52,  .iconID = icon_fire_stone_NCGR,    .paletteID = pltt_fire_stone_NCLR,     .gen3ID = GBA_ITEM_FIRE_STONE,    },
    [ITEM_THUNDERSTONE] = {  .dataID = 0x53,  .iconID = icon_thunderstone_NCGR,  .paletteID = pltt_thunderstone_NCLR,   .gen3ID = GBA_ITEM_THUNDER_STONE, },
    [ITEM_WATER_STONE]  = {  .dataID = 0x54,  .iconID = icon_water_stone_NCGR,   .paletteID = pltt_water_stone_NCLR,    .gen3ID = GBA_ITEM_WATER_STONE,   },
    [ITEM_LEAF_STONE]   = {  .dataID = 0x55,  .iconID = icon_leaf_stone_NCGR,    .paletteID = pltt_leaf_stone_NCLR,     .gen3ID = GBA_ITEM_LEAF_STONE,    },
    [ITEM_TINYMUSHROOM] = {  .dataID = 0x56,  .iconID = icon_tinymushroom_NCGR,  .paletteID = pltt_mushroom_NCLR,       .gen3ID = GBA_ITEM_TINY_MUSHROOM, },
    [ITEM_BIG_MUSHROOM] = {  .dataID = 0x57,  .iconID = icon_big_mushroom_NCGR,  .paletteID = pltt_mushroom_NCLR,       .gen3ID = GBA_ITEM_BIG_MUSHROOM,  },
    [ITEM_PEARL]        = {  .dataID = 0x58,  .iconID = icon_pearl_NCGR,         .paletteID = pltt_pearl_NCLR,          .gen3ID = GBA_ITEM_PEARL,         },
    [ITEM_BIG_PEARL]    = {  .dataID = 0x59,  .iconID = icon_big_pearl_NCGR,     .paletteID = pltt_pearl_NCLR,          .gen3ID = GBA_ITEM_BIG_PEARL,     },
    [ITEM_STARDUST]     = {  .dataID = 0x5A,  .iconID = icon_stardust_NCGR,      .paletteID = pltt_stardust_NCLR,       .gen3ID = GBA_ITEM_STARDUST,      },
    [ITEM_STAR_PIECE]   = {  .dataID = 0x5B,  .iconID = icon_star_piece_NCGR,    .paletteID = pltt_stardust_NCLR,       .gen3ID = GBA_ITEM_STAR_PIECE,    },
    [ITEM_NUGGET]       = {  .dataID = 0x5C,  .iconID = icon_nugget_NCGR,        .paletteID = pltt_nugget_NCLR,         .gen3ID = GBA_ITEM_NUGGET,        },
    [ITEM_HEART_SCALE]  = {  .dataID = 0x5D,  .iconID = icon_heart_scale_NCGR,   .paletteID = pltt_heart_scale_NCLR,    .gen3ID = GBA_ITEM_HEART_SCALE,   },
    [ITEM_HONEY]        = {  .dataID = 0x5E,  .iconID = icon_honey_NCGR,         .paletteID = pltt_honey_NCLR,          .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_GROWTH_MULCH] = {  .dataID = 0x5F,  .iconID = icon_growth_mulch_NCGR,  .paletteID = pltt_growth_mulch_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_DAMP_MULCH]   = {  .dataID = 0x60,  .iconID = icon_damp_mulch_NCGR,    .paletteID = pltt_damp_mulch_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_STABLE_MULCH] = {  .dataID = 0x61,  .iconID = icon_stable_mulch_NCGR,  .paletteID = pltt_stable_mulch_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_GOOEY_MULCH]  = {  .dataID = 0x62,  .iconID = icon_gooey_mulch_NCGR,   .paletteID = pltt_gooey_mulch_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_ROOT_FOSSIL]  = {  .dataID = 0x63,  .iconID = icon_root_fossil_NCGR,   .paletteID = pltt_root_fossil_NCLR,    .gen3ID = GBA_ITEM_ROOT_FOSSIL,   },
    [ITEM_CLAW_FOSSIL]  = {  .dataID = 0x64,  .iconID = icon_claw_fossil_NCGR,   .paletteID = pltt_root_fossil_NCLR,    .gen3ID = GBA_ITEM_CLAW_FOSSIL,   },
    [ITEM_HELIX_FOSSIL] = {  .dataID = 0x65,  .iconID = icon_helix_fossil_NCGR,  .paletteID = pltt_helix_fossil_NCLR,   .gen3ID = GBA_ITEM_HELIX_FOSSIL,  },
    [ITEM_DOME_FOSSIL]  = {  .dataID = 0x66,  .iconID = icon_dome_fossil_NCGR,   .paletteID = pltt_helix_fossil_NCLR,   .gen3ID = GBA_ITEM_DOME_FOSSIL,   },
    [ITEM_OLD_AMBER]    = {  .dataID = 0x67,  .iconID = icon_old_amber_NCGR,     .paletteID = pltt_old_amber_NCLR,      .gen3ID = GBA_ITEM_OLD_AMBER,     },
    [ITEM_ARMOR_FOSSIL] = {  .dataID = 0x68,  .iconID = icon_armor_fossil_NCGR,  .paletteID = pltt_armor_fossil_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_SKULL_FOSSIL] = {  .dataID = 0x69,  .iconID = icon_skull_fossil_NCGR,  .paletteID = pltt_skull_fossil_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_RARE_BONE]    = {  .dataID = 0x6A,  .iconID = icon_bone_NCGR,          .paletteID = pltt_bone_NCLR,           .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_SHINY_STONE]  = {  .dataID = 0x6B,  .iconID = icon_shiny_stone_NCGR,   .paletteID = pltt_shiny_stone_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_DUSK_STONE]   = {  .dataID = 0x6C,  .iconID = icon_dusk_stone_NCGR,    .paletteID = pltt_dusk_stone_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_DAWN_STONE]   = {  .dataID = 0x6D,  .iconID = icon_dawn_stone_NCGR,    .paletteID = pltt_dawn_stone_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_OVAL_STONE]   = {  .dataID = 0x6E,  .iconID = icon_oval_stone_NCGR,    .paletteID = pltt_oval_stone_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_ODD_KEYSTONE] = {  .dataID = 0x6F,  .iconID = icon_odd_keystone_NCGR,  .paletteID = pltt_odd_keystone_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_GRISEOUS_ORB] = {  .dataID = 0x70,  .iconID = icon_griseous_orb_NCGR,  .paletteID = pltt_griseous_orb_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_UNUSED_113]   = {  .dataID = 0x0,   .iconID = icon_unused_NCGR,        .paletteID = pltt_unused_NCLR,         .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_UNUSED_114]   = {  .dataID = 0x0,   .iconID = icon_unused_NCGR,        .paletteID = pltt_unused_NCLR,         .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_UNUSED_115]   = {  .dataID = 0x0,   .iconID = icon_unused_NCGR,        .paletteID = pltt_unused_NCLR,         .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_UNUSED_116]   = {  .dataID = 0x0,   .iconID = icon_unused_NCGR,        .paletteID = pltt_unused_NCLR,         .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_UNUSED_117]   = {  .dataID = 0x0,   .iconID = icon_unused_NCGR,        .paletteID = pltt_unused_NCLR,         .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_UNUSED_118]   = {  .dataID = 0x0,   .iconID = icon_unused_NCGR,        .paletteID = pltt_unused_NCLR,         .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_UNUSED_119]   = {  .dataID = 0x0,   .iconID = icon_unused_NCGR,        .paletteID = pltt_unused_NCLR,         .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_UNUSED_120]   = {  .dataID = 0x0,   .iconID = icon_unused_NCGR,        .paletteID = pltt_unused_NCLR,         .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_UNUSED_121]   = {  .dataID = 0x0,   .iconID = icon_unused_NCGR,        .paletteID = pltt_unused_NCLR,         .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_UNUSED_122]   = {  .dataID = 0x0,   .iconID = icon_unused_NCGR,        .paletteID = pltt_unused_NCLR,         .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_UNUSED_123]   = {  .dataID = 0x0,   .iconID = icon_unused_NCGR,        .paletteID = pltt_unused_NCLR,         .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_UNUSED_124]   = {  .dataID = 0x0,   .iconID = icon_unused_NCGR,        .paletteID = pltt_unused_NCLR,         .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_UNUSED_125]   = {  .dataID = 0x0,   .iconID = icon_unused_NCGR,        .paletteID = pltt_unused_NCLR,         .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_UNUSED_126]   = {  .dataID = 0x0,   .iconID = icon_unused_NCGR,        .paletteID = pltt_unused_NCLR,         .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_UNUSED_127]   = {  .dataID = 0x0,   .iconID = icon_unused_NCGR,        .paletteID = pltt_unused_NCLR,         .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_UNUSED_128]   = {  .dataID = 0x0,   .iconID = icon_unused_NCGR,        .paletteID = pltt_unused_NCLR,         .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_UNUSED_129]   = {  .dataID = 0x0,   .iconID = icon_unused_NCGR,        .paletteID = pltt_unused_NCLR,         .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_UNUSED_130]   = {  .dataID = 0x0,   .iconID = icon_unused_NCGR,        .paletteID = pltt_unused_NCLR,         .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_UNUSED_131]   = {  .dataID = 0x0,   .iconID = icon_unused_NCGR,        .paletteID = pltt_unused_NCLR,         .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_UNUSED_132]   = {  .dataID = 0x0,   .iconID = icon_unused_NCGR,        .paletteID = pltt_unused_NCLR,         .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_UNUSED_133]   = {  .dataID = 0x0,   .iconID = icon_unused_NCGR,        .paletteID = pltt_unused_NCLR,         .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_UNUSED_134]   = {  .dataID = 0x0,   .iconID = icon_unused_NCGR,        .paletteID = pltt_unused_NCLR,         .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_ADAMANT_ORB]  = {  .dataID = 0x71,  .iconID = icon_adamant_orb_NCGR,   .paletteID = pltt_adamant_orb_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_LUSTROUS_ORB] = {  .dataID = 0x72,  .iconID = icon_lustrous_orb_NCGR,  .paletteID = pltt_lustrous_orb_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_GRASS_MAIL]   = {  .dataID = 0x73,  .iconID = icon_grass_mail_NCGR,    .paletteID = pltt_grass_mail_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_FLAME_MAIL]   = {  .dataID = 0x74,  .iconID = icon_flame_mail_NCGR,    .paletteID = pltt_flame_mail_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_BUBBLE_MAIL]  = {  .dataID = 0x75,  .iconID = icon_bubble_mail_NCGR,   .paletteID = pltt_bubble_mail_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_BLOOM_MAIL]   = {  .dataID = 0x76,  .iconID = icon_bloom_mail_NCGR,    .paletteID = pltt_bloom_mail_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TUNNEL_MAIL]  = {  .dataID = 0x77,  .iconID = icon_tunnel_mail_NCGR,   .paletteID = pltt_tunnel_mail_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_STEEL_MAIL]   = {  .dataID = 0x78,  .iconID = icon_steel_mail_NCGR,    .paletteID = pltt_steel_mail_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_HEART_MAIL]   = {  .dataID = 0x79,  .iconID = icon_heart_mail_NCGR,    .paletteID = pltt_heart_mail_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_SNOW_MAIL]    = {  .dataID = 0x7A,  .iconID = icon_snow_mail_NCGR,     .paletteID = pltt_snow_mail_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_SPACE_MAIL]   = {  .dataID = 0x7B,  .iconID = icon_space_mail_NCGR,    .paletteID = pltt_space_mail_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_AIR_MAIL]     = {  .dataID = 0x7C,  .iconID = icon_air_mail_NCGR,      .paletteID = pltt_air_mail_NCLR,       .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_MOSAIC_MAIL]  = {  .dataID = 0x7D,  .iconID = icon_mosaic_mail_NCGR,   .paletteID = pltt_mosaic_mail_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_BRICK_MAIL]   = {  .dataID = 0x7E,  .iconID = icon_brick_mail_NCGR,    .paletteID = pltt_brick_mail_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_CHERI_BERRY]  = {  .dataID = 0x7F,  .iconID = icon_cheri_berry_NCGR,   .paletteID = pltt_cheri_berry_NCLR,    .gen3ID = GBA_ITEM_CHERI_BERRY,   },
    [ITEM_CHESTO_BERRY] = {  .dataID = 0x80,  .iconID = icon_chesto_berry_NCGR,  .paletteID = pltt_chesto_berry_NCLR,   .gen3ID = GBA_ITEM_CHESTO_BERRY,  },
    [ITEM_PECHA_BERRY]  = {  .dataID = 0x81,  .iconID = icon_pecha_berry_NCGR,   .paletteID = pltt_pecha_berry_NCLR,    .gen3ID = GBA_ITEM_PECHA_BERRY,   },
    [ITEM_RAWST_BERRY]  = {  .dataID = 0x82,  .iconID = icon_rawst_berry_NCGR,   .paletteID = pltt_rawst_berry_NCLR,    .gen3ID = GBA_ITEM_RAWST_BERRY,   },
    [ITEM_ASPEAR_BERRY] = {  .dataID = 0x83,  .iconID = icon_aspear_berry_NCGR,  .paletteID = pltt_aspear_berry_NCLR,   .gen3ID = GBA_ITEM_ASPEAR_BERRY,  },
    [ITEM_LEPPA_BERRY]  = {  .dataID = 0x84,  .iconID = icon_leppa_berry_NCGR,   .paletteID = pltt_leppa_berry_NCLR,    .gen3ID = GBA_ITEM_LEPPA_BERRY,   },
    [ITEM_ORAN_BERRY]   = {  .dataID = 0x85,  .iconID = icon_oran_berry_NCGR,    .paletteID = pltt_oran_berry_NCLR,     .gen3ID = GBA_ITEM_ORAN_BERRY,    },
    [ITEM_PERSIM_BERRY] = {  .dataID = 0x86,  .iconID = icon_persim_berry_NCGR,  .paletteID = pltt_persim_berry_NCLR,   .gen3ID = GBA_ITEM_PERSIM_BERRY,  },
    [ITEM_LUM_BERRY]    = {  .dataID = 0x87,  .iconID = icon_lum_berry_NCGR,     .paletteID = pltt_lum_berry_NCLR,      .gen3ID = GBA_ITEM_LUM_BERRY,     },
    [ITEM_SITRUS_BERRY] = {  .dataID = 0x88,  .iconID = icon_sitrus_berry_NCGR,  .paletteID = pltt_sitrus_berry_NCLR,   .gen3ID = GBA_ITEM_SITRUS_BERRY,  },
    [ITEM_FIGY_BERRY]   = {  .dataID = 0x89,  .iconID = icon_figy_berry_NCGR,    .paletteID = pltt_figy_berry_NCLR,     .gen3ID = GBA_ITEM_FIGY_BERRY,    },
    [ITEM_WIKI_BERRY]   = {  .dataID = 0x8A,  .iconID = icon_wiki_berry_NCGR,    .paletteID = pltt_wiki_berry_NCLR,     .gen3ID = GBA_ITEM_WIKI_BERRY,    },
    [ITEM_MAGO_BERRY]   = {  .dataID = 0x8B,  .iconID = icon_mago_berry_NCGR,    .paletteID = pltt_mago_berry_NCLR,     .gen3ID = GBA_ITEM_MAGO_BERRY,    },
    [ITEM_AGUAV_BERRY]  = {  .dataID = 0x8C,  .iconID = icon_aguav_berry_NCGR,   .paletteID = pltt_aguav_berry_NCLR,    .gen3ID = GBA_ITEM_AGUAV_BERRY,   },
    [ITEM_IAPAPA_BERRY] = {  .dataID = 0x8D,  .iconID = icon_iapapa_berry_NCGR,  .paletteID = pltt_iapapa_berry_NCLR,   .gen3ID = GBA_ITEM_IAPAPA_BERRY,  },
    [ITEM_RAZZ_BERRY]   = {  .dataID = 0x8E,  .iconID = icon_razz_berry_NCGR,    .paletteID = pltt_razz_berry_NCLR,     .gen3ID = GBA_ITEM_RAZZ_BERRY,    },
    [ITEM_BLUK_BERRY]   = {  .dataID = 0x8F,  .iconID = icon_bluk_berry_NCGR,    .paletteID = pltt_bluk_berry_NCLR,     .gen3ID = GBA_ITEM_BLUK_BERRY,    },
    [ITEM_NANAB_BERRY]  = {  .dataID = 0x90,  .iconID = icon_nanab_berry_NCGR,   .paletteID = pltt_nanab_berry_NCLR,    .gen3ID = GBA_ITEM_NANAB_BERRY,   },
    [ITEM_WEPEAR_BERRY] = {  .dataID = 0x91,  .iconID = icon_wepear_berry_NCGR,  .paletteID = pltt_wepear_berry_NCLR,   .gen3ID = GBA_ITEM_WEPEAR_BERRY,  },
    [ITEM_PINAP_BERRY]  = {  .dataID = 0x92,  .iconID = icon_pinap_berry_NCGR,   .paletteID = pltt_pinap_berry_NCLR,    .gen3ID = GBA_ITEM_PINAP_BERRY,   },
    [ITEM_POMEG_BERRY]  = {  .dataID = 0x93,  .iconID = icon_pomeg_berry_NCGR,   .paletteID = pltt_pomeg_berry_NCLR,    .gen3ID = GBA_ITEM_POMEG_BERRY,   },
    [ITEM_KELPSY_BERRY] = {  .dataID = 0x94,  .iconID = icon_kelpsy_berry_NCGR,  .paletteID = pltt_kelpsy_berry_NCLR,   .gen3ID = GBA_ITEM_KELPSY_BERRY,  },
    [ITEM_QUALOT_BERRY] = {  .dataID = 0x95,  .iconID = icon_qualot_berry_NCGR,  .paletteID = pltt_qualot_berry_NCLR,   .gen3ID = GBA_ITEM_QUALOT_BERRY,  },
    [ITEM_HONDEW_BERRY] = {  .dataID = 0x96,  .iconID = icon_hondew_berry_NCGR,  .paletteID = pltt_hondew_berry_NCLR,   .gen3ID = GBA_ITEM_HONDEW_BERRY,  },
    [ITEM_GREPA_BERRY]  = {  .dataID = 0x97,  .iconID = icon_grepa_berry_NCGR,   .paletteID = pltt_grepa_berry_NCLR,    .gen3ID = GBA_ITEM_GREPA_BERRY,   },
    [ITEM_TAMATO_BERRY] = {  .dataID = 0x98,  .iconID = icon_tamato_berry_NCGR,  .paletteID = pltt_tamato_berry_NCLR,   .gen3ID = GBA_ITEM_TAMATO_BERRY,  },
    [ITEM_CORNN_BERRY]  = {  .dataID = 0x99,  .iconID = icon_cornn_berry_NCGR,   .paletteID = pltt_cornn_berry_NCLR,    .gen3ID = GBA_ITEM_CORNN_BERRY,   },
    [ITEM_MAGOST_BERRY] = {  .dataID = 0x9A,  .iconID = icon_magost_berry_NCGR,  .paletteID = pltt_magost_berry_NCLR,   .gen3ID = GBA_ITEM_MAGOST_BERRY,  },
    [ITEM_RABUTA_BERRY] = {  .dataID = 0x9B,  .iconID = icon_rabuta_berry_NCGR,  .paletteID = pltt_rabuta_berry_NCLR,   .gen3ID = GBA_ITEM_RABUTA_BERRY,  },
    [ITEM_NOMEL_BERRY]  = {  .dataID = 0x9C,  .iconID = icon_nomel_berry_NCGR,   .paletteID = pltt_nomel_berry_NCLR,    .gen3ID = GBA_ITEM_NOMEL_BERRY,   },
    [ITEM_SPELON_BERRY] = {  .dataID = 0x9D,  .iconID = icon_spelon_berry_NCGR,  .paletteID = pltt_spelon_berry_NCLR,   .gen3ID = GBA_ITEM_SPELON_BERRY,  },
    [ITEM_PAMTRE_BERRY] = {  .dataID = 0x9E,  .iconID = icon_pamtre_berry_NCGR,  .paletteID = pltt_pamtre_berry_NCLR,   .gen3ID = GBA_ITEM_PAMTRE_BERRY,  },
    [ITEM_WATMEL_BERRY] = {  .dataID = 0x9F,  .iconID = icon_watmel_berry_NCGR,  .paletteID = pltt_watmel_berry_NCLR,   .gen3ID = GBA_ITEM_WATMEL_BERRY,  },
    [ITEM_DURIN_BERRY]  = {  .dataID = 0xA0,  .iconID = icon_durin_berry_NCGR,   .paletteID = pltt_durin_berry_NCLR,    .gen3ID = GBA_ITEM_DURIN_BERRY,   },
    [ITEM_BELUE_BERRY]  = {  .dataID = 0xA1,  .iconID = icon_belue_berry_NCGR,   .paletteID = pltt_belue_berry_NCLR,    .gen3ID = GBA_ITEM_BELUE_BERRY,   },
    [ITEM_OCCA_BERRY]   = {  .dataID = 0xA2,  .iconID = icon_occa_berry_NCGR,    .paletteID = pltt_occa_berry_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_PASSHO_BERRY] = {  .dataID = 0xA3,  .iconID = icon_passho_berry_NCGR,  .paletteID = pltt_passho_berry_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_WACAN_BERRY]  = {  .dataID = 0xA4,  .iconID = icon_wacan_berry_NCGR,   .paletteID = pltt_wacan_berry_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_RINDO_BERRY]  = {  .dataID = 0xA5,  .iconID = icon_rindo_berry_NCGR,   .paletteID = pltt_rindo_berry_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_YACHE_BERRY]  = {  .dataID = 0xA6,  .iconID = icon_yache_berry_NCGR,   .paletteID = pltt_yache_berry_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_CHOPLE_BERRY] = {  .dataID = 0xA7,  .iconID = icon_chople_berry_NCGR,  .paletteID = pltt_chople_berry_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_KEBIA_BERRY]  = {  .dataID = 0xA8,  .iconID = icon_kebia_berry_NCGR,   .paletteID = pltt_kebia_berry_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_SHUCA_BERRY]  = {  .dataID = 0xA9,  .iconID = icon_shuca_berry_NCGR,   .paletteID = pltt_shuca_berry_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_COBA_BERRY]   = {  .dataID = 0xAA,  .iconID = icon_coba_berry_NCGR,    .paletteID = pltt_coba_berry_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_PAYAPA_BERRY] = {  .dataID = 0xAB,  .iconID = icon_payapa_berry_NCGR,  .paletteID = pltt_payapa_berry_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TANGA_BERRY]  = {  .dataID = 0xAC,  .iconID = icon_tanga_berry_NCGR,   .paletteID = pltt_tanga_berry_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_CHARTI_BERRY] = {  .dataID = 0xAD,  .iconID = icon_charti_berry_NCGR,  .paletteID = pltt_charti_berry_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_KASIB_BERRY]  = {  .dataID = 0xAE,  .iconID = icon_kasib_berry_NCGR,   .paletteID = pltt_kasib_berry_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_HABAN_BERRY]  = {  .dataID = 0xAF,  .iconID = icon_haban_berry_NCGR,   .paletteID = pltt_haban_berry_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_COLBUR_BERRY] = {  .dataID = 0xB0,  .iconID = icon_colbur_berry_NCGR,  .paletteID = pltt_colbur_berry_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_BABIRI_BERRY] = {  .dataID = 0xB1,  .iconID = icon_babiri_berry_NCGR,  .paletteID = pltt_babiri_berry_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_CHILAN_BERRY] = {  .dataID = 0xB2,  .iconID = icon_chilan_berry_NCGR,  .paletteID = pltt_chilan_berry_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_LIECHI_BERRY] = {  .dataID = 0xB3,  .iconID = icon_liechi_berry_NCGR,  .paletteID = pltt_liechi_berry_NCLR,   .gen3ID = GBA_ITEM_LIECHI_BERRY,  },
    [ITEM_GANLON_BERRY] = {  .dataID = 0xB4,  .iconID = icon_ganlon_berry_NCGR,  .paletteID = pltt_ganlon_berry_NCLR,   .gen3ID = GBA_ITEM_GANLON_BERRY,  },
    [ITEM_SALAC_BERRY]  = {  .dataID = 0xB5,  .iconID = icon_salac_berry_NCGR,   .paletteID = pltt_salac_berry_NCLR,    .gen3ID = GBA_ITEM_SALAC_BERRY,   },
    [ITEM_PETAYA_BERRY] = {  .dataID = 0xB6,  .iconID = icon_petaya_berry_NCGR,  .paletteID = pltt_petaya_berry_NCLR,   .gen3ID = GBA_ITEM_PETAYA_BERRY,  },
    [ITEM_APICOT_BERRY] = {  .dataID = 0xB7,  .iconID = icon_apicot_berry_NCGR,  .paletteID = pltt_apicot_berry_NCLR,   .gen3ID = GBA_ITEM_APICOT_BERRY,  },
    [ITEM_LANSAT_BERRY] = {  .dataID = 0xB8,  .iconID = icon_lansat_berry_NCGR,  .paletteID = pltt_lansat_berry_NCLR,   .gen3ID = GBA_ITEM_LANSAT_BERRY,  },
    [ITEM_STARF_BERRY]  = {  .dataID = 0xB9,  .iconID = icon_starf_berry_NCGR,   .paletteID = pltt_starf_berry_NCLR,    .gen3ID = GBA_ITEM_STARF_BERRY,   },
    [ITEM_ENIGMA_BERRY] = {  .dataID = 0xBA,  .iconID = icon_enigma_berry_NCGR,  .paletteID = pltt_enigma_berry_NCLR,   .gen3ID = GBA_ITEM_ENIGMA_BERRY,  },
    [ITEM_MICLE_BERRY]  = {  .dataID = 0xBB,  .iconID = icon_micle_berry_NCGR,   .paletteID = pltt_micle_berry_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_CUSTAP_BERRY] = {  .dataID = 0xBC,  .iconID = icon_custap_berry_NCGR,  .paletteID = pltt_custap_berry_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_JABOCA_BERRY] = {  .dataID = 0xBD,  .iconID = icon_jaboca_berry_NCGR,  .paletteID = pltt_jaboca_berry_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_ROWAP_BERRY]  = {  .dataID = 0xBE,  .iconID = icon_rowap_berry_NCGR,   .paletteID = pltt_rowap_berry_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_BRIGHTPOWDER] = {  .dataID = 0xBF,  .iconID = icon_brightpowder_NCGR,  .paletteID = pltt_brightpowder_NCLR,   .gen3ID = GBA_ITEM_BRIGHT_POWDER, },
    [ITEM_WHITE_HERB]   = {  .dataID = 0xC0,  .iconID = icon_herb_NCGR,          .paletteID = pltt_white_herb_NCLR,     .gen3ID = GBA_ITEM_WHITE_HERB,    },
    [ITEM_MACHO_BRACE]  = {  .dataID = 0xC1,  .iconID = icon_macho_brace_NCGR,   .paletteID = pltt_macho_brace_NCLR,    .gen3ID = GBA_ITEM_MACHO_BRACE,   },
    [ITEM_EXP_SHARE]    = {  .dataID = 0xC2,  .iconID = icon_exp_share_NCGR,     .paletteID = pltt_exp_share_NCLR,      .gen3ID = GBA_ITEM_EXP_SHARE,     },
    [ITEM_QUICK_CLAW]   = {  .dataID = 0xC3,  .iconID = icon_quick_claw_NCGR,    .paletteID = pltt_quick_claw_NCLR,     .gen3ID = GBA_ITEM_QUICK_CLAW,    },
    [ITEM_SOOTHE_BELL]  = {  .dataID = 0xC4,  .iconID = icon_soothe_bell_NCGR,   .paletteID = pltt_soothe_bell_NCLR,    .gen3ID = GBA_ITEM_SOOTHE_BELL,   },
    [ITEM_MENTAL_HERB]  = {  .dataID = 0xC5,  .iconID = icon_herb_NCGR,          .paletteID = pltt_mental_herb_NCLR,    .gen3ID = GBA_ITEM_MENTAL_HERB,   },
    [ITEM_CHOICE_BAND]  = {  .dataID = 0xC6,  .iconID = icon_choice_band_NCGR,   .paletteID = pltt_choice_band_NCLR,    .gen3ID = GBA_ITEM_CHOICE_BAND,   },
    [ITEM_KINGS_ROCK]   = {  .dataID = 0xC7,  .iconID = icon_kings_rock_NCGR,    .paletteID = pltt_kings_rock_NCLR,     .gen3ID = GBA_ITEM_KINGS_ROCK,    },
    [ITEM_SILVERPOWDER] = {  .dataID = 0xC8,  .iconID = icon_silverpowder_NCGR,  .paletteID = pltt_silverpowder_NCLR,   .gen3ID = GBA_ITEM_SILVER_POWDER, },
    [ITEM_AMULET_COIN]  = {  .dataID = 0xC9,  .iconID = icon_amulet_coin_NCGR,   .paletteID = pltt_amulet_coin_NCLR,    .gen3ID = GBA_ITEM_AMULET_COIN,   },
    [ITEM_CLEANSE_TAG]  = {  .dataID = 0xCA,  .iconID = icon_cleanse_tag_NCGR,   .paletteID = pltt_cleanse_tag_NCLR,    .gen3ID = GBA_ITEM_CLEANSE_TAG,   },
    [ITEM_SOUL_DEW]     = {  .dataID = 0xCB,  .iconID = icon_soul_dew_NCGR,      .paletteID = pltt_soul_dew_NCLR,       .gen3ID = GBA_ITEM_SOUL_DEW,      },
    [ITEM_DEEPSEATOOTH] = {  .dataID = 0xCC,  .iconID = icon_deepseatooth_NCGR,  .paletteID = pltt_deepseatooth_NCLR,   .gen3ID = GBA_ITEM_DEEP_SEA_TOOTH,},
    [ITEM_DEEPSEASCALE] = {  .dataID = 0xCD,  .iconID = icon_deepseascale_NCGR,  .paletteID = pltt_deepseascale_NCLR,   .gen3ID = GBA_ITEM_DEEP_SEA_SCALE,},
    [ITEM_SMOKE_BALL]   = {  .dataID = 0xCE,  .iconID = icon_smoke_ball_NCGR,    .paletteID = pltt_smoke_ball_NCLR,     .gen3ID = GBA_ITEM_SMOKE_BALL,    },
    [ITEM_EVERSTONE]    = {  .dataID = 0xCF,  .iconID = icon_everstone_NCGR,     .paletteID = pltt_everstone_NCLR,      .gen3ID = GBA_ITEM_EVERSTONE,     },
    [ITEM_FOCUS_BAND]   = {  .dataID = 0xD0,  .iconID = icon_focus_band_NCGR,    .paletteID = pltt_focus_band_NCLR,     .gen3ID = GBA_ITEM_FOCUS_BAND,    },
    [ITEM_LUCKY_EGG]    = {  .dataID = 0xD1,  .iconID = icon_lucky_egg_NCGR,     .paletteID = pltt_lucky_egg_NCLR,      .gen3ID = GBA_ITEM_LUCKY_EGG,     },
    [ITEM_SCOPE_LENS]   = {  .dataID = 0xD2,  .iconID = icon_scope_lens_NCGR,    .paletteID = pltt_scope_lens_NCLR,     .gen3ID = GBA_ITEM_SCOPE_LENS,    },
    [ITEM_METAL_COAT]   = {  .dataID = 0xD3,  .iconID = icon_metal_coat_NCGR,    .paletteID = pltt_metal_coat_NCLR,     .gen3ID = GBA_ITEM_METAL_COAT,    },
    [ITEM_LEFTOVERS]    = {  .dataID = 0xD4,  .iconID = icon_leftovers_NCGR,     .paletteID = pltt_leftovers_NCLR,      .gen3ID = GBA_ITEM_LEFTOVERS,     },
    [ITEM_DRAGON_SCALE] = {  .dataID = 0xD5,  .iconID = icon_dragon_scale_NCGR,  .paletteID = pltt_dragon_scale_NCLR,   .gen3ID = GBA_ITEM_DRAGON_SCALE,  },
    [ITEM_LIGHT_BALL]   = {  .dataID = 0xD6,  .iconID = icon_light_ball_NCGR,    .paletteID = pltt_light_ball_NCLR,     .gen3ID = GBA_ITEM_LIGHT_BALL,    },
    [ITEM_SOFT_SAND]    = {  .dataID = 0xD7,  .iconID = icon_soft_sand_NCGR,     .paletteID = pltt_soft_sand_NCLR,      .gen3ID = GBA_ITEM_SOFT_SAND,     },
    [ITEM_HARD_STONE]   = {  .dataID = 0xD8,  .iconID = icon_hard_stone_NCGR,    .paletteID = pltt_hard_stone_NCLR,     .gen3ID = GBA_ITEM_HARD_STONE,    },
    [ITEM_MIRACLE_SEED] = {  .dataID = 0xD9,  .iconID = icon_miracle_seed_NCGR,  .paletteID = pltt_miracle_seed_NCLR,   .gen3ID = GBA_ITEM_MIRACLE_SEED,  },
    [ITEM_BLACKGLASSES] = {  .dataID = 0xDA,  .iconID = icon_blackglasses_NCGR,  .paletteID = pltt_blackglasses_NCLR,   .gen3ID = GBA_ITEM_BLACK_GLASSES, },
    [ITEM_BLACK_BELT]   = {  .dataID = 0xDB,  .iconID = icon_black_belt_NCGR,    .paletteID = pltt_blackglasses_NCLR,   .gen3ID = GBA_ITEM_BLACK_BELT,    },
    [ITEM_MAGNET]       = {  .dataID = 0xDC,  .iconID = icon_magnet_NCGR,        .paletteID = pltt_magnet_NCLR,         .gen3ID = GBA_ITEM_MAGNET,        },
    [ITEM_MYSTIC_WATER] = {  .dataID = 0xDD,  .iconID = icon_mystic_water_NCGR,  .paletteID = pltt_mystic_water_NCLR,   .gen3ID = GBA_ITEM_MYSTIC_WATER,  },
    [ITEM_SHARP_BEAK]   = {  .dataID = 0xDE,  .iconID = icon_sharp_beak_NCGR,    .paletteID = pltt_sharp_beak_NCLR,     .gen3ID = GBA_ITEM_SHARP_BEAK,    },
    [ITEM_POISON_BARB]  = {  .dataID = 0xDF,  .iconID = icon_poison_barb_NCGR,   .paletteID = pltt_poison_barb_NCLR,    .gen3ID = GBA_ITEM_POISON_BARB,   },
    [ITEM_NEVERMELTICE] = {  .dataID = 0xE0,  .iconID = icon_nevermeltice_NCGR,  .paletteID = pltt_nevermeltice_NCLR,   .gen3ID = GBA_ITEM_NEVER_MELT_ICE,},
    [ITEM_SPELL_TAG]    = {  .dataID = 0xE1,  .iconID = icon_spell_tag_NCGR,     .paletteID = pltt_spell_tag_NCLR,      .gen3ID = GBA_ITEM_SPELL_TAG,     },
    [ITEM_TWISTEDSPOON] = {  .dataID = 0xE2,  .iconID = icon_twistedspoon_NCGR,  .paletteID = pltt_twistedspoon_NCLR,   .gen3ID = GBA_ITEM_TWISTED_SPOON, },
    [ITEM_CHARCOAL]     = {  .dataID = 0xE3,  .iconID = icon_charcoal_NCGR,      .paletteID = pltt_charcoal_NCLR,       .gen3ID = GBA_ITEM_CHARCOAL,      },
    [ITEM_DRAGON_FANG]  = {  .dataID = 0xE4,  .iconID = icon_dragon_fang_NCGR,   .paletteID = pltt_dragon_fang_NCLR,    .gen3ID = GBA_ITEM_DRAGON_FANG,   },
    [ITEM_SILK_SCARF]   = {  .dataID = 0xE5,  .iconID = icon_silk_scarf_NCGR,    .paletteID = pltt_silk_scarf_NCLR,     .gen3ID = GBA_ITEM_SILK_SCARF,    },
    [ITEM_UPGRADE]      = {  .dataID = 0xE6,  .iconID = icon_upgrade_NCGR,       .paletteID = pltt_upgrade_NCLR,        .gen3ID = GBA_ITEM_UP_GRADE,      },
    [ITEM_SHELL_BELL]   = {  .dataID = 0xE7,  .iconID = icon_shell_bell_NCGR,    .paletteID = pltt_shoal_shell_NCLR,    .gen3ID = GBA_ITEM_SHELL_BELL,    },
    [ITEM_SEA_INCENSE]  = {  .dataID = 0xE8,  .iconID = icon_sea_incense_NCGR,   .paletteID = pltt_sea_incense_NCLR,    .gen3ID = GBA_ITEM_SEA_INCENSE,   },
    [ITEM_LAX_INCENSE]  = {  .dataID = 0xE9,  .iconID = icon_lax_incense_NCGR,   .paletteID = pltt_lax_incense_NCLR,    .gen3ID = GBA_ITEM_LAX_INCENSE,   },
    [ITEM_LUCKY_PUNCH]  = {  .dataID = 0xEA,  .iconID = icon_lucky_punch_NCGR,   .paletteID = pltt_lucky_punch_NCLR,    .gen3ID = GBA_ITEM_LUCKY_PUNCH,   },
    [ITEM_METAL_POWDER] = {  .dataID = 0xEB,  .iconID = icon_powder_NCGR,        .paletteID = pltt_metal_powder_NCLR,   .gen3ID = GBA_ITEM_METAL_POWDER,  },
    [ITEM_THICK_CLUB]   = {  .dataID = 0xEC,  .iconID = icon_bone_NCGR,          .paletteID = pltt_bone_NCLR,           .gen3ID = GBA_ITEM_THICK_CLUB,    },
    [ITEM_STICK]        = {  .dataID = 0xED,  .iconID = icon_stick_NCGR,         .paletteID = pltt_stick_NCLR,          .gen3ID = GBA_ITEM_STICK,         },
    [ITEM_RED_SCARF]    = {  .dataID = 0xEE,  .iconID = icon_scarf_NCGR,         .paletteID = pltt_red_scarf_NCLR,      .gen3ID = GBA_ITEM_RED_SCARF,     },
    [ITEM_BLUE_SCARF]   = {  .dataID = 0xEF,  .iconID = icon_scarf_NCGR,         .paletteID = pltt_blue_scarf_NCLR,     .gen3ID = GBA_ITEM_BLUE_SCARF,    },
    [ITEM_PINK_SCARF]   = {  .dataID = 0xF0,  .iconID = icon_scarf_NCGR,         .paletteID = pltt_pink_scarf_NCLR,     .gen3ID = GBA_ITEM_PINK_SCARF,    },
    [ITEM_GREEN_SCARF]  = {  .dataID = 0xF1,  .iconID = icon_scarf_NCGR,         .paletteID = pltt_green_scarf_NCLR,    .gen3ID = GBA_ITEM_GREEN_SCARF,   },
    [ITEM_YELLOW_SCARF] = {  .dataID = 0xF2,  .iconID = icon_scarf_NCGR,         .paletteID = pltt_yellow_scarf_NCLR,   .gen3ID = GBA_ITEM_YELLOW_SCARF,  },
    [ITEM_WIDE_LENS]    = {  .dataID = 0xF3,  .iconID = icon_wide_lens_NCGR,     .paletteID = pltt_wide_lens_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_MUSCLE_BAND]  = {  .dataID = 0xF4,  .iconID = icon_muscle_band_NCGR,   .paletteID = pltt_muscle_band_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_WISE_GLASSES] = {  .dataID = 0xF5,  .iconID = icon_wise_glasses_NCGR,  .paletteID = pltt_wise_glasses_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_EXPERT_BELT]  = {  .dataID = 0xF6,  .iconID = icon_expert_belt_NCGR,   .paletteID = pltt_expert_belt_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_LIGHT_CLAY]   = {  .dataID = 0xF7,  .iconID = icon_light_clay_NCGR,    .paletteID = pltt_light_clay_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_LIFE_ORB]     = {  .dataID = 0xF8,  .iconID = icon_life_orb_NCGR,      .paletteID = pltt_life_orb_NCLR,       .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_POWER_HERB]   = {  .dataID = 0xF9,  .iconID = icon_power_herb_NCGR,    .paletteID = pltt_power_herb_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TOXIC_ORB]    = {  .dataID = 0xFA,  .iconID = icon_toxic_orb_NCGR,     .paletteID = pltt_toxic_orb_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_FLAME_ORB]    = {  .dataID = 0xFB,  .iconID = icon_flame_orb_NCGR,     .paletteID = pltt_flame_orb_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_QUICK_POWDER] = {  .dataID = 0xFC,  .iconID = icon_powder_NCGR,        .paletteID = pltt_quick_powder_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_FOCUS_SASH]   = {  .dataID = 0xFD,  .iconID = icon_focus_sash_NCGR,    .paletteID = pltt_focus_sash_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_ZOOM_LENS]    = {  .dataID = 0xFE,  .iconID = icon_zoom_lens_NCGR,     .paletteID = pltt_zoom_lens_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_METRONOME]    = {  .dataID = 0xFF,  .iconID = icon_metronome_NCGR,     .paletteID = pltt_metronome_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_IRON_BALL]    = {  .dataID = 0x100, .iconID = icon_iron_ball_NCGR,     .paletteID = pltt_iron_ball_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_LAGGING_TAIL] = {  .dataID = 0x101, .iconID = icon_lagging_tail_NCGR,  .paletteID = pltt_lagging_tail_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_DESTINY_KNOT] = {  .dataID = 0x102, .iconID = icon_destiny_knot_NCGR,  .paletteID = pltt_destiny_knot_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_BLACK_SLUDGE] = {  .dataID = 0x103, .iconID = icon_black_sludge_NCGR,  .paletteID = pltt_black_sludge_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_ICY_ROCK]     = {  .dataID = 0x104, .iconID = icon_icy_rock_NCGR,      .paletteID = pltt_icy_rock_NCLR,       .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_SMOOTH_ROCK]  = {  .dataID = 0x105, .iconID = icon_smooth_rock_NCGR,   .paletteID = pltt_smooth_rock_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_HEAT_ROCK]    = {  .dataID = 0x106, .iconID = icon_heat_rock_NCGR,     .paletteID = pltt_heat_rock_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_DAMP_ROCK]    = {  .dataID = 0x107, .iconID = icon_damp_rock_NCGR,     .paletteID = pltt_damp_rock_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_GRIP_CLAW]    = {  .dataID = 0x108, .iconID = icon_grip_claw_NCGR,     .paletteID = pltt_grip_claw_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_CHOICE_SCARF] = {  .dataID = 0x109, .iconID = icon_choice_scarf_NCGR,  .paletteID = pltt_choice_scarf_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_STICKY_BARB]  = {  .dataID = 0x10A, .iconID = icon_sticky_barb_NCGR,   .paletteID = pltt_sticky_barb_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_POWER_BRACER] = {  .dataID = 0x10B, .iconID = icon_power_bracer_NCGR,  .paletteID = pltt_power_bracer_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_POWER_BELT]   = {  .dataID = 0x10C, .iconID = icon_power_belt_NCGR,    .paletteID = pltt_power_belt_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_POWER_LENS]   = {  .dataID = 0x10D, .iconID = icon_power_lens_NCGR,    .paletteID = pltt_power_lens_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_POWER_BAND]   = {  .dataID = 0x10E, .iconID = icon_power_band_NCGR,    .paletteID = pltt_power_band_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_POWER_ANKLET] = {  .dataID = 0x10F, .iconID = icon_power_anklet_NCGR,  .paletteID = pltt_power_anklet_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_POWER_WEIGHT] = {  .dataID = 0x110, .iconID = icon_power_weight_NCGR,  .paletteID = pltt_power_weight_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_SHED_SHELL]   = {  .dataID = 0x111, .iconID = icon_shed_shell_NCGR,    .paletteID = pltt_shed_shell_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_BIG_ROOT]     = {  .dataID = 0x112, .iconID = icon_big_root_NCGR,      .paletteID = pltt_big_root_NCLR,       .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_CHOICE_SPECS] = {  .dataID = 0x113, .iconID = icon_choice_specs_NCGR,  .paletteID = pltt_choice_specs_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_FLAME_PLATE]  = {  .dataID = 0x114, .iconID = icon_plate_NCGR,         .paletteID = pltt_flame_plate_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_SPLASH_PLATE] = {  .dataID = 0x115, .iconID = icon_plate_NCGR,         .paletteID = pltt_splash_plate_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_ZAP_PLATE]    = {  .dataID = 0x116, .iconID = icon_plate_NCGR,         .paletteID = pltt_zap_plate_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_MEADOW_PLATE] = {  .dataID = 0x117, .iconID = icon_plate_NCGR,         .paletteID = pltt_meadow_plate_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_ICICLE_PLATE] = {  .dataID = 0x118, .iconID = icon_plate_NCGR,         .paletteID = pltt_icicle_plate_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_FIST_PLATE]   = {  .dataID = 0x119, .iconID = icon_plate_NCGR,         .paletteID = pltt_fist_plate_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TOXIC_PLATE]  = {  .dataID = 0x11A, .iconID = icon_plate_NCGR,         .paletteID = pltt_toxic_plate_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_EARTH_PLATE]  = {  .dataID = 0x11B, .iconID = icon_plate_NCGR,         .paletteID = pltt_earth_plate_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_SKY_PLATE]    = {  .dataID = 0x11C, .iconID = icon_plate_NCGR,         .paletteID = pltt_sky_plate_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_MIND_PLATE]   = {  .dataID = 0x11D, .iconID = icon_plate_NCGR,         .paletteID = pltt_mind_plate_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_INSECT_PLATE] = {  .dataID = 0x11E, .iconID = icon_plate_NCGR,         .paletteID = pltt_insect_plate_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_STONE_PLATE]  = {  .dataID = 0x11F, .iconID = icon_plate_NCGR,         .paletteID = pltt_stone_plate_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_SPOOKY_PLATE] = {  .dataID = 0x120, .iconID = icon_plate_NCGR,         .paletteID = pltt_spooky_plate_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_DRACO_PLATE]  = {  .dataID = 0x121, .iconID = icon_plate_NCGR,         .paletteID = pltt_draco_plate_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_DREAD_PLATE]  = {  .dataID = 0x122, .iconID = icon_plate_NCGR,         .paletteID = pltt_dread_plate_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_IRON_PLATE]   = {  .dataID = 0x123, .iconID = icon_plate_NCGR,         .paletteID = pltt_iron_plate_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_ODD_INCENSE]  = {  .dataID = 0x124, .iconID = icon_odd_incense_NCGR,   .paletteID = pltt_odd_incense_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_ROCK_INCENSE] = {  .dataID = 0x125, .iconID = icon_rock_incense_NCGR,  .paletteID = pltt_rock_incense_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_FULL_INCENSE] = {  .dataID = 0x126, .iconID = icon_full_incense_NCGR,  .paletteID = pltt_full_incense_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_WAVE_INCENSE] = {  .dataID = 0x127, .iconID = icon_wave_incense_NCGR,  .paletteID = pltt_wave_incense_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_ROSE_INCENSE] = {  .dataID = 0x128, .iconID = icon_rose_incense_NCGR,  .paletteID = pltt_rose_incense_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_LUCK_INCENSE] = {  .dataID = 0x129, .iconID = icon_luck_incense_NCGR,  .paletteID = pltt_luck_incense_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_PURE_INCENSE] = {  .dataID = 0x12A, .iconID = icon_pure_incense_NCGR,  .paletteID = pltt_pure_incense_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_PROTECTOR]    = {  .dataID = 0x12B, .iconID = icon_protector_NCGR,     .paletteID = pltt_protector_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_ELECTIRIZER]  = {  .dataID = 0x12C, .iconID = icon_electirizer_NCGR,   .paletteID = pltt_electirizer_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_MAGMARIZER]   = {  .dataID = 0x12D, .iconID = icon_magmarizer_NCGR,    .paletteID = pltt_magmarizer_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_DUBIOUS_DISC] = {  .dataID = 0x12E, .iconID = icon_dubious_disc_NCGR,  .paletteID = pltt_dubious_disc_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_REAPER_CLOTH] = {  .dataID = 0x12F, .iconID = icon_reaper_cloth_NCGR,  .paletteID = pltt_reaper_cloth_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_RAZOR_CLAW]   = {  .dataID = 0x130, .iconID = icon_razor_claw_NCGR,    .paletteID = pltt_razor_claw_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_RAZOR_FANG]   = {  .dataID = 0x131, .iconID = icon_razor_fang_NCGR,    .paletteID = pltt_razor_fang_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM01]         = {  .dataID = 0x132, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_fighting_NCLR, .gen3ID = GBA_ITEM_TM01,          },
    [ITEM_TM02]         = {  .dataID = 0x133, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_dragon_NCLR,   .gen3ID = GBA_ITEM_TM02,          },
    [ITEM_TM03]         = {  .dataID = 0x134, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_water_NCLR,    .gen3ID = GBA_ITEM_TM03,          },
    [ITEM_TM04]         = {  .dataID = 0x135, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_psychic_NCLR,  .gen3ID = GBA_ITEM_TM04,          },
    [ITEM_TM05]         = {  .dataID = 0x136, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_TM05,          },
    [ITEM_TM06]         = {  .dataID = 0x137, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_poison_NCLR,   .gen3ID = GBA_ITEM_TM06,          },
    [ITEM_TM07]         = {  .dataID = 0x138, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_ice_NCLR,      .gen3ID = GBA_ITEM_TM07,          },
    [ITEM_TM08]         = {  .dataID = 0x139, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_fighting_NCLR, .gen3ID = GBA_ITEM_TM08,          },
    [ITEM_TM09]         = {  .dataID = 0x13A, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_grass_NCLR,    .gen3ID = GBA_ITEM_TM09,          },
    [ITEM_TM10]         = {  .dataID = 0x13B, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_TM10,          },
    [ITEM_TM11]         = {  .dataID = 0x13C, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_fire_NCLR,     .gen3ID = GBA_ITEM_TM11,          },
    [ITEM_TM12]         = {  .dataID = 0x13D, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_dark_NCLR,     .gen3ID = GBA_ITEM_TM12,          },
    [ITEM_TM13]         = {  .dataID = 0x13E, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_ice_NCLR,      .gen3ID = GBA_ITEM_TM13,          },
    [ITEM_TM14]         = {  .dataID = 0x13F, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_ice_NCLR,      .gen3ID = GBA_ITEM_TM14,          },
    [ITEM_TM15]         = {  .dataID = 0x140, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_TM15,          },
    [ITEM_TM16]         = {  .dataID = 0x141, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_psychic_NCLR,  .gen3ID = GBA_ITEM_TM16,          },
    [ITEM_TM17]         = {  .dataID = 0x142, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_TM17,          },
    [ITEM_TM18]         = {  .dataID = 0x143, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_water_NCLR,    .gen3ID = GBA_ITEM_TM18,          },
    [ITEM_TM19]         = {  .dataID = 0x144, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_grass_NCLR,    .gen3ID = GBA_ITEM_TM19,          },
    [ITEM_TM20]         = {  .dataID = 0x145, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_TM20,          },
    [ITEM_TM21]         = {  .dataID = 0x146, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_TM21,          },
    [ITEM_TM22]         = {  .dataID = 0x147, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_grass_NCLR,    .gen3ID = GBA_ITEM_TM22,          },
    [ITEM_TM23]         = {  .dataID = 0x148, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_steel_NCLR,    .gen3ID = GBA_ITEM_TM23,          },
    [ITEM_TM24]         = {  .dataID = 0x149, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_electric_NCLR, .gen3ID = GBA_ITEM_TM24,          },
    [ITEM_TM25]         = {  .dataID = 0x14A, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_electric_NCLR, .gen3ID = GBA_ITEM_TM25,          },
    [ITEM_TM26]         = {  .dataID = 0x14B, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_ground_NCLR,   .gen3ID = GBA_ITEM_TM26,          },
    [ITEM_TM27]         = {  .dataID = 0x14C, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_TM27,          },
    [ITEM_TM28]         = {  .dataID = 0x14D, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_ground_NCLR,   .gen3ID = GBA_ITEM_TM28,          },
    [ITEM_TM29]         = {  .dataID = 0x14E, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_psychic_NCLR,  .gen3ID = GBA_ITEM_TM29,          },
    [ITEM_TM30]         = {  .dataID = 0x14F, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_ghost_NCLR,    .gen3ID = GBA_ITEM_TM30,          },
    [ITEM_TM31]         = {  .dataID = 0x150, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_fighting_NCLR, .gen3ID = GBA_ITEM_TM31,          },
    [ITEM_TM32]         = {  .dataID = 0x151, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_TM32,          },
    [ITEM_TM33]         = {  .dataID = 0x152, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_psychic_NCLR,  .gen3ID = GBA_ITEM_TM33,          },
    [ITEM_TM34]         = {  .dataID = 0x153, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_electric_NCLR, .gen3ID = GBA_ITEM_TM34,          },
    [ITEM_TM35]         = {  .dataID = 0x154, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_fire_NCLR,     .gen3ID = GBA_ITEM_TM35,          },
    [ITEM_TM36]         = {  .dataID = 0x155, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_poison_NCLR,   .gen3ID = GBA_ITEM_TM36,          },
    [ITEM_TM37]         = {  .dataID = 0x156, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_rock_NCLR,     .gen3ID = GBA_ITEM_TM37,          },
    [ITEM_TM38]         = {  .dataID = 0x157, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_fire_NCLR,     .gen3ID = GBA_ITEM_TM38,          },
    [ITEM_TM39]         = {  .dataID = 0x158, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_rock_NCLR,     .gen3ID = GBA_ITEM_TM39,          },
    [ITEM_TM40]         = {  .dataID = 0x159, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_flying_NCLR,   .gen3ID = GBA_ITEM_TM40,          },
    [ITEM_TM41]         = {  .dataID = 0x15A, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_dark_NCLR,     .gen3ID = GBA_ITEM_TM41,          },
    [ITEM_TM42]         = {  .dataID = 0x15B, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_TM42,          },
    [ITEM_TM43]         = {  .dataID = 0x15C, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_TM43,          },
    [ITEM_TM44]         = {  .dataID = 0x15D, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_psychic_NCLR,  .gen3ID = GBA_ITEM_TM44,          },
    [ITEM_TM45]         = {  .dataID = 0x15E, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_TM45,          },
    [ITEM_TM46]         = {  .dataID = 0x15F, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_dark_NCLR,     .gen3ID = GBA_ITEM_TM46,          },
    [ITEM_TM47]         = {  .dataID = 0x160, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_steel_NCLR,    .gen3ID = GBA_ITEM_TM47,          },
    [ITEM_TM48]         = {  .dataID = 0x161, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_psychic_NCLR,  .gen3ID = GBA_ITEM_TM48,          },
    [ITEM_TM49]         = {  .dataID = 0x162, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_dark_NCLR,     .gen3ID = GBA_ITEM_TM49,          },
    [ITEM_TM50]         = {  .dataID = 0x163, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_fire_NCLR,     .gen3ID = GBA_ITEM_TM50,          },
    [ITEM_TM51]         = {  .dataID = 0x164, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_flying_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM52]         = {  .dataID = 0x165, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_fighting_NCLR, .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM53]         = {  .dataID = 0x166, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_grass_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM54]         = {  .dataID = 0x167, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM55]         = {  .dataID = 0x168, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_water_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM56]         = {  .dataID = 0x169, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_dark_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM57]         = {  .dataID = 0x16A, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_electric_NCLR, .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM58]         = {  .dataID = 0x16B, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM59]         = {  .dataID = 0x16C, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_dragon_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM60]         = {  .dataID = 0x16D, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_fighting_NCLR, .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM61]         = {  .dataID = 0x16E, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_fire_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM62]         = {  .dataID = 0x16F, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_bug_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM63]         = {  .dataID = 0x170, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_dark_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM64]         = {  .dataID = 0x171, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM65]         = {  .dataID = 0x172, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_ghost_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM66]         = {  .dataID = 0x173, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_dark_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM67]         = {  .dataID = 0x174, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM68]         = {  .dataID = 0x175, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM69]         = {  .dataID = 0x176, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_rock_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM70]         = {  .dataID = 0x177, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM71]         = {  .dataID = 0x178, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_rock_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM72]         = {  .dataID = 0x179, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_ice_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM73]         = {  .dataID = 0x17A, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_electric_NCLR, .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM74]         = {  .dataID = 0x17B, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_steel_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM75]         = {  .dataID = 0x17C, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM76]         = {  .dataID = 0x17D, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_rock_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM77]         = {  .dataID = 0x17E, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM78]         = {  .dataID = 0x17F, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM79]         = {  .dataID = 0x180, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_dark_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM80]         = {  .dataID = 0x181, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_rock_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM81]         = {  .dataID = 0x182, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_bug_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM82]         = {  .dataID = 0x183, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM83]         = {  .dataID = 0x184, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM84]         = {  .dataID = 0x185, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_poison_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM85]         = {  .dataID = 0x186, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_psychic_NCLR,  .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM86]         = {  .dataID = 0x187, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_grass_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM87]         = {  .dataID = 0x188, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM88]         = {  .dataID = 0x189, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_flying_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM89]         = {  .dataID = 0x18A, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_bug_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM90]         = {  .dataID = 0x18B, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM91]         = {  .dataID = 0x18C, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_steel_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TM92]         = {  .dataID = 0x18D, .iconID = icon_tm_NCGR,            .paletteID = pltt_tm_hm_psychic_NCLR,  .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_HM01]         = {  .dataID = 0x18E, .iconID = icon_hm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_HM01,          },
    [ITEM_HM02]         = {  .dataID = 0x18F, .iconID = icon_hm_NCGR,            .paletteID = pltt_tm_hm_flying_NCLR,   .gen3ID = GBA_ITEM_HM02,          },
    [ITEM_HM03]         = {  .dataID = 0x190, .iconID = icon_hm_NCGR,            .paletteID = pltt_tm_hm_water_NCLR,    .gen3ID = GBA_ITEM_HM03,          },
    [ITEM_HM04]         = {  .dataID = 0x191, .iconID = icon_hm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_HM04,          },
    [ITEM_HM05]         = {  .dataID = 0x192, .iconID = icon_hm_NCGR,            .paletteID = pltt_tm_hm_water_NCLR,    .gen3ID = GBA_ITEM_HM05,          },
    [ITEM_HM06]         = {  .dataID = 0x193, .iconID = icon_hm_NCGR,            .paletteID = pltt_tm_hm_fighting_NCLR, .gen3ID = GBA_ITEM_HM06,          },
    [ITEM_HM07]         = {  .dataID = 0x194, .iconID = icon_hm_NCGR,            .paletteID = pltt_tm_hm_water_NCLR,    .gen3ID = GBA_ITEM_HM07,          },
    [ITEM_HM08]         = {  .dataID = 0x195, .iconID = icon_hm_NCGR,            .paletteID = pltt_tm_hm_normal_NCLR,   .gen3ID = GBA_ITEM_HM08,          },
    [ITEM_EXPLORER_KIT] = {  .dataID = 0x196, .iconID = icon_explorer_kit_NCGR,  .paletteID = pltt_explorer_kit_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_LOOT_SACK]    = {  .dataID = 0x197, .iconID = icon_loot_sack_NCGR,     .paletteID = pltt_loot_sack_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_RULE_BOOK]    = {  .dataID = 0x198, .iconID = icon_rule_book_NCGR,     .paletteID = pltt_rule_book_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_POKE_RADAR]   = {  .dataID = 0x199, .iconID = icon_poke_radar_NCGR,    .paletteID = pltt_poke_radar_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_POINT_CARD]   = {  .dataID = 0x19A, .iconID = icon_point_card_NCGR,    .paletteID = pltt_point_card_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_JOURNAL]      = {  .dataID = 0x19B, .iconID = icon_journal_NCGR,       .paletteID = pltt_journal_NCLR,        .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_SEAL_CASE]    = {  .dataID = 0x19C, .iconID = icon_seal_case_NCGR,     .paletteID = pltt_seal_case_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_FASHION_CASE] = {  .dataID = 0x19D, .iconID = icon_fashion_case_NCGR,  .paletteID = pltt_fashion_case_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_SEAL_BAG]     = {  .dataID = 0x19E, .iconID = icon_seal_bag_NCGR,      .paletteID = pltt_seal_bag_NCLR,       .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_PAL_PAD]      = {  .dataID = 0x19F, .iconID = icon_pal_pad_NCGR,       .paletteID = pltt_pal_pad_NCLR,        .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_WORKS_KEY]    = {  .dataID = 0x1A0, .iconID = icon_works_key_NCGR,     .paletteID = pltt_works_key_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_OLD_CHARM]    = {  .dataID = 0x1A1, .iconID = icon_old_charm_NCGR,     .paletteID = pltt_old_charm_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_GALACTIC_KEY] = {  .dataID = 0x1A2, .iconID = icon_galactic_key_NCGR,  .paletteID = pltt_galactic_key_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_RED_CHAIN]    = {  .dataID = 0x1A3, .iconID = icon_red_chain_NCGR,     .paletteID = pltt_red_chain_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TOWN_MAP]     = {  .dataID = 0x1A4, .iconID = icon_town_map_NCGR,      .paletteID = pltt_town_map_NCLR,       .gen3ID = GBA_ITEM_TOWN_MAP,      },
    [ITEM_VS_SEEKER]    = {  .dataID = 0x1A5, .iconID = icon_vs_seeker_NCGR,     .paletteID = pltt_vs_seeker_NCLR,      .gen3ID = GBA_ITEM_VS_SEEKER,     },
    [ITEM_COIN_CASE]    = {  .dataID = 0x1A6, .iconID = icon_coin_case_NCGR,     .paletteID = pltt_coin_case_NCLR,      .gen3ID = GBA_ITEM_COIN_CASE,     },
    [ITEM_OLD_ROD]      = {  .dataID = 0x1A7, .iconID = icon_old_rod_NCGR,       .paletteID = pltt_old_rod_NCLR,        .gen3ID = GBA_ITEM_OLD_ROD,       },
    [ITEM_GOOD_ROD]     = {  .dataID = 0x1A8, .iconID = icon_good_rod_NCGR,      .paletteID = pltt_good_rod_NCLR,       .gen3ID = GBA_ITEM_GOOD_ROD,      },
    [ITEM_SUPER_ROD]    = {  .dataID = 0x1A9, .iconID = icon_super_rod_NCGR,     .paletteID = pltt_super_rod_NCLR,      .gen3ID = GBA_ITEM_SUPER_ROD,     },
    [ITEM_SPRAYDUCK]    = {  .dataID = 0x1AA, .iconID = icon_sprayduck_NCGR,     .paletteID = pltt_sprayduck_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_POFFIN_CASE]  = {  .dataID = 0x1AB, .iconID = icon_poffin_case_NCGR,   .paletteID = pltt_poffin_case_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_BICYCLE]      = {  .dataID = 0x1AC, .iconID = icon_bicycle_NCGR,       .paletteID = pltt_bicycle_NCLR,        .gen3ID = GBA_ITEM_BICYCLE,       },
    [ITEM_SUITE_KEY]    = {  .dataID = 0x1AD, .iconID = icon_suite_key_NCGR,     .paletteID = pltt_suite_key_NCLR,      .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_OAKS_LETTER]  = {  .dataID = 0x1AE, .iconID = icon_oaks_letter_NCGR,   .paletteID = pltt_lava_cookie_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_LUNAR_WING]   = {  .dataID = 0x1AF, .iconID = icon_lunar_wing_NCGR,    .paletteID = pltt_lunar_wing_NCLR,     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_MEMBER_CARD]  = {  .dataID = 0x1B0, .iconID = icon_member_card_NCGR,   .paletteID = pltt_member_card_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_AZURE_FLUTE]  = {  .dataID = 0x1B1, .iconID = icon_azure_flute_NCGR,   .paletteID = pltt_azure_flute_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_S_S_TICKET]   = {  .dataID = 0x1B2, .iconID = icon_s_s_ticket_NCGR,    .paletteID = pltt_s_s_ticket_NCLR,     .gen3ID = GBA_ITEM_SS_TICKET,     },
    [ITEM_CONTEST_PASS] = {  .dataID = 0x1B3, .iconID = icon_contest_pass_NCGR,  .paletteID = pltt_contest_pass_NCLR,   .gen3ID = GBA_ITEM_CONTEST_PASS,  },
    [ITEM_MAGMA_STONE]  = {  .dataID = 0x1B4, .iconID = icon_magma_stone_NCGR,   .paletteID = pltt_magma_stone_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_PARCEL]       = {  .dataID = 0x1B5, .iconID = icon_parcel_NCGR,        .paletteID = pltt_parcel_NCLR,         .gen3ID = GBA_ITEM_OAKS_PARCEL,   },
    [ITEM_COUPON_1]     = {  .dataID = 0x1B6, .iconID = icon_coupon_1_NCGR,      .paletteID = pltt_coupon_1_NCLR,       .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_COUPON_2]     = {  .dataID = 0x1B7, .iconID = icon_coupon_2_NCGR,      .paletteID = pltt_coupon_2_NCLR,       .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_COUPON_3]     = {  .dataID = 0x1B8, .iconID = icon_coupon_3_NCGR,      .paletteID = pltt_coupon_3_NCLR,       .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_STORAGE_KEY]  = {  .dataID = 0x1B9, .iconID = icon_storage_key_NCGR,   .paletteID = pltt_storage_key_NCLR,    .gen3ID = GBA_ITEM_STORAGE_KEY,   },
    [ITEM_SECRETPOTION] = {  .dataID = 0x1BA, .iconID = icon_secretpotion_NCGR,  .paletteID = pltt_secretpotion_NCLR,   .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_VS_RECORDER]  = {  .dataID = 0x1BB, .iconID = icon_vs_recorder_NCGR,   .paletteID = pltt_vs_recorder_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_GRACIDEA]     = {  .dataID = 0x1BC, .iconID = icon_gracidea_NCGR,      .paletteID = pltt_gracidea_NCLR,       .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_SECRET_KEY]   = {  .dataID = 0x1BD, .iconID = icon_secret_key_NCGR,    .paletteID = pltt_secret_key_NCLR,     .gen3ID = GBA_ITEM_SECRET_KEY,    },
    [ITEM_AIR_BALLOON]  = {  .dataID = 0x1BE, .iconID = icon_air_balloon_NCGR,   .paletteID = pltt_air_balloon_NCLR,    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_ROCKY_HELMET] = {  .dataID = 0x1BF, .iconID = icon_rocky_helmet_NCGR,                   .paletteID = pltt_rocky_helmet_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_EJECT_BUTTON] = {  .dataID = 0x1C0, .iconID = icon_eject_button_NCGR,                   .paletteID = pltt_eject_button_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_HEAVY_DUTY_BOOTS] = {  .dataID = 0x1C1, .iconID = icon_heavy_duty_boots_NCGR,                   .paletteID = pltt_heavy_duty_boots_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_WEAKNESS_POLICY] = {  .dataID = 0x1C2, .iconID = icon_weakness_policy_NCGR,                   .paletteID = pltt_weakness_policy_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_EJECT_PACK]   = {  .dataID = 0x1C3, .iconID = icon_eject_pack_NCGR,                   .paletteID = pltt_eject_pack_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_RED_CARD]     = {  .dataID = 0x1C4, .iconID = icon_red_card_NCGR,                   .paletteID = pltt_red_card_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_ASSAULT_VEST] = {  .dataID = 0x1C5, .iconID = icon_assault_vest_NCGR,                   .paletteID = pltt_assault_vest_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_COVERT_CLOAK] = {  .dataID = 0x1C6, .iconID = icon_covert_cloak_NCGR,                   .paletteID = pltt_covert_cloak_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_CLEAR_AMULET] = {  .dataID = 0x1C7, .iconID = icon_clear_amulet_NCGR,                   .paletteID = pltt_clear_amulet_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_SAFETY_GOGGLES] = {  .dataID = 0x1C8, .iconID = icon_safety_goggles_NCGR,                   .paletteID = pltt_safety_goggles_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_FLOAT_STONE]  = {  .dataID = 0x1C9, .iconID = icon_float_stone_NCGR,                   .paletteID = pltt_float_stone_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_THROAT_SPRAY] = {  .dataID = 0x1CA, .iconID = icon_throat_spray_NCGR,                   .paletteID = pltt_throat_spray_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_LOADED_DICE]  = {  .dataID = 0x1CB, .iconID = icon_loaded_dice_NCGR,                   .paletteID = pltt_loaded_dice_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_ROOM_SERVICE] = {  .dataID = 0x1CC, .iconID = icon_room_service_NCGR,                   .paletteID = pltt_room_service_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_EVIOLITE]     = {  .dataID = 0x1CD, .iconID = icon_eviolite_NCGR,                   .paletteID = pltt_eviolite_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_ABSORB_BULB]  = {  .dataID = 0x1CE, .iconID = icon_absorb_bulb_NCGR,                   .paletteID = pltt_absorb_bulb_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_ADRENALINE_ORB] = {  .dataID = 0x1CF,  .iconID = icon_adrenaline_orb_NCGR,                    .paletteID = pltt_adrenaline_orb_NCLR,                     .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_BLUNDER_POLICY] = {  .dataID = 0x1D0, .iconID = icon_blunder_policy_NCGR,                   .paletteID = pltt_blunder_policy_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_CELL_BATTERY] = {  .dataID = 0x1D1, .iconID = icon_cell_battery_NCGR,                   .paletteID = pltt_cell_battery_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_PUNCHING_GLOVE] = {  .dataID = 0x1D2, .iconID = icon_punching_glove_NCGR,                   .paletteID = pltt_punching_glove_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_NORMAL_GEM] = {  .dataID = 0x1D3, .iconID = icon_gem_NCGR,                .paletteID = 0x15F,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_FIGHTING_GEM] = {  .dataID = 0x1D4, .iconID = icon_gem_NCGR,                .paletteID = 0x15F,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_FLYING_GEM] = {  .dataID = 0x1D5, .iconID = icon_gem_NCGR,                .paletteID = 0x15F,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_POISON_GEM] = {  .dataID = 0x1D6, .iconID = icon_gem_NCGR,                .paletteID = 0x15F,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_GROUND_GEM] = {  .dataID = 0x1D7, .iconID = icon_gem_NCGR,                .paletteID = 0x15F,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_ROCK_GEM] = {  .dataID = 0x1D8, .iconID = icon_gem_NCGR,                .paletteID = 0x15F,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_BUG_GEM] = {  .dataID = 0x1D9, .iconID = icon_gem_NCGR,                .paletteID = 0x15F,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_GHOST_GEM] = {  .dataID = 0x1DA, .iconID = icon_gem_NCGR,                .paletteID = 0x15F,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_STEEL_GEM] = {  .dataID = 0x1DB, .iconID = icon_gem_NCGR,                .paletteID = 0x15F,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_MYSTERY_GEM] = {  .dataID = 0x1DC, .iconID = icon_gem_NCGR,                .paletteID = 0x15F,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_FIRE_GEM] = {  .dataID = 0x1DD, .iconID = icon_gem_NCGR,                .paletteID = 0x15F,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_WATER_GEM] = {  .dataID = 0x1DE, .iconID = icon_gem_NCGR,                .paletteID = 0x15F,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_GRASS_GEM] = {  .dataID = 0x1DF, .iconID = icon_gem_NCGR,                .paletteID = 0x15F,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_ELECTRIC_GEM] = {  .dataID = 0x1E0, .iconID = icon_gem_NCGR,                .paletteID = 0x15F,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_PSYCHIC_GEM] = {  .dataID = 0x1E1, .iconID = icon_gem_NCGR,                .paletteID = 0x15F,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_ICE_GEM] = {  .dataID = 0x1E2, .iconID = icon_gem_NCGR,                .paletteID = 0x15F,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_DRAGON_GEM] = {  .dataID = 0x1E3, .iconID = icon_gem_NCGR,                .paletteID = 0x15F,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_DARK_GEM] = {  .dataID = 0x1E4, .iconID = icon_gem_NCGR,                .paletteID = 0x15F,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_INFINITE_CANDY] = {  .dataID = 0x1E5, .iconID = icon_rare_candy_NCGR,    .paletteID = pltt_rare_candy_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_ABILITY_CAPSULE] = {  .dataID = 0x1E6, .iconID = icon_ability_capsule_NCGR,                .paletteID = pltt_ability_capsule_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_BINDING_BAND] = {  .dataID = 0x1E7, .iconID = icon_binding_band_NCGR,                .paletteID = pltt_binding_band_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_ICE_STONE] = {  .dataID = 0x1E8, .iconID = icon_ice_stone_NCGR,                .paletteID = pltt_ice_stone_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_PRISM_SCALE] = {  .dataID = 0x1E9, .iconID = icon_prism_scale_NCGR,                .paletteID = pltt_prism_scale_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_CLEAR_BELL] = {  .dataID = 0x1EA, .iconID = icon_clear_bell_NCGR,                .paletteID = pltt_clear_bell_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_TIDAL_BELL] = {  .dataID = 0x1EB, .iconID = icon_tidal_bell_NCGR,                .paletteID = pltt_tidal_bell_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
    [ITEM_RED_ORB] = {  .dataID = 0x1EC, .iconID = icon_red_orb_NCGR,                .paletteID = pltt_red_orb_NCLR,                    .gen3ID = GBA_ITEM_BLUE_ORB,          },
    [ITEM_BLUE_ORB] = {  .dataID = 0x1ED, .iconID = icon_blue_orb_NCGR,                .paletteID = pltt_blue_orb_NCLR,                    .gen3ID = GBA_ITEM_RED_ORB,          },
    [ITEM_JADE_ORB] = {  .dataID = 0x1EE, .iconID = icon_jade_orb_NCGR,                .paletteID = pltt_jade_orb_NCLR,                    .gen3ID = GBA_ITEM_NONE,          },
};

static const u16 sTMHMMoves[] = {
    MOVE_HIGH_HORSEPOWER, // TM01
    MOVE_DRAGON_CLAW, // TM02
    MOVE_LIQUIDATION, // TM03
    MOVE_ACROBATICS, // TM04
    MOVE_ROAR, // TM05
    MOVE_TOXIC, // TM06
    MOVE_TAILWIND, // TM07
    MOVE_FIRST_IMPRESSION, // TM08
    MOVE_BULLET_SEED, // TM09
    MOVE_HIDDEN_POWER, // TM10
    MOVE_WAVE_CRASH, // TM11
    MOVE_TAUNT, // TM12
    MOVE_ICE_BEAM, // TM13
    MOVE_BLIZZARD, // TM14
    MOVE_HYPER_BEAM, // TM15
    MOVE_LIGHT_SCREEN, // TM16
    MOVE_ACCELEROCK, // TM17
    MOVE_TRIPLE_AXEL, // TM18
    MOVE_GIGA_DRAIN, // TM19
    MOVE_SAFEGUARD, // TM20
    MOVE_SOLAR_BLADE, // TM21
    MOVE_SOLAR_BEAM, // TM22
    MOVE_IRON_TAIL, // TM23
    MOVE_THUNDERBOLT, // TM24
    MOVE_THUNDER, // TM25
    MOVE_EARTHQUAKE, // TM26
    MOVE_RETURN, // TM27
    MOVE_DRILL_RUN, // TM28
    MOVE_PSYCHIC, // TM29
    MOVE_SHADOW_BALL, // TM30
    MOVE_BRICK_BREAK, // TM31
    MOVE_SLUDGE_WAVE, // TM32
    MOVE_REFLECT, // TM33
    MOVE_SHOCK_WAVE, // TM34
    MOVE_FLAMETHROWER, // TM35
    MOVE_SLUDGE_BOMB, // TM36
    MOVE_DRAGON_DARTS, // TM37
    MOVE_FIRE_BLAST, // TM38
    MOVE_ROCK_TOMB, // TM39
    MOVE_AERIAL_ACE, // TM40
    MOVE_VOLT_SWITCH, // TM41
    MOVE_FACADE, // TM42
    MOVE_SECRET_POWER, // TM43
    MOVE_REST, // TM44
    MOVE_WEATHER_BALL, // TM45
    MOVE_HURRICANE, // TM46
    MOVE_SMART_STRIKE, // TM47
    MOVE_SCALD, // TM48
    MOVE_WILD_CHARGE, // TM49
    MOVE_OVERHEAT, // TM50
    MOVE_ROOST, // TM51
    MOVE_FOCUS_BLAST, // TM52
    MOVE_ENERGY_BALL, // TM53
    MOVE_FALSE_SWIPE, // TM54
    MOVE_DUAL_WINGBEAT, // TM55
    MOVE_RAZOR_SHELL, // TM56
    MOVE_ELECTROWEB, // TM57
    MOVE_ICICLE_CRASH, // TM58
    MOVE_DRAGON_PULSE, // TM59
    MOVE_DRAIN_PUNCH, // TM60
    MOVE_WILL_O_WISP, // TM61
    MOVE_SILVER_WIND, // TM62
    MOVE_SCORCHING_SANDS, // TM63
    MOVE_EXPLOSION, // TM64
    MOVE_SHADOW_CLAW, // TM65
    MOVE_PAYBACK, // TM66
    MOVE_BREAKING_SWIPE, // TM67
    MOVE_GIGA_IMPACT, // TM68
    MOVE_EARTH_POWER, // TM69
    MOVE_PSYCHIC_FANGS, // TM70
    MOVE_STONE_EDGE, // TM71
    MOVE_AVALANCHE, // TM72
    MOVE_THUNDER_WAVE, // TM73
    MOVE_GYRO_BALL, // TM74
    MOVE_KNOCK_OFF, // TM75
    MOVE_SEED_BOMB, // TM76
    MOVE_ZEN_HEADBUTT, // TM77
    MOVE_FLIP_TURN, // TM78
    MOVE_DARK_PULSE, // TM79
    MOVE_ROCK_SLIDE, // TM80
    MOVE_X_SCISSOR, // TM81
    MOVE_SLEEP_TALK, // TM82
    MOVE_BULLDOZE, // TM83
    MOVE_POISON_JAB, // TM84
    MOVE_MYSTICAL_FIRE, // TM85
    MOVE_GRASS_KNOT, // TM86
    MOVE_HEAT_WAVE, // TM87
    MOVE_PLUCK, // TM88
    MOVE_U_TURN, // TM89
    MOVE_SIGNAL_BEAM, // TM90
    MOVE_FLASH_CANNON, // TM91
    MOVE_SUPERPOWER, // TM92
    MOVE_CUT, // HM01
    MOVE_FLY, // HM02
    MOVE_SURF, // HM03
    MOVE_STRENGTH, // HM04
    MOVE_DEFOG, // HM05
    MOVE_ROCK_SMASH, // HM06
    MOVE_WATERFALL, // HM07
    MOVE_ROCK_CLIMB, // HM08
};

const u16 sMailItemIDs[] = {
    ITEM_GRASS_MAIL,
    ITEM_FLAME_MAIL,
    ITEM_BUBBLE_MAIL,
    ITEM_BLOOM_MAIL,
    ITEM_TUNNEL_MAIL,
    ITEM_STEEL_MAIL,
    ITEM_HEART_MAIL,
    ITEM_SNOW_MAIL,
    ITEM_SPACE_MAIL,
    ITEM_AIR_MAIL,
    ITEM_MOSAIC_MAIL,
    ITEM_BRICK_MAIL
};

const u16 sBerryItemIDs[] = {
    ITEM_CHERI_BERRY,
    ITEM_CHESTO_BERRY,
    ITEM_PECHA_BERRY,
    ITEM_RAWST_BERRY,
    ITEM_ASPEAR_BERRY,
    ITEM_LEPPA_BERRY,
    ITEM_ORAN_BERRY,
    ITEM_PERSIM_BERRY,
    ITEM_LUM_BERRY,
    ITEM_SITRUS_BERRY,
    ITEM_FIGY_BERRY,
    ITEM_WIKI_BERRY,
    ITEM_MAGO_BERRY,
    ITEM_AGUAV_BERRY,
    ITEM_IAPAPA_BERRY,
    ITEM_RAZZ_BERRY,
    ITEM_BLUK_BERRY,
    ITEM_NANAB_BERRY,
    ITEM_WEPEAR_BERRY,
    ITEM_PINAP_BERRY,
    ITEM_POMEG_BERRY,
    ITEM_KELPSY_BERRY,
    ITEM_QUALOT_BERRY,
    ITEM_HONDEW_BERRY,
    ITEM_GREPA_BERRY,
    ITEM_TAMATO_BERRY,
    ITEM_CORNN_BERRY,
    ITEM_MAGOST_BERRY,
    ITEM_RABUTA_BERRY,
    ITEM_NOMEL_BERRY,
    ITEM_SPELON_BERRY,
    ITEM_PAMTRE_BERRY,
    ITEM_WATMEL_BERRY,
    ITEM_DURIN_BERRY,
    ITEM_BELUE_BERRY,
    ITEM_OCCA_BERRY,
    ITEM_PASSHO_BERRY,
    ITEM_WACAN_BERRY,
    ITEM_RINDO_BERRY,
    ITEM_YACHE_BERRY,
    ITEM_CHOPLE_BERRY,
    ITEM_KEBIA_BERRY,
    ITEM_SHUCA_BERRY,
    ITEM_COBA_BERRY,
    ITEM_PAYAPA_BERRY,
    ITEM_TANGA_BERRY,
    ITEM_CHARTI_BERRY,
    ITEM_KASIB_BERRY,
    ITEM_HABAN_BERRY,
    ITEM_COLBUR_BERRY,
    ITEM_BABIRI_BERRY,
    ITEM_CHILAN_BERRY,
    ITEM_LIECHI_BERRY,
    ITEM_GANLON_BERRY,
    ITEM_SALAC_BERRY,
    ITEM_PETAYA_BERRY,
    ITEM_APICOT_BERRY,
    ITEM_LANSAT_BERRY,
    ITEM_STARF_BERRY,
    ITEM_ENIGMA_BERRY,
    ITEM_MICLE_BERRY,
    ITEM_CUSTAP_BERRY,
    ITEM_JABOCA_BERRY,
    ITEM_ROWAP_BERRY
};

void Item_MoveInPocket(void *pocket, u16 srcSlot, u16 dstSlot)
{
    if (srcSlot == dstSlot) {
        return;
    }

    BagItem *items = (BagItem *)pocket;
    BagItem item = items[srcSlot];
    s16 i; // must be here to match
    if (dstSlot > srcSlot) {
        // Moving the item down in the pocket list
        dstSlot -= 1;

        // Bubble all the other items upward
        for (i = (s16)srcSlot; i < (s16)dstSlot; i++) {
            items[i] = items[i + 1];
        }
    } else {
        // Moving the item up in the pocket list
        // Sift all the other items downward
        for (i = (s16)srcSlot; i > (s16)dstSlot; i--) {
            items[i] = items[i - 1];
        }
    }

    items[dstSlot] = item;
}

u16 Item_FileID(u16 item, enum ItemFileType type)
{
    switch (type) {
    case ITEM_FILE_TYPE_DATA:
        if (item == ITEM_NONE || item == ITEM_RETURN_ID) {
            break;
        }

        return sItemArchiveIDs[item].dataID;
    case ITEM_FILE_TYPE_ICON:
        if (item == ITEM_NONE) {
            return 707;
        }

        if (item == ITEM_RETURN_ID) {
            return 709;
        }

        return sItemArchiveIDs[item].iconID;
    case ITEM_FILE_TYPE_PALETTE:
        if (item == 0) {
            return 708;
        }

        if (item == ITEM_RETURN_ID) {
            return 710;
        }

        return sItemArchiveIDs[item].paletteID;
    case ITEM_FILE_TYPE_GBA:
        if (item == ITEM_NONE || item == ITEM_RETURN_ID) {
            break;
        }

        return sItemArchiveIDs[item].gen3ID;
    }

    return 0;
}

u16 Item_FromGBAID(u16 gbaID)
{
    for (u16 i = 1; i <= NUM_ITEMS; i++) {
        if (gbaID == sItemArchiveIDs[i].gen3ID) {
            return i;
        }
    }

    return ITEM_NONE;
}

u16 Item_IconNCERFile(void)
{
    return 1;
}

u16 Item_IconNANRFile(void)
{
    return 0;
}

void *Item_Load(u16 item, enum ItemFileType type, u32 heapID)
{
    if (item > NUM_ITEMS) {
        item = ITEM_NONE;
    }

    switch (type) {
    case ITEM_FILE_TYPE_DATA:
        return NARC_AllocAndReadWholeMemberByIndexPair(NARC_INDEX_ITEMTOOL__ITEMDATA__PL_ITEM_DATA, sItemArchiveIDs[item].dataID, heapID);
    case ITEM_FILE_TYPE_ICON:
        return NARC_AllocAndReadWholeMemberByIndexPair(NARC_INDEX_ITEMTOOL__ITEMDATA__ITEM_ICON, sItemArchiveIDs[item].iconID, heapID);
    case ITEM_FILE_TYPE_PALETTE:
        return NARC_AllocAndReadWholeMemberByIndexPair(NARC_INDEX_ITEMTOOL__ITEMDATA__ITEM_ICON, sItemArchiveIDs[item].paletteID, heapID);
    }

    return NULL;
}

void Item_LoadName(Strbuf *dst, u16 item, u32 heapID)
{
    MessageLoader *msgData = MessageLoader_Init(1, 26, 392, heapID);

    MessageLoader_GetStrbuf(msgData, item, dst);
    MessageLoader_Free(msgData);
}

void Item_LoadDescription(Strbuf *dst, u16 item, u16 heapID)
{
    MessageLoader *msgData = MessageLoader_Init(1, 26, 391, heapID);

    MessageLoader_GetStrbuf(msgData, item, dst);
    MessageLoader_Free(msgData);
}

s32 Item_LoadParam(u16 item, enum ItemDataParam param, u32 heapID)
{
    ItemData *itemData = (ItemData *)Item_Load(item, 0, heapID);
    s32 val = Item_Get(itemData, param);
    Heap_FreeToHeapExplicit(heapID, itemData);

    return val;
}

s32 Item_Get(ItemData *itemData, enum ItemDataParam param)
{
    switch (param) {
    case ITEM_PARAM_PRICE:
        return (s32)itemData->price;
    case ITEM_PARAM_HOLD_EFFECT:
        return (s32)itemData->holdEffect;
    case ITEM_PARAM_HOLD_EFFECT_PARAM:
        return (s32)itemData->holdEffectParam;
    case ITEM_PARAM_PREVENT_TOSS:
        return (s32)itemData->preventToss;
    case ITEM_PARAM_IS_SELECTABLE:
        return (s32)itemData->isSelectable;
    case ITEM_PARAM_FIELD_POCKET:
        return (s32)itemData->fieldPocket;
    case ITEM_PARAM_FIELD_USE_FUNC:
        return (s32)itemData->fieldUseFunc;
    case ITEM_PARAM_BATTLE_USE_FUNC:
        return (s32)itemData->battleUseFunc;
    case ITEM_PARAM_PLUCK_EFFECT:
        return (s32)itemData->pluckEffect;
    case ITEM_PARAM_FLING_EFFECT:
        return (s32)itemData->flingEffect;
    case ITEM_PARAM_FLING_POWER:
        return (s32)itemData->flingPower;
    case ITEM_PARAM_NATURAL_GIFT_POWER:
        return (s32)itemData->naturalGiftPower;
    case ITEM_PARAM_NATURAL_GIFT_TYPE:
        return (s32)itemData->naturalGiftType;
    case ITEM_PARAM_BATTLE_POCKET:
        if (!(getVar(VAR_DIFFICULTY) & ITEMS_ENABLED) && ((s32)itemData->battlePocket != 1))
        {
            return 0;
        }
        return (s32)itemData->battlePocket;
    case ITEM_PARAM_PARTY_USE:
        return (s32)itemData->partyUse;
    default:
        switch (itemData->partyUse) {
        case FALSE:
            return itemData->dummy;
        case TRUE:
            return ItemPartyParam_Get(&itemData->partyUseParam, param);
        }
    }

    return 0;
}

static s32 ItemPartyParam_Get(ItemPartyParam *partyParam, enum ItemDataParam attributeID)
{
    switch (attributeID) {
    case ITEM_PARAM_HEAL_SLEEP:
        return (s32)partyParam->healSleep;
    case ITEM_PARAM_HEAL_POISON:
        return (s32)partyParam->healPoison;
    case ITEM_PARAM_HEAL_BURN:
        return (s32)partyParam->healBurn;
    case ITEM_PARAM_HEAL_FREEZE:
        return (s32)partyParam->healFreeze;
    case ITEM_PARAM_HEAL_PARALYSIS:
        return (s32)partyParam->healParalysis;
    case ITEM_PARAM_HEAL_CONFUSION:
        return (s32)partyParam->healConfusion;
    case ITEM_PARAM_HEAL_ATTRACT:
        return (s32)partyParam->healAttract;
    case ITEM_PARAM_GUARD_SPEC:
        return (s32)partyParam->guardSpec;
    case ITEM_PARAM_REVIVE:
        return (s32)partyParam->revive;
    case ITEM_PARAM_REVIVE_ALL:
        return (s32)partyParam->reviveAll;
    case ITEM_PARAM_LEVEL_UP:
        return (s32)partyParam->levelUp;
    case ITEM_PARAM_EVOLVE:
        return (s32)partyParam->evolve;
    case ITEM_PARAM_ATK_STAGES:
        return (s32)partyParam->atkStages;
    case ITEM_PARAM_DEF_STAGES:
        return (s32)partyParam->defStages;
    case ITEM_PARAM_SPATK_STAGES:
        return (s32)partyParam->spatkStages;
    case ITEM_PARAM_SPDEF_STAGES:
        return (s32)partyParam->spdefStages;
    case ITEM_PARAM_SPEED_STAGES:
        return (s32)partyParam->speedStages;
    case ITEM_PARAM_ACC_STAGES:
        return (s32)partyParam->accStages;
    case ITEM_PARAM_CRIT_STAGES:
        return (s32)partyParam->critStages;
    case ITEM_PARAM_PP_UP:
        return (s32)partyParam->ppUp;
    case ITEM_PARAM_PP_MAX:
        return (s32)partyParam->ppMax;
    case ITEM_PARAM_PP_RESTORE:
        return (s32)partyParam->ppRestore;
    case ITEM_PARAM_PP_RESTORE_ALL:
        return (s32)partyParam->ppRestoreAll;
    case ITEM_PARAM_HP_RESTORE:
        return (s32)partyParam->hpRestore;
    case ITEM_PARAM_GIVE_HP_EVS:
        return (s32)partyParam->giveHPEVs;
    case ITEM_PARAM_GIVE_ATK_EVS:
        return (s32)partyParam->giveAtkEVs;
    case ITEM_PARAM_GIVE_DEF_EVS:
        return (s32)partyParam->giveDefEVs;
    case ITEM_PARAM_GIVE_SPEED_EVS:
        return (s32)partyParam->giveSpeedEVs;
    case ITEM_PARAM_GIVE_SPATK_EVS:
        return (s32)partyParam->giveSpAtkEVs;
    case ITEM_PARAM_GIVE_SPDEF_EVS:
        return (s32)partyParam->giveSpDefEVs;
    case ITEM_PARAM_GIVE_FRIENDSHIP_LOW:
        return (s32)partyParam->giveFriendshipLow;
    case ITEM_PARAM_GIVE_FRIENDSHIP_MED:
        return (s32)partyParam->giveFriendshipMed;
    case ITEM_PARAM_GIVE_FRIENDSHIP_HIGH:
        return (s32)partyParam->giveFriendshipHigh;
    case ITEM_PARAM_HP_EVS:
        return (s32)partyParam->hpEVs;
    case ITEM_PARAM_ATK_EVS:
        return (s32)partyParam->atkEVs;
    case ITEM_PARAM_DEF_EVS:
        return (s32)partyParam->defEVs;
    case ITEM_PARAM_SPEED_EVS:
        return (s32)partyParam->speedEVs;
    case ITEM_PARAM_SPATK_EVS:
        return (s32)partyParam->spatkEVs;
    case ITEM_PARAM_SPDEF_EVS:
        return (s32)partyParam->spdefEVs;
    case ITEM_PARAM_HP_RESTORED:
        return (s32)partyParam->hpRestored;
    case ITEM_PARAM_PP_RESTORED:
        return (s32)partyParam->ppRestored;
    case ITEM_PARAM_FRIENDSHIP_LOW:
        return (s32)partyParam->friendshipLow;
    case ITEM_PARAM_FRIENDSHIP_MED:
        return (s32)partyParam->friendshipMed;
    case ITEM_PARAM_FRIENDSHIP_HIGH:
        return (s32)partyParam->friendshipHigh;
    }

    return 0;
}

const u16 Item_MoveForTMHM(u16 item)
{
    if (item < ITEM_TM01 || item > ITEM_HM08) {
        return MOVE_NONE;
    }

    item -= ITEM_TM01;
    return sTMHMMoves[item];
}

u8 Item_IsHMMove(u16 move)
{
    for (u8 i = 0; i < 8; i++) {
        if (sTMHMMoves[92 + i] == move) {
            return TRUE;
        }
    }

    return FALSE;
}

u8 Item_TMHMNumber(u16 item)
{
    if (item < ITEM_TM01 || item > ITEM_HM08) {
        return ITEM_NONE;
    }

    return item - ITEM_TM01;
}

u16 Item_HMFromMove(u16 moveID)
{
    u16 item = ITEM_NONE;
    switch (moveID) {
    case MOVE_CUT:
        item = ITEM_HM01;
        break;
    case MOVE_FLY:
        item = ITEM_HM02;
        break;
    case MOVE_SURF:
        item = ITEM_HM03;
        break;
    case MOVE_STRENGTH:
        item = ITEM_HM04;
        break;
    case MOVE_DEFOG:
        item = ITEM_HM05;
        break;
    case MOVE_ROCK_SMASH:
        item = ITEM_HM06;
        break;
    case MOVE_WATERFALL:
        item = ITEM_HM07;
        break;
    case MOVE_ROCK_CLIMB:
        item = ITEM_HM08;
        break;
    }
    return item;
}

u8 Item_IsMail(u16 item)
{
    for (u32 i = 0; i < NUM_MAILS; i++) {
        if (sMailItemIDs[i] == item) {
            return TRUE;
        }
    }

    return FALSE;
}

u8 Item_MailNumber(u16 item)
{
    for (u32 i = 0; i < NUM_MAILS; i++) {
        if (sMailItemIDs[i] == item) {
            return i;
        }
    }

    return ITEM_NONE;
}

u16 Item_ForMailNumber(u8 mail)
{
    if (mail >= NUM_MAILS) {
        return ITEM_NONE;
    }

    return sMailItemIDs[mail];
}

u8 Item_IsBerry(u16 item)
{
    for (u32 i = 0; i < NUM_BERRIES; i++) {
        if (sBerryItemIDs[i] == item) {
            return TRUE;
        }
    }

    return FALSE;
}

u8 Item_BerryNumber(u16 item)
{
    if (item < ITEM_CHERI_BERRY) {
        return 0xFF;
    }

    return item - ITEM_CHERI_BERRY;
}

u16 Item_ForBerryNumber(u8 berry)
{
    if (berry >= NUM_BERRIES) {
        return ITEM_RETURN_ID;
    }

    return sBerryItemIDs[berry];
}

u8 Item_IsHerbalMedicine(u16 item)
{
    return item == ITEM_ENERGYPOWDER
        || item == ITEM_ENERGY_ROOT
        || item == ITEM_HEAL_POWDER
        || item == ITEM_REVIVAL_HERB;
}

void *ItemTable_Load(int heapID)
{
    int maxItem = Item_FileID(NUM_ITEMS, ITEM_FILE_TYPE_DATA);
    return NARC_AllocAndReadFromMemberByIndexPair(NARC_INDEX_ITEMTOOL__ITEMDATA__PL_ITEM_DATA, 0, heapID, 0, sizeof(ItemData) * maxItem);
}

ItemData *ItemTable_Index(ItemData *itemTable, u16 index)
{
    return (ItemData *)((u8 *)itemTable + index * sizeof(ItemData));
}
