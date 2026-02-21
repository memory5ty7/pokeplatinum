#ifndef POKEPLATINUM_BATTLE_MEGA_EVOLUTION_H
#define POKEPLATINUM_BATTLE_MEGA_EVOLUTION_H

#include "struct_decls/battle_system.h"

#include "battle/battle_context.h"
#include "pokemon.h"

/**
 * @brief Check if a Pokémon can mega evolve
 * 
 * Checks if the Pokémon's species and held item match an entry
 * in the mega evolution data table.
 * 
 * @param battleCtx Pointer to the battle context
 * @param battler The battler ID of the Pokémon to check
 * @return TRUE if the Pokémon can mega evolve, FALSE otherwise
 */
BOOL BattleMon_CanMegaEvolve(BattleContext *battleCtx, u8 battler);

/**
 * @brief Get mega evolution data for a Pokémon
 * 
 * Searches the mega evolution table for a matching entry.
 * 
 * @param mon Pointer to the Pokémon to check
 * @return The mega form ID if found, 0 otherwise
 */
u16 GetMegaEvolutionData(BattleContext *battleCtx, u8 battler);

BOOL MonIsMega(Pokemon *mon);

void RevertMegaEvolution(Pokemon *mon);

void BattleFormChange(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int form);

u8 GetSpeciesMegaNumber(int species);

#endif // POKEPLATINUM_BATTLE_MEGA_EVOLUTION_H
