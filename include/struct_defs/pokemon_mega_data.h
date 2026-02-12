#ifndef POKEPLATINUM_STRUCT_DEFS_POKEMON_MEGA_DATA_H
#define POKEPLATINUM_STRUCT_DEFS_POKEMON_MEGA_DATA_H

#include "nitro/types.h"

typedef struct {
    u16 baseSpecies;      // Base Pokémon species ID
    u16 megaForm;         // Mega form ID
    u16 requiredItem;     // Mega stone item ID
} MegaEvolutionData;

#endif // POKEPLATINUM_STRUCT_DEFS_POKEMON_MEGA_DATA_H
