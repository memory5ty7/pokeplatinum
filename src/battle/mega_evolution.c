#include "battle/mega_evolution.h"
#include "constants/forms.h"
#include "pokemon.h"
#include "generated/items.h"
#include "generated/species.h"
#include "struct_defs/pokemon_mega_data.h"
#include "res/battle/scripts/sub_seq.naix.h"
#include "battle/battle_lib.h"
#include "battle/ov16_0223DF00.h"
#include "desmume.h"

const MegaEvolutionData sMegaEvolutionTable[] = {
    {SPECIES_VENUSAUR, VENUSAUR_FORM_MEGA, ITEM_VENUSAURITE},
    {SPECIES_CHARIZARD, CHARIZARD_FORM_MEGA_X, ITEM_CHARIZARDITE_X},
    {SPECIES_CHARIZARD, CHARIZARD_FORM_MEGA_Y, ITEM_CHARIZARDITE_Y},
    {SPECIES_BLASTOISE, BLASTOISE_FORM_MEGA, ITEM_BLASTOISINITE},
    {SPECIES_BEEDRILL, BEEDRILL_FORM_MEGA, ITEM_BEEDRILLITE},
    {SPECIES_PIDGEOT, PIDGEOT_FORM_MEGA, ITEM_PIDGEOTITE},
    {SPECIES_RAICHU, RAICHU_FORM_MEGA_X, ITEM_RAICHUNITE_X},
    {SPECIES_RAICHU, RAICHU_FORM_MEGA_Y, ITEM_RAICHUNITE_Y},
    {SPECIES_CLEFABLE, CLEFABLE_FORM_MEGA, ITEM_CLEFABLITE},
    {SPECIES_ALAKAZAM, ALAKAZAM_FORM_MEGA, ITEM_ALAKAZITE},
    {SPECIES_VICTREEBEL, VICTREEBEL_FORM_MEGA, ITEM_VICTREEBELITE},
    {SPECIES_SLOWBRO, SLOWBRO_FORM_MEGA, ITEM_SLOWBRONITE},
    {SPECIES_GENGAR, GENGAR_FORM_MEGA, ITEM_GENGARITE},
    {SPECIES_KANGASKHAN, KANGASKHAN_FORM_MEGA, ITEM_KANGASKHANITE},
    {SPECIES_STARMIE, STARMIE_FORM_MEGA, ITEM_STARMINITE},
    {SPECIES_PINSIR, PINSIR_FORM_MEGA, ITEM_PINSIRITE},
    {SPECIES_GYARADOS, GYARADOS_FORM_MEGA, ITEM_GYARADOSITE},
    {SPECIES_AERODACTYL, AERODACTYL_FORM_MEGA, ITEM_AERODACTYLITE},
    {SPECIES_DRAGONITE, DRAGONITE_FORM_MEGA, ITEM_DRAGONINITE},
    {SPECIES_MEWTWO, MEWTWO_FORM_MEGA_X, ITEM_MEWTWONITE_X},
    {SPECIES_MEWTWO, MEWTWO_FORM_MEGA_Y, ITEM_MEWTWONITE_Y},
    {SPECIES_MEGANIUM, MEGANIUM_FORM_MEGA, ITEM_MEGANIUMITE},
    {SPECIES_FERALIGATR, FERALIGATR_FORM_MEGA, ITEM_FERALIGITE},
    {SPECIES_AMPHAROS, AMPHAROS_FORM_MEGA, ITEM_AMPHAROSITE},
    {SPECIES_STEELIX, STEELIX_FORM_MEGA, ITEM_STEELIXITE},
    {SPECIES_SCIZOR, SCIZOR_FORM_MEGA, ITEM_SCIZORITE},
    {SPECIES_HERACROSS, HERACROSS_FORM_MEGA, ITEM_HERACRONITE},
    {SPECIES_SKARMORY, SKARMORY_FORM_MEGA, ITEM_SKARMORITE},
    {SPECIES_HOUNDOOM, HOUNDOOM_FORM_MEGA, ITEM_HOUNDOOMINITE},
    {SPECIES_TYRANITAR, TYRANITAR_FORM_MEGA, ITEM_TYRANITARITE},
    {SPECIES_SCEPTILE, SCEPTILE_FORM_MEGA, ITEM_SCEPTILITE},
    {SPECIES_BLAZIKEN, BLAZIKEN_FORM_MEGA, ITEM_BLAZIKENITE },
    {SPECIES_SWAMPERT , SWAMPERT_FORM_MEGA, ITEM_SWAMPERTITE },
    {SPECIES_GARDEVOIR , GARDEVOIR_FORM_MEGA, ITEM_GARDEVOIRITE },
    {SPECIES_SABLEYE , SABLEYE_FORM_MEGA, ITEM_SABLENITE },
 	{SPECIES_MAWILE , MAWILE_FORM_MEGA, ITEM_MAWILITE },
 	{SPECIES_AGGRON , AGGRON_FORM_MEGA, ITEM_AGGRONITE },
 	{SPECIES_MEDICHAM , MEDICHAM_FORM_MEGA ,ITEM_MEDICHAMITE },
 	{SPECIES_MANECTRIC , MANECTRIC_FORM_MEGA , ITEM_MANECTITE },
 	{SPECIES_SHARPEDO , SHARPEDO_FORM_MEGA ,ITEM_SHARPEDONITE },
 	{SPECIES_CAMERUPT , CAMERUPT_FORM_MEGA ,ITEM_CAMERUPTITE },
 	{SPECIES_ALTARIA , ALTARIA_FORM_MEGA ,ITEM_ALTARIANITE },
 	{SPECIES_BANETTE , BANETTE_FORM_MEGA ,ITEM_BANETTITE },
 	{SPECIES_CHIMECHO , CHIMECHO_FORM_MEGA, ITEM_CHIMECHITE },
    {SPECIES_ABSOL , ABSOL_FORM_MEGA, ITEM_ABSOLITE },
    {SPECIES_ABSOL , ABSOL_FORM_MEGA_Z, ITEM_ABSOLITE_Z },
    {SPECIES_GLALIE , GLALIE_FORM_MEGA, ITEM_GLALITITE },
    {SPECIES_SALAMENCE , SALAMENCE_FORM_MEGA, ITEM_SALAMENCITE },
    {SPECIES_METAGROSS , METAGROSS_FORM_MEGA, ITEM_METAGROSSITE },
    {SPECIES_LATIAS , LATIAS_FORM_MEGA, ITEM_LATIASITE },
    {SPECIES_LATIOS , LATIOS_FORM_MEGA, ITEM_LATIOSITE },
    {SPECIES_STARAPTOR , STARAPTOR_FORM_MEGA, ITEM_STARAPTITE },
    {SPECIES_LOPUNNY , LOPUNNY_FORM_MEGA, ITEM_LOPUNNITE },
    {SPECIES_GARCHOMP , GARCHOMP_FORM_MEGA, ITEM_GARCHOMPITE },
    {SPECIES_GARCHOMP , GARCHOMP_FORM_MEGA_Z, ITEM_GARCHOMPITE_Z },
    {SPECIES_LUCARIO , LUCARIO_FORM_MEGA, ITEM_LUCARIONITE },
    {SPECIES_LUCARIO , LUCARIO_FORM_MEGA_Z, ITEM_LUCARIONITE_Z },
    {SPECIES_ABOMASNOW , ABOMASNOW_FORM_MEGA, ITEM_ABOMASITE },
    {SPECIES_GALLADE , GALLADE_FORM_MEGA, ITEM_GALLADITE }, 
    {SPECIES_FROSLASS , FROSLASS_FORM_MEGA, ITEM_FROSLASSITE },
    {SPECIES_HEATRAN , HEATRAN_FORM_MEGA, ITEM_HEATRANITE },
    {SPECIES_DARKRAI , DARKRAI_FORM_MEGA, ITEM_DARKRANITE },
};

