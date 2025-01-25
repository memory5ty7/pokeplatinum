#ifndef POKEPLATINUM_GLOBAL_H
#define POKEPLATINUM_GLOBAL_H

#include <nitro.h>
#pragma thumb on
#include <nnsys.h>
#include <stddef.h>
#include <string.h>

#include "constants/pokemon.h"

#include "global/assert.h"
#include "global/config.h" // MUST COME BEFORE ANY OTHER GAMEFREAK HEADERS
#include "global/pm_version.h"

#define NELEMS(a)                     (sizeof(a) / sizeof(*(a)))
#define XtOffset(pointer_type, field) ((unsigned int)&(((pointer_type)NULL)->field))

#define ALIGN_4 __attribute__((aligned(4)))

typedef struct {
    int x;
    int y;
    int z;
} Vec32;

#endif // POKEPLATINUM_GLOBAL_H
