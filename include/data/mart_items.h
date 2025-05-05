#include "constants/items.h"
#include "constants/seals.h"

typedef struct {
    u16 itemID;
    u16 requiredBadges;
} PokeMartCommonItem;

const PokeMartCommonItem PokeMartCommonItems[] = {
    { ITEM_POKE_BALL, 0x1 },
    { ITEM_POTION, 0x1 },
    { ITEM_SUPER_POTION, 0x1 },
    { ITEM_HYPER_POTION, 0x1 },
    { ITEM_MAX_POTION, 0x1 },
    { ITEM_FULL_HEAL, 0x4 },
    { ITEM_FULL_RESTORE, 0x1 },
    { ITEM_REVIVE, 0x1 },
    { ITEM_ESCAPE_ROPE, 0x1 }
};

const u16 JubilifeMartSpecialties[] = {
    ITEM_PECHA_BERRY,
    ITEM_CHERI_BERRY,
    ITEM_RAWST_BERRY,
    ITEM_ASPEAR_BERRY,
    ITEM_PERSIM_BERRY,
    0xffff
};

const u16 OreburghMartSpecialties[] = {
    ITEM_TUNNEL_MAIL,
    ITEM_HEAL_BALL,
    ITEM_NET_BALL,
    0xffff
};

const u16 FloaromaMartSpecialties[] = {
    ITEM_BLOOM_MAIL,
    ITEM_HEAL_BALL,
    ITEM_NET_BALL,
    0xffff
};

const u16 EternaMartSpecialties[] = {
    ITEM_AIR_MAIL,
    ITEM_HEAL_BALL,
    ITEM_NET_BALL,
    ITEM_NEST_BALL,
    0xffff
};

const u16 EternaHerbShopStock[] = {
    ITEM_HEAL_POWDER,
    ITEM_ENERGYPOWDER,
    ITEM_ENERGY_ROOT,
    ITEM_REVIVAL_HERB,
    0xffff
};

const u16 HearthomeMartSpecialties[] = {
    ITEM_HEART_MAIL,
    ITEM_HEAL_BALL,
    ITEM_NET_BALL,
    ITEM_NEST_BALL,
    0xffff
};

const u16 SolaceonMartSpecialties[] = {
    ITEM_AIR_MAIL,
    ITEM_NET_BALL,
    ITEM_NEST_BALL,
    ITEM_DUSK_BALL,
    0xffff
};

const u16 PastoriaMartSpecialties[] = {
    ITEM_AIR_MAIL,
    ITEM_NEST_BALL,
    ITEM_DUSK_BALL,
    ITEM_QUICK_BALL,
    0xffff
};

const u16 VeilstoneDeptStoreStock_1F_RIGHT[] = {
    ITEM_POTION,
    ITEM_SUPER_POTION,
    ITEM_HYPER_POTION,
    ITEM_MAX_POTION,
    ITEM_REVIVE,
    ITEM_ANTIDOTE,
    ITEM_PARLYZ_HEAL,
    ITEM_BURN_HEAL,
    ITEM_ICE_HEAL,
    ITEM_AWAKENING,
    ITEM_FULL_HEAL,
    0xffff
};

const u16 VeilstoneDeptStoreStock_1F_LEFT[] = {
    ITEM_POKE_BALL,
    ITEM_GREAT_BALL,
    ITEM_ULTRA_BALL,
    ITEM_ESCAPE_ROPE,
    ITEM_POKE_DOLL,
    ITEM_REPEL,
    ITEM_SUPER_REPEL,
    ITEM_MAX_REPEL,
    ITEM_GRASS_MAIL,
    ITEM_FLAME_MAIL,
    ITEM_BUBBLE_MAIL,
    ITEM_SPACE_MAIL,
    0xffff
};

const u16 VeilstoneDeptStoreStock_2F_UP[] = {
    ITEM_X_SPEED,
    ITEM_X_ATTACK,
    ITEM_X_DEFENSE,
    ITEM_GUARD_SPEC_,
    ITEM_DIRE_HIT,
    ITEM_X_ACCURACY,
    ITEM_X_SPECIAL,
    ITEM_X_SP__DEF,
    0xffff
};

const u16 VeilstoneDeptStoreStock_2F_MID[] = {
    ITEM_PROTEIN,
    ITEM_IRON,
    ITEM_CALCIUM,
    ITEM_ZINC,
    ITEM_CARBOS,
    ITEM_HP_UP,
    0xffff
};

const u16 VeilstoneDeptStoreStock_3F_UP[] = {
    ITEM_TM31,
    ITEM_TM14,
    ITEM_TM25,
    ITEM_TM38,
    ITEM_TM46,
    0xffff
};

const u16 VeilstoneDeptStoreStock_3F_DOWN[] = {
    ITEM_TM38,
    ITEM_TM25,
    ITEM_TM14,
    ITEM_TM22,
    ITEM_TM52,
    ITEM_TM15,
    0xffff
};

const u16 CelesticMartSpecialties[] = {
    ITEM_AIR_MAIL,
    ITEM_DUSK_BALL,
    ITEM_QUICK_BALL,
    ITEM_TIMER_BALL,
    0xffff
};

const u16 SnowpointMartSpecialties[] = {
    ITEM_SNOW_MAIL,
    ITEM_DUSK_BALL,
    ITEM_QUICK_BALL,
    ITEM_TIMER_BALL,
    0xffff
};