BOOL BattleMon_CanMegaEvolve(BattleContext *battleCtx, u8 battler)
{

    if (battleCtx->megaEvolutionUsed[battler])
    {
        return FALSE;
    }

    int species = BattleMon_Get(battleCtx, battler, BATTLEMON_SPECIES, NULL);
    int heldItem = BattleMon_Get(battleCtx, battler, BATTLEMON_HELD_ITEM, NULL);
    int currentForm = BattleMon_Get(battleCtx, battler, BATTLEMON_FORM_NUM, NULL);

    // Cannot mega evolve if already mega evolved
    if (currentForm != 0) {
        return FALSE;
    }

    // Check if this species + item combination exists in the table
    for (int i = 0; i < sizeof(sMegaEvolutionTable) / sizeof(sMegaEvolutionTable[0]); i++) {
        if (sMegaEvolutionTable[i].baseSpecies == species && 
            sMegaEvolutionTable[i].requiredItem == heldItem) {
            return TRUE;
        }
    }

    return FALSE;
}

u16 GetMegaEvolutionData(BattleContext *battleCtx, u8 battler)
{
    int species = BattleMon_Get(battleCtx, battler, BATTLEMON_SPECIES, NULL);
    int heldItem = BattleMon_Get(battleCtx, battler, BATTLEMON_HELD_ITEM, NULL);

    for (int i = 0; i < sizeof(sMegaEvolutionTable) / sizeof(sMegaEvolutionTable[0]); i++) {
        if (sMegaEvolutionTable[i].baseSpecies == species && 
            sMegaEvolutionTable[i].requiredItem == heldItem) {
            return sMegaEvolutionTable[i].megaForm;
        }
    }

    return 0;
}