const u16 CanalaveMartSpecialties[] = {
    ITEM_AIR_MAIL,
    ITEM_QUICK_BALL,
    ITEM_TIMER_BALL,
    ITEM_REPEAT_BALL,
    0xffff
};

const u16 SunyshoreMartSpecialties[] = {
    ITEM_STEEL_MAIL,
    ITEM_LUXURY_BALL,
    0xffff
};

const u16 PokemonLeagueMartSpecialties[] = {
    ITEM_NORMAL_GEM,
    ITEM_WATER_GEM,
    0xffff
};

const u16 VeilstoneDeptStoreStock_B1F_DOWN_LEFT[] = {
    ITEM_FIGY_BERRY,
    ITEM_WIKI_BERRY,
    ITEM_MAGO_BERRY,
    ITEM_AGUAV_BERRY,
    ITEM_IAPAPA_BERRY,
    0xffff
};

const u16 SandgemTownSpecialties[] = {
    ITEM_ORAN_BERRY,
    0xffff
};

const u16 *PokeMartSpecialties[] = {
    JubilifeMartSpecialties,
    OreburghMartSpecialties,
    FloaromaMartSpecialties,
    EternaMartSpecialties,
    EternaHerbShopStock,
    HearthomeMartSpecialties,
    SolaceonMartSpecialties,
    PastoriaMartSpecialties,
    VeilstoneDeptStoreStock_1F_RIGHT,
    VeilstoneDeptStoreStock_1F_LEFT,
    VeilstoneDeptStoreStock_2F_UP,
    VeilstoneDeptStoreStock_2F_MID,
    VeilstoneDeptStoreStock_3F_UP,
    VeilstoneDeptStoreStock_3F_DOWN,
    CelesticMartSpecialties,
    SnowpointMartSpecialties,
    CanalaveMartSpecialties,
    SunyshoreMartSpecialties,
    PokemonLeagueMartSpecialties,
    VeilstoneDeptStoreStock_B1F_DOWN_LEFT,
    SandgemTownSpecialties
};

const u16 VeilstoneDeptStoreStock_4F_UP[] = {
    0x7,
    0x16,
    0x19,
    0x1A,
    0x1B,
    0xffff
};

const u16 VeilstoneDeptStoreStock_4F_DOWN[] = {
    0x73,
    0x74,
    0x75,
    0x77,
    0x78,
    0x79,
    0xffff
};

const u16 *VeilstoneDeptStoreDecorationStocks[] = {
    VeilstoneDeptStoreStock_4F_UP,
    VeilstoneDeptStoreStock_4F_DOWN
};

const u16 SunyshoreMarketStockMonday[] = {
    HEART_SEAL_A,
    STAR_SEAL_B,
    FIRE_SEAL_A,
    SONG_SEAL_A,
    LINE_SEAL_C,
    ELE_SEAL_B,
    PARTY_SEAL_D,
    0xffff
};

const u16 SunyshoreMarketStockTuesday[] = {
    HEART_SEAL_B,
    STAR_SEAL_C,
    FIRE_SEAL_B,
    FLORA_SEAL_A,
    SONG_SEAL_B,
    LINE_SEAL_D,
    ELE_SEAL_C,
    0xffff
};

const u16 SunyshoreMarketStockWednesday[] = {
    HEART_SEAL_C,
    STAR_SEAL_D,
    FIRE_SEAL_C,
    FLORA_SEAL_B,
    SONG_SEAL_C,
    SMOKE_SEAL_A,
    ELE_SEAL_D,
    0xffff
};

const u16 SunyshoreMarketStockThursday[] = {
    HEART_SEAL_D,
    FOAMY_SEAL_A,
    FIRE_SEAL_D,
    FLORA_SEAL_C,
    SONG_SEAL_D,
    STAR_SEAL_E,
    SMOKE_SEAL_B,
    0xffff
};

const u16 SunyshoreMarketStockFriday[] = {
    FOAMY_SEAL_B,
    PARTY_SEAL_A,
    FLORA_SEAL_D,
    SONG_SEAL_E,
    HEART_SEAL_E,
    STAR_SEAL_F,
    SMOKE_SEAL_C,
    0xffff
};

const u16 SunyshoreMarketStockSaturday[] = {
    FOAMY_SEAL_C,
    PARTY_SEAL_B,
    FLORA_SEAL_E,
    SONG_SEAL_F,
    HEART_SEAL_F,
    LINE_SEAL_A,
    SMOKE_SEAL_D,
    0xffff
};

const u16 SunyshoreMarketStockSunday[] = {
    STAR_SEAL_A,
    SONG_SEAL_G,
    FOAMY_SEAL_D,
    FLORA_SEAL_F,
    LINE_SEAL_B,
    ELE_SEAL_A,
    PARTY_SEAL_C,
    0xffff
};

const u16 *SunyshoreMarketDailyStocks[] = {
    SunyshoreMarketStockMonday,
    SunyshoreMarketStockTuesday,
    SunyshoreMarketStockWednesday,
    SunyshoreMarketStockThursday,
    SunyshoreMarketStockFriday,
    SunyshoreMarketStockSaturday,
    SunyshoreMarketStockSunday
};