BOOL MonIsMega(Pokemon *mon)
{
    int species = Pokemon_GetValue(mon, MON_DATA_SPECIES, NULL);
    int form = Pokemon_GetValue(mon, MON_DATA_FORM, NULL);

    for (int i = 0; i < sizeof(sMegaEvolutionTable) / sizeof(sMegaEvolutionTable[0]); i++) {
        if (sMegaEvolutionTable[i].baseSpecies == species && 
            sMegaEvolutionTable[i].megaForm == form) {
            return TRUE;
        }
    }

    return FALSE;
}

void RevertMegaEvolution(Pokemon *mon)
{
    Pokemon_SetValue(mon, MON_DATA_FORM, 0);
    Pokemon_CalcAbility(mon);
    Pokemon_CalcLevelAndStats(mon);
}

void BattleFormChange(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int form)
{
    Desmume_Log("Form Num : %d\n", form);
    Pokemon *mon = Pokemon_New(HEAP_ID_BATTLE);

    Pokemon_Copy(BattleSystem_PartyPokemon(battleSys, battler, battleCtx->selectedPartySlot[battler]), mon);

    Pokemon_SetValue(mon, MON_DATA_FORM, &form);
    BattleMon_Set(battleCtx, battler, BATTLEMON_FORM_NUM, &form);

    Pokemon_CalcLevelAndStats(mon);

    Pokemon_CalcAbility(mon);
    u16 ability = Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL);
    BattleMon_Set(battleCtx, battler, BATTLEMON_ABILITY, &ability);

    u32 stats[6];
    u32 types[2];

    for (int i = 0; i < 5; i++)
    {
        stats[i] = Pokemon_GetValue(mon, MON_DATA_ATK + i, NULL);
        Desmume_Log("Old Stat : %d | New Stat : %d\n", BattleMon_Get(battleCtx, battler, BATTLEMON_ATTACK + i, NULL), stats[i]);
        BattleMon_Set(battleCtx, battler, BATTLEMON_ATTACK + i, &stats[i]);
    }

    for (int i = 0; i < 2; i++)
    {
        types[i] = Pokemon_GetValue(mon, MON_DATA_TYPE_1 + i, NULL);
        BattleMon_Set(battleCtx, battler, BATTLEMON_TYPE_1 + i, &types[i]);
    }

    Heap_Free(mon);
}

u8 GetSpeciesMegaNumber(int species)
{
    u8 megaNb = 0;

    switch (species) {
    case SPECIES_VENUSAUR:
    case SPECIES_BLASTOISE:
    case SPECIES_BEEDRILL:
    case SPECIES_PIDGEOT:
    case SPECIES_CLEFABLE:
    case SPECIES_ALAKAZAM:
    case SPECIES_VICTREEBEL:
    case SPECIES_SLOWBRO:
    case SPECIES_GENGAR:
    case SPECIES_KANGASKHAN:
    case SPECIES_STARMIE:
    case SPECIES_PINSIR:
    case SPECIES_GYARADOS:
    case SPECIES_AERODACTYL:
    case SPECIES_DRAGONITE:
    case SPECIES_MEGANIUM:
    case SPECIES_FERALIGATR:
    case SPECIES_AMPHAROS:
    case SPECIES_STEELIX:
    case SPECIES_SCIZOR:
    case SPECIES_HERACROSS:
    case SPECIES_SKARMORY:
    case SPECIES_HOUNDOOM:
    case SPECIES_TYRANITAR:
    case SPECIES_SCEPTILE:
    case SPECIES_BLAZIKEN:
    case SPECIES_SWAMPERT:
    case SPECIES_GARDEVOIR:
    case SPECIES_SABLEYE:
    case SPECIES_MAWILE:
    case SPECIES_AGGRON:
    case SPECIES_MEDICHAM:
    case SPECIES_MANECTRIC:
    case SPECIES_SHARPEDO:
    case SPECIES_CAMERUPT:
    case SPECIES_ALTARIA:
    case SPECIES_BANETTE:
    case SPECIES_CHIMECHO:
    case SPECIES_GLALIE:
    case SPECIES_SALAMENCE:
    case SPECIES_METAGROSS:
    case SPECIES_LATIAS:
    case SPECIES_LATIOS:
    case SPECIES_STARAPTOR:
    case SPECIES_LOPUNNY:
    case SPECIES_ABOMASNOW:
    case SPECIES_GALLADE:
    case SPECIES_FROSLASS:
    case SPECIES_HEATRAN:
    case SPECIES_DARKRAI:
        megaNb = 1;
        break;
    case SPECIES_CHARIZARD:
    case SPECIES_RAICHU:
    case SPECIES_MEWTWO:
    case SPECIES_ABSOL:
    case SPECIES_GARCHOMP:
    case SPECIES_LUCARIO:
        megaNb = 2;
        break;
    }

    return megaNb;
}