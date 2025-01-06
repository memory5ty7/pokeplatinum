#include "pokedex_data.h"

#include <nitro.h>
#include <string.h>

#include "constants/species.h"

#include "heap.h"
#include "inlines.h"
#include "pokedex_language.h"
#include "pokemon.h"
#include "savedata.h"

static const u16 sExcludedMonsNational[] = {
    SPECIES_MEW,
    SPECIES_LUGIA,
    SPECIES_HO_OH,
    SPECIES_CELEBI,
    SPECIES_JIRACHI,
    SPECIES_DEOXYS,
    SPECIES_PHIONE,
    SPECIES_MANAPHY,
    SPECIES_DARKRAI,
    SPECIES_SHAYMIN,
    SPECIES_ARCEUS
};
static const u16 sExcludedMonsLocal[] = {};

#define DEX_SIZE_U32          ((int)((NATIONAL_DEX_COUNT - 1) / 32) + 1) // default 16
#define MAGIC_NUMBER          0xBEEFCAFE
#define NUM_EXCLUDED_NATIONAL ((int)(sizeof(sExcludedMonsNational) / sizeof(u16)))
#define NUM_EXCLUDED_LOCAL    0 //((int)(sizeof(sExcludedMonsLocal) / sizeof(u16)))
#define NATIONAL_DEX_GOAL     (NATIONAL_DEX_COUNT - NUM_EXCLUDED_NATIONAL)
#define LOCAL_DEX_GOAL        (LOCAL_DEX_COUNT - NUM_EXCLUDED_LOCAL)
#define UNOWN_COUNT           28
#define DEOXYS_COUNT          4
#define ROTOM_COUNT           6

typedef struct PokedexData {
    u32 magic;
    u32 caughtPokemon[DEX_SIZE_U32];
    u32 seenPokemon[DEX_SIZE_U32];
    u32 recordedGenders[2][DEX_SIZE_U32];
    u32 spindaForm;
    u8 shellosFormsSeen;
    u8 gastrodonFormsSeen;
    u8 burmyFormsSeen;
    u8 wormadamFormsSeen;
    u8 unownFormsSeen[UNOWN_COUNT];
    u8 recordedLanguages[MAX_SPECIES + 1];
    u8 canDetectForms;
    u8 canDetectLanguages;
    u8 pokedexObtained;
    u8 nationalDexObtained;
    u32 rotomFormsSeen;
    u8 shayminFormsSeen;
    u8 giratinaFormsSeen;
} PokedexData;

int PokedexData_SaveSize(void)
{
    return sizeof(PokedexData);
}

PokedexData *PokedexData_Alloc(u32 heapID)
{
    PokedexData *pokedexData = Heap_AllocFromHeap(heapID, sizeof(PokedexData));
    PokedexData_Init(pokedexData);

    return pokedexData;
}

void PokedexData_Copy(const PokedexData *src, PokedexData *dest)
{
    MI_CpuCopy8(src, dest, sizeof(PokedexData));
}

static inline void CheckPokedexIntegrity(const PokedexData *pokedexData)
{
    GF_ASSERT(pokedexData->magic == MAGIC_NUMBER);
}

static BOOL SpeciesInvalid(u16 species)
{
    if (species == SPECIES_NONE || species > NATIONAL_DEX_COUNT) {
        GF_ASSERT(FALSE);
        return TRUE;
    } else {
        return FALSE;
    }
}

static inline BOOL ReadBit_2Forms(const u8 *array, u16 bitIndex)
{
    bitIndex--;
    return 0 != (array[bitIndex >> 3] & (1 << (bitIndex & 0x07)));
}

static inline void ActivateBit_2Forms(u8 *array, u16 bitIndex)
{
    bitIndex--;
    array[bitIndex >> 3] |= 1 << (bitIndex & 0x07);
}

static inline void SetBit_2Forms(u8 *array, u8 value, u16 bitIndex)
{
    GF_ASSERT(value < 2);

    bitIndex--;

    array[bitIndex >> 3] &= ~(1 << (bitIndex & 0x07));
    array[bitIndex >> 3] |= value << (bitIndex & 0x07);
}

static inline u32 ReadBit_3Forms(const u8 *array, u16 bitIndex)
{
    return (array[bitIndex >> 2] >> ((bitIndex & 0x03) * 2)) & 0x03;
}

static inline void SetBit_3Forms(u8 *array, u8 value, u16 bitIndex)
{
    GF_ASSERT(value < 4);

    array[bitIndex >> 2] &= ~(0x03 << ((bitIndex & 0x03) * 2));
    array[bitIndex >> 2] |= value << ((bitIndex & 0x03) * 2);
}

static inline void Write_SeenSpecies(PokedexData *pokedexData, u16 species)
{
    ActivateBit_2Forms((u8 *)pokedexData->seenPokemon, species);
}

static inline void Write_CaughtSpecies(PokedexData *pokedexData, u16 species)
{
    ActivateBit_2Forms((u8 *)pokedexData->caughtPokemon, species);
}

static void SetBit_Gender(PokedexData *pokedexData, u8 gender, u8 isSeen, u16 bitIndex)
{
    if (isSeen == FALSE) {
        SetBit_2Forms((u8 *)pokedexData->recordedGenders[1], gender, bitIndex);
    }

    SetBit_2Forms((u8 *)pokedexData->recordedGenders[isSeen], gender, bitIndex);
}

static void UpdateGender(PokedexData *pokedexData, u8 gender, u8 isSeen, u16 bitIndex)
{
    GF_ASSERT(gender <= GENDER_NONE);

    if (gender == GENDER_NONE) {
        gender = GENDER_MALE;
    }

    SetBit_Gender(pokedexData, gender, isSeen, bitIndex);
}

static inline BOOL SpeciesSeen(const PokedexData *pokedexData, u16 species)
{
    return ReadBit_2Forms((const u8 *)pokedexData->seenPokemon, species);
}

static inline BOOL SpeciesCaught(const PokedexData *pokedexData, u16 species)
{
    return ReadBit_2Forms((const u8 *)pokedexData->caughtPokemon, species);
}

static inline u8 GetGender(const PokedexData *pokedexData, u16 species, u8 bitIndex)
{
    return ReadBit_2Forms((const u8 *)pokedexData->recordedGenders[bitIndex], species);
}

static inline void SetForm_Spinda(PokedexData *pokedexData, u16 species, u32 personality)
{
    if (species == SPECIES_SPINDA) {
        pokedexData->spindaForm = personality;
    }
}

static int NumFormsSeen_Unown(const PokedexData *pokedexData)
{
    int formIndex;

    for (formIndex = 0; formIndex < UNOWN_COUNT; formIndex++) {
        if (pokedexData->unownFormsSeen[formIndex] == 0xFF) {
            break;
        }
    }

    return formIndex;
}

static BOOL UnownFormSeen(const PokedexData *pokedexData, u8 form)
{
    for (int formIndex = 0; formIndex < UNOWN_COUNT; formIndex++) {
        if (pokedexData->unownFormsSeen[formIndex] == form) {
            return TRUE;
        }
    }

    return FALSE;
}

static void SetUnownForm(PokedexData *pokedexData, int form)
{
    if (UnownFormSeen(pokedexData, form)) {
        return;
    }

    int numUnownSeen = NumFormsSeen_Unown(pokedexData);

    if (numUnownSeen < UNOWN_COUNT) {
        pokedexData->unownFormsSeen[numUnownSeen] = form;
    }
}

static int NumFormsSeen_TwoForms(const PokedexData *pokedexData, u32 species)
{
    GF_ASSERT((species == SPECIES_SHELLOS) || (species == SPECIES_GASTRODON) || (species == SPECIES_SHAYMIN) || (species == SPECIES_GIRATINA));

    if (PokedexData_HasSeenSpecies(pokedexData, species) == FALSE) {
        return 0;
    }

    const u8 *formArray;
    switch (species) {
    case SPECIES_SHELLOS:
        formArray = &pokedexData->shellosFormsSeen;
        break;
    case SPECIES_GASTRODON:
        formArray = &pokedexData->gastrodonFormsSeen;
        break;
    case SPECIES_SHAYMIN:
        formArray = &pokedexData->shayminFormsSeen;
        break;
    case SPECIES_GIRATINA:
        formArray = &pokedexData->giratinaFormsSeen;
        break;
    }

    u32 form_1 = ReadBit_2Forms(formArray, 1);
    u32 form_2 = ReadBit_2Forms(formArray, 2);

    if (form_1 == form_2) {
        return 1;
    }

    return 2;
}

static BOOL FormSeen_TwoForms(const PokedexData *pokedexData, u32 species, u8 form)
{
    GF_ASSERT((species == SPECIES_SHELLOS) || (species == SPECIES_GASTRODON) || (species == SPECIES_SHAYMIN) || (species == SPECIES_GIRATINA));

    if (PokedexData_HasSeenSpecies(pokedexData, species) == FALSE) {
        return FALSE;
    }

    const u8 *formArray;
    switch (species) {
    case SPECIES_SHELLOS:
        formArray = &pokedexData->shellosFormsSeen;
        break;
    case SPECIES_GASTRODON:
        formArray = &pokedexData->gastrodonFormsSeen;
        break;
    case SPECIES_SHAYMIN:
        formArray = &pokedexData->shayminFormsSeen;
        break;
    case SPECIES_GIRATINA:
        formArray = &pokedexData->giratinaFormsSeen;
        break;
    }

    u32 numFormsSeen = NumFormsSeen_TwoForms(pokedexData, species);

    for (u32 formIndex = 0; formIndex < numFormsSeen; formIndex++) {
        u32 currentForm = ReadBit_2Forms(formArray, formIndex + 1);

        if (currentForm == form) {
            return TRUE;
        }
    }

    return FALSE;
}

static void UpdateForms_TwoForms(PokedexData *pokedexData, u32 species, int form)
{
    GF_ASSERT((species == SPECIES_SHELLOS) || (species == SPECIES_GASTRODON) || (species == SPECIES_SHAYMIN) || (species == SPECIES_GIRATINA));

    if (FormSeen_TwoForms(pokedexData, species, form)) {
        return;
    }

    u8 *formArray;
    switch (species) {
    case SPECIES_SHELLOS:
        formArray = &pokedexData->shellosFormsSeen;
        break;
    case SPECIES_GASTRODON:
        formArray = &pokedexData->gastrodonFormsSeen;
        break;
    case SPECIES_SHAYMIN:
        formArray = &pokedexData->shayminFormsSeen;
        break;
    case SPECIES_GIRATINA:
        formArray = &pokedexData->giratinaFormsSeen;
        break;
    }

    int numFormsSeen = NumFormsSeen_TwoForms(pokedexData, species);

    if (numFormsSeen < 2) {
        SetBit_2Forms(formArray, form, numFormsSeen + 1);

        if (numFormsSeen == 0) {
            SetBit_2Forms(formArray, form, numFormsSeen + 2);
        }
    }
}

static int NumFormsSeen_ThreeForms(const PokedexData *pokedexData, u32 species)
{
    GF_ASSERT((species == SPECIES_BURMY) || (species == SPECIES_WORMADAM));

    if (PokedexData_HasSeenSpecies(pokedexData, species) == FALSE) {
        return 0;
    }

    const u8 *formArray;
    if (species == SPECIES_BURMY) {
        formArray = &pokedexData->burmyFormsSeen;
    } else {
        formArray = &pokedexData->wormadamFormsSeen;
    }

    int formIndex;
    for (formIndex = 0; formIndex < 3; formIndex++) {
        u32 currentForm = ReadBit_3Forms(formArray, formIndex);

        if (currentForm == 3) {
            break;
        }
    }

    return formIndex;
}

static BOOL FormSeen_ThreeForms(const PokedexData *pokedexData, u32 species, u8 form)
{
    GF_ASSERT((species == SPECIES_BURMY) || (species == SPECIES_WORMADAM));

    if (PokedexData_HasSeenSpecies(pokedexData, species) == FALSE) {
        return FALSE;
    }

    const u8 *formArray;
    if (species == SPECIES_BURMY) {
        formArray = &pokedexData->burmyFormsSeen;
    } else {
        formArray = &pokedexData->wormadamFormsSeen;
    }

    for (int formIndex = 0; formIndex < 3; formIndex++) {
        u32 currentForm = ReadBit_3Forms(formArray, formIndex);

        if (currentForm == form) {
            return TRUE;
        }
    }

    return FALSE;
}

static void UpdateForms_ThreeForms(PokedexData *pokedexData, u32 species, int form)
{
    GF_ASSERT((species == SPECIES_BURMY) || (species == SPECIES_WORMADAM));

    if (FormSeen_ThreeForms(pokedexData, species, form)) {
        return;
    }

    u8 *formArray;
    if (species == SPECIES_BURMY) {
        formArray = &pokedexData->burmyFormsSeen;
    } else {
        formArray = &pokedexData->wormadamFormsSeen;
    }

    int numFormsSeen = NumFormsSeen_ThreeForms(pokedexData, species);

    if (numFormsSeen < 3) {
        SetBit_3Forms(formArray, form, numFormsSeen);
    }
}

static void WriteBit_Deoxys(u32 *array, u8 value, u8 bitIndex)
{
    u32 bitOffset = (24 + (bitIndex * 4));
    u32 emptyBits = ~(0x0F << bitOffset);

    array[DEX_SIZE_U32 - 1] &= emptyBits;
    array[DEX_SIZE_U32 - 1] |= (value << bitOffset);
}

static void UpdateFormArray_Deoxys(PokedexData *pokedexData, u8 form, u8 bitIndex)
{
    // Deoxys forms are currently stored in spare bits in these arrays
    // This will want to be changed when modding to avoid overlapping references

    GF_ASSERT(bitIndex < DEOXYS_COUNT);
    GF_ASSERT(form <= 0x0F);

    if (bitIndex < 2) {
        WriteBit_Deoxys(pokedexData->caughtPokemon, form, bitIndex);
    } else {
        WriteBit_Deoxys(pokedexData->seenPokemon, form, bitIndex - 2);
    }
}

static inline u32 ReadBit_Deoxys(const u32 *array, u8 bitIndex)
{
    u32 bitOffset = (24 + (bitIndex * 4));
    return (array[DEX_SIZE_U32 - 1] >> bitOffset) & 0x0F;
}

static u32 GetForm_Deoxys(const PokedexData *pokedexData, u8 formIndex)
{
    // Deoxys forms are currently stored in spare bits in these arrays
    // This will want to be changed when modding to avoid overlapping references

    u32 form;
    if (formIndex < 2) {
        form = ReadBit_Deoxys(pokedexData->caughtPokemon, formIndex);
    } else {
        form = ReadBit_Deoxys(pokedexData->seenPokemon, formIndex - 2);
    }

    return form;
}

static u32 NumFormsSeen_Deoxys(const PokedexData *pokedexData)
{
    int formIndex;

    for (formIndex = 0; formIndex < DEOXYS_COUNT; formIndex++) {
        if (GetForm_Deoxys(pokedexData, formIndex) == 0x0F) {
            break;
        }
    }

    return formIndex;
}

static BOOL FormSeen_Deoxys(const PokedexData *pokedexData, u32 form)
{
    for (int formIndex = 0; formIndex < DEOXYS_COUNT; formIndex++) {
        if (GetForm_Deoxys(pokedexData, formIndex) == form) {
            return TRUE;
        }
    }

    return FALSE;
}

static void UpdateForms_Deoxys(PokedexData *pokedexData, u16 species, Pokemon *pokemon)
{
    u8 form = Pokemon_GetValue(pokemon, MON_DATA_FORM, NULL);

    if (species == SPECIES_DEOXYS) {
        if (FormSeen_Deoxys(pokedexData, form) == FALSE) {
            u32 newIndex = NumFormsSeen_Deoxys(pokedexData);
            UpdateFormArray_Deoxys(pokedexData, form, newIndex);
        }
    }
}

static void InitDeoxys(PokedexData *pokedexData)
{
    for (int formIndex = 0; formIndex < DEOXYS_COUNT; formIndex++) {
        UpdateFormArray_Deoxys(pokedexData, 0x0F, formIndex);
    }
}

static inline u32 ReadBit_Rotom(u32 formArray, u32 formIndex)
{
    return (formArray >> (formIndex * 3)) & 0x07;
}

static inline void SetBit_Rotom(u32 *formArray, u32 formIndex, u32 form)
{
    GF_ASSERT(form < 0x07);

    (*formArray) &= ~(0x07 << (formIndex * 3));
    (*formArray) |= (form << (formIndex * 3));
}

static int NumFormsSeen_Rotom(const PokedexData *pokedexData, u32 species)
{
    GF_ASSERT(species == SPECIES_ROTOM);

    if (PokedexData_HasSeenSpecies(pokedexData, species) == FALSE) {
        return 0;
    }

    u32 form;
    int formIndex;
    int numFormsSeen = 0;

    for (formIndex = 0; formIndex < ROTOM_COUNT; formIndex++) {
        form = ReadBit_Rotom(pokedexData->rotomFormsSeen, formIndex);

        if (form != 0x07) {
            numFormsSeen++;
        } else {
            break;
        }
    }

    return numFormsSeen;
}

static BOOL FormSeen_Rotom(const PokedexData *pokedexData, u32 species, u8 form)
{
    GF_ASSERT(species == SPECIES_ROTOM);

    if (PokedexData_HasSeenSpecies(pokedexData, species) == FALSE) {
        return FALSE;
    }

    int formIndex;
    u32 numFormsSeen = NumFormsSeen_Rotom(pokedexData, species);

    for (formIndex = 0; formIndex < numFormsSeen; formIndex++) {
        u32 rotomForm = ReadBit_Rotom(pokedexData->rotomFormsSeen, formIndex);

        if (rotomForm == form) {
            return TRUE;
        }
    }

    return FALSE;
}

static void UpdateForms_Rotom(PokedexData *pokedexData, u32 species, int form)
{
    int numFormsSeen;

    GF_ASSERT(species == SPECIES_ROTOM);

    if (FormSeen_Rotom(pokedexData, species, form)) {
        return;
    }

    numFormsSeen = NumFormsSeen_Rotom(pokedexData, species);

    if (numFormsSeen < ROTOM_COUNT) {
        SetBit_Rotom(&pokedexData->rotomFormsSeen, numFormsSeen, form);
    }
}

static void UpdateForm(PokedexData *pokedexData, u16 species, Pokemon *pokemon)
{
    int form;

    switch (species) {
    case SPECIES_UNOWN:
        form = Pokemon_GetForm(pokemon);
        SetUnownForm(pokedexData, form);
        break;
    case SPECIES_BURMY:
        form = Pokemon_GetValue(pokemon, MON_DATA_FORM, NULL);
        UpdateForms_ThreeForms(pokedexData, species, form);
        break;
    case SPECIES_WORMADAM:
        form = Pokemon_GetValue(pokemon, MON_DATA_FORM, NULL);
        UpdateForms_ThreeForms(pokedexData, species, form);
        break;
    case SPECIES_SHELLOS:
        form = Pokemon_GetValue(pokemon, MON_DATA_FORM, NULL);
        UpdateForms_TwoForms(pokedexData, species, form);
        break;
    case SPECIES_GASTRODON:
        form = Pokemon_GetValue(pokemon, MON_DATA_FORM, NULL);
        UpdateForms_TwoForms(pokedexData, species, form);
        break;
    case SPECIES_DEOXYS:
        UpdateForms_Deoxys(pokedexData, species, pokemon);
        break;
    case SPECIES_SHAYMIN:
        form = Pokemon_GetValue(pokemon, MON_DATA_FORM, NULL);
        UpdateForms_TwoForms(pokedexData, species, form);
        break;
    case SPECIES_GIRATINA:
        form = Pokemon_GetValue(pokemon, MON_DATA_FORM, NULL);
        UpdateForms_TwoForms(pokedexData, species, form);
        break;
    case SPECIES_ROTOM:
        form = Pokemon_GetValue(pokemon, MON_DATA_FORM, NULL);
        UpdateForms_Rotom(pokedexData, species, form);
        break;
    }
}

static void UpdateLanguage(PokedexData *pokedexData, u16 species, u32 language)
{
    int bitIndex = species;
    int languageIndex = PokedexLanguage_LanguageToIndex(language);

    if (languageIndex == NUM_LANGUAGES) {
        return;
    }

    pokedexData->recordedLanguages[bitIndex] |= 1 << languageIndex;
}

static u32 GetDisplayGender(const PokedexData *pokedexData, u16 species, int displaySecondary)
{
    u32 defaultGender, secondaryGender;
    u32 displayGender;

    if (PokemonPersonalData_GetSpeciesValue(species, MON_DATA_PERSONAL_GENDER) == GENDER_RATIO_NO_GENDER) {
        if (displaySecondary == FALSE) {
            return GENDER_NONE;
        } else {
            return -1;
        }
    }

    defaultGender = GetGender(pokedexData, species, 0);

    if (displaySecondary == TRUE) {
        secondaryGender = GetGender(pokedexData, species, 1);

        if (secondaryGender == defaultGender) {
            displayGender = -1;
        } else {
            displayGender = secondaryGender;
        }
    } else {
        displayGender = defaultGender;
    }

    return displayGender;
}

static inline int GetForm_Unown(const PokedexData *pokedexData, int formIndex)
{
    return pokedexData->unownFormsSeen[formIndex];
}

static int GetForm_TwoForms(const PokedexData *pokedexData, u32 species, int formIndex)
{
    const u8 *formArray;

    GF_ASSERT((species == SPECIES_SHELLOS) || (species == SPECIES_GASTRODON) || (species == SPECIES_SHAYMIN) || (species == SPECIES_GIRATINA));
    GF_ASSERT(formIndex < 2);

    switch (species) {
    case SPECIES_SHELLOS:
        formArray = &pokedexData->shellosFormsSeen;
        break;
    case SPECIES_GASTRODON:
        formArray = &pokedexData->gastrodonFormsSeen;
        break;
    case SPECIES_SHAYMIN:
        formArray = &pokedexData->shayminFormsSeen;
        break;
    case SPECIES_GIRATINA:
        formArray = &pokedexData->giratinaFormsSeen;
        break;
    }

    return ReadBit_2Forms(formArray, formIndex + 1);
}

static int GetForm_Rotom(const PokedexData *pokedexData, u32 species, int formIndex)
{
    GF_ASSERT(species == SPECIES_ROTOM);
    GF_ASSERT(formIndex < ROTOM_COUNT);

    return ReadBit_Rotom(pokedexData->rotomFormsSeen, formIndex);
}

static int GetForm_3Forms(const PokedexData *pokedexData, u32 species, int formIndex)
{
    const u8 *formArray;

    GF_ASSERT((species == SPECIES_BURMY) || (species == SPECIES_WORMADAM));
    GF_ASSERT(formIndex < 3);

    if (species == SPECIES_BURMY) {
        formArray = &pokedexData->burmyFormsSeen;
    } else {
        formArray = &pokedexData->wormadamFormsSeen;
    }

    return ReadBit_3Forms(formArray, formIndex);
}

static BOOL CountsForDexCompletion_National(u16 species)
{
    int i;
    BOOL included = TRUE;

    for (i = 0; i < NUM_EXCLUDED_NATIONAL; i++) {
        if (sExcludedMonsNational[i] == species) {
            included = FALSE;
        }
    }

    return included;
}

static BOOL CountsForDexCompletion_Local(u16 species)
{
    BOOL included = TRUE;

    for (int i = 0; i < NUM_EXCLUDED_LOCAL; i++) {
        if (sExcludedMonsLocal[i] == species) {
            included = FALSE;
        }
    }

    return included;
}

void PokedexData_Init(PokedexData *pokedexData)
{
    memset(pokedexData, 0, sizeof(PokedexData));

    pokedexData->magic = MAGIC_NUMBER;
    pokedexData->nationalDexObtained = FALSE;

    memset(pokedexData->unownFormsSeen, 0xFF, sizeof(u8) * UNOWN_COUNT);

    pokedexData->shellosFormsSeen = 0xFF;
    pokedexData->gastrodonFormsSeen = 0xFF;
    pokedexData->burmyFormsSeen = 0xFF;
    pokedexData->wormadamFormsSeen = 0xFF;
    pokedexData->rotomFormsSeen = 0xFFFFFFFF;
    pokedexData->shayminFormsSeen = 0xFF;
    pokedexData->giratinaFormsSeen = 0xFF;

    InitDeoxys(pokedexData);
}

u16 PokedexData_CountCaught_National(const PokedexData *pokedexData)
{
    CheckPokedexIntegrity(pokedexData);

    int species;
    int caughtCount = 0;
    for (species = 1; species <= NATIONAL_DEX_COUNT; species++) {
        if (PokedexData_HasCaughtSpecies(pokedexData, species) == TRUE) {
            caughtCount++;
        }
    }

    return caughtCount;
}

u16 PokedexData_CountSeen_National(const PokedexData *pokedex)
{
    CheckPokedexIntegrity(pokedex);

    int species;
    int seenCount = 0;
    for (species = 1; species <= NATIONAL_DEX_COUNT; species++) {
        if (PokedexData_HasSeenSpecies(pokedex, species) == TRUE) {
            seenCount++;
        }
    }

    return seenCount;
}

u16 PokedexData_CountSeen(const PokedexData *pokedex)
{
    if (PokedexData_IsNationalDexObtained(pokedex)) {
        return PokedexData_CountSeen_National(pokedex);
    }

    return PokedexData_CountSeen_Local(pokedex);
}

u16 PokedexData_CountCaught_Local(const PokedexData *pokedexData)
{
    CheckPokedexIntegrity(pokedexData);

    int species;
    int caughtCount = 0;
    for (species = 1; species <= NATIONAL_DEX_COUNT; species++) {
        if (PokedexData_HasCaughtSpecies(pokedexData, species) == TRUE) {
            if (Pokemon_SinnohDexNumber(species) != 0) {
                caughtCount++;
            }
        }
    }

    return caughtCount;
}

u16 PokedexData_CountSeen_Local(const PokedexData *pokedex)
{
    CheckPokedexIntegrity(pokedex);

    int species;
    int seenCount = 0;
    for (species = 1; species <= NATIONAL_DEX_COUNT; species++) {
        if (PokedexData_HasSeenSpecies(pokedex, species) == TRUE && Pokemon_SinnohDexNumber(species) != 0) {
            seenCount++;
        }
    }

    return seenCount;
}

BOOL PokedexData_NationalDexCompleted(const PokedexData *pokedexData)
{
    u16 numCaught = PokedexData_NumCaught_National(pokedexData);

    if (numCaught >= NATIONAL_DEX_GOAL) {
        return TRUE;
    }

    return FALSE;
}

BOOL PokedexData_LocalDexCompleted(const PokedexData *pokedexData)
{
    u16 numCaught = PokedexData_NumCaught_Local(pokedexData);

    if (numCaught >= LOCAL_DEX_GOAL) {
        return TRUE;
    }

    return FALSE;
}

u16 PokedexData_NumCaught_National(const PokedexData *pokedexData)
{
    int species;
    u16 numCaught = 0;

    for (species = 1; species <= NATIONAL_DEX_COUNT; species++) {
        if (PokedexData_HasCaughtSpecies(pokedexData, species) == TRUE) {
            if (CountsForDexCompletion_National(species) == TRUE) {
                numCaught++;
            }
        }
    }

    return numCaught;
}

u16 PokedexData_NumCaught_Local(const PokedexData *pokedexData)
{
    int species;
    u16 numCaught = 0;

    for (species = 1; species <= NATIONAL_DEX_COUNT; species++) {
        if (PokedexData_HasSeenSpecies(pokedexData, species) == TRUE) {
            if (Pokemon_SinnohDexNumber(species) != 0
                && CountsForDexCompletion_Local(species) == TRUE) {
                numCaught++;
            }
        }
    }

    return numCaught;
}

BOOL PokedexData_HasCaughtSpecies(const PokedexData *pokedex, u16 species)
{
    CheckPokedexIntegrity(pokedex);

    if (SpeciesInvalid(species)) {
        return FALSE;
    }

    if (SpeciesCaught(pokedex, species) && SpeciesSeen(pokedex, species)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL PokedexData_HasSeenSpecies(const PokedexData *pokedex, u16 species)
{
    CheckPokedexIntegrity(pokedex);

    if (SpeciesInvalid(species)) {
        return FALSE;
    }

    return SpeciesSeen(pokedex, species);
}

u32 PokedexData_GetForm_Spinda(const PokedexData *pokedexData, u8 formIndex)
{
    CheckPokedexIntegrity(pokedexData);

    u32 form;
    switch (formIndex) {
    case 0:
        form = pokedexData->spindaForm;
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }

    return form;
}

u32 PokedexData_DisplayedGender(const PokedexData *pokedexData, u16 species, int displaySecondary)
{
    CheckPokedexIntegrity(pokedexData);

    if (SpeciesInvalid(species)) {
        return -1;
    }

    if (!SpeciesSeen(pokedexData, species)) {
        return -1;
    }

    return GetDisplayGender(pokedexData, species, displaySecondary);
}

u32 PokedexData_GetForm_Unown(const PokedexData *pokedexData, int formIndex)
{
    CheckPokedexIntegrity(pokedexData);

    if (NumFormsSeen_Unown(pokedexData) <= formIndex) {
        return -1;
    }

    return GetForm_Unown(pokedexData, formIndex);
}

u32 PokedexData_NumFormsSeen_Unown(const PokedexData *pokedexData)
{
    CheckPokedexIntegrity(pokedexData);
    return NumFormsSeen_Unown(pokedexData);
}

u32 PokedexData_GetForm_Shellos(const PokedexData *pokedexData, int formIndex)
{
    CheckPokedexIntegrity(pokedexData);

    if (NumFormsSeen_TwoForms(pokedexData, SPECIES_SHELLOS) <= formIndex) {
        return -1;
    }

    return GetForm_TwoForms(pokedexData, SPECIES_SHELLOS, formIndex);
}

u32 PokedexData_NumFormsSeen_Shellos(const PokedexData *pokedexData)
{
    CheckPokedexIntegrity(pokedexData);
    return NumFormsSeen_TwoForms(pokedexData, SPECIES_SHELLOS);
}

u32 PokedexData_GetForm_Gastrodon(const PokedexData *pokedexData, int formIndex)
{
    CheckPokedexIntegrity(pokedexData);

    if (NumFormsSeen_TwoForms(pokedexData, SPECIES_GASTRODON) <= formIndex) {
        return -1;
    }

    return GetForm_TwoForms(pokedexData, SPECIES_GASTRODON, formIndex);
}

u32 PokedexData_NumFormsSeen_Gastrodon(const PokedexData *pokedexData)
{
    CheckPokedexIntegrity(pokedexData);
    return NumFormsSeen_TwoForms(pokedexData, SPECIES_GASTRODON);
}

u32 PokedexData_GetForm_Burmy(const PokedexData *pokedexData, int formIndex)
{
    CheckPokedexIntegrity(pokedexData);

    if (NumFormsSeen_ThreeForms(pokedexData, SPECIES_BURMY) <= formIndex) {
        return -1;
    }

    return GetForm_3Forms(pokedexData, SPECIES_BURMY, formIndex);
}

u32 PokedexData_NumFormsSeen_Burmy(const PokedexData *pokedexData)
{
    CheckPokedexIntegrity(pokedexData);
    return NumFormsSeen_ThreeForms(pokedexData, SPECIES_BURMY);
}

u32 PokedexData_GetForm_Wormadam(const PokedexData *pokedexData, int formIndex)
{
    CheckPokedexIntegrity(pokedexData);

    if (NumFormsSeen_ThreeForms(pokedexData, SPECIES_WORMADAM) <= formIndex) {
        return -1;
    }

    return GetForm_3Forms(pokedexData, SPECIES_WORMADAM, formIndex);
}

u32 PokedexData_NumFormsSeen_Wormadam(const PokedexData *pokedexData)
{
    CheckPokedexIntegrity(pokedexData);
    return NumFormsSeen_ThreeForms(pokedexData, SPECIES_WORMADAM);
}

u32 PokedexData_GetForm_Deoxys(const PokedexData *pokedexData, int formIndex)
{
    CheckPokedexIntegrity(pokedexData);
    return GetForm_Deoxys(pokedexData, formIndex);
}

u32 PokedexData_NumFormsSeen_Deoxys(const PokedexData *pokedexData)
{
    CheckPokedexIntegrity(pokedexData);
    return NumFormsSeen_Deoxys(pokedexData);
}

void PokedexData_Encounter(PokedexData *pokedexData, Pokemon *pokemon)
{
    u16 species = Pokemon_GetValue(pokemon, MON_DATA_SPECIES, NULL);
    u32 personality = Pokemon_GetValue(pokemon, MON_DATA_PERSONALITY, NULL);
    u32 gender = Pokemon_GetGender(pokemon);

    CheckPokedexIntegrity(pokedexData);

    if (SpeciesInvalid(species)) {
        return;
    }

    if (!SpeciesSeen(pokedexData, species)) {
        SetForm_Spinda(pokedexData, species, personality);
        UpdateGender(pokedexData, gender, FALSE, species);
    } else {
        u32 defaultGender = GetGender(pokedexData, species, 0);

        if (defaultGender != gender) {
            UpdateGender(pokedexData, gender, TRUE, species);
        }
    }

    UpdateForm(pokedexData, species, pokemon);
    Write_SeenSpecies(pokedexData, species);
}

void PokedexData_Capture(PokedexData *pokedexData, Pokemon *pokemon)
{
    u16 species = Pokemon_GetValue(pokemon, MON_DATA_SPECIES, NULL);
    u32 language = Pokemon_GetValue(pokemon, MON_DATA_LANGUAGE, NULL);
    u32 personality = Pokemon_GetValue(pokemon, MON_DATA_PERSONALITY, NULL);
    u32 gender = Pokemon_GetGender(pokemon);

    CheckPokedexIntegrity(pokedexData);

    if (SpeciesInvalid(species)) {
        return;
    }

    if (!SpeciesSeen(pokedexData, species)) {
        SetForm_Spinda(pokedexData, species, personality);
        UpdateGender(pokedexData, gender, FALSE, species);
    } else {
        u32 displayedGender = GetGender(pokedexData, species, 0);

        if (displayedGender != gender) {
            UpdateGender(pokedexData, gender, TRUE, species);
        }
    }

    UpdateForm(pokedexData, species, pokemon);
    UpdateLanguage(pokedexData, species, language);

    Write_CaughtSpecies(pokedexData, species);
    Write_SeenSpecies(pokedexData, species);
}

void PokedexData_ObtainNationalDex(PokedexData *pokedexData)
{
    CheckPokedexIntegrity(pokedexData);
    pokedexData->nationalDexObtained = TRUE;
}

BOOL PokedexData_IsNationalDexObtained(const PokedexData *pokedex)
{
    CheckPokedexIntegrity(pokedex);
    return pokedex->nationalDexObtained;
}

BOOL PokedexData_CanDetectForms(const PokedexData *pokedexData)
{
    CheckPokedexIntegrity(pokedexData);
    return pokedexData->canDetectForms;
}

void PokedexData_TurnOnFormDetection(PokedexData *pokedexData)
{
    CheckPokedexIntegrity(pokedexData);
    pokedexData->canDetectForms = TRUE;
}

BOOL PokedexData_IsLanguageObtained(const PokedexData *pokedexData, u16 species, u32 languageIndex)
{
    int bitIndex;

    GF_ASSERT(languageIndex < 8);

    CheckPokedexIntegrity(pokedexData);

    bitIndex = species;
    languageIndex = PokedexLanguage_LanguageToIndex(languageIndex);

    return pokedexData->recordedLanguages[bitIndex] & (1 << languageIndex);
}

void PokedexData_TurnOnLanguageDetection(PokedexData *pokedexData)
{
    pokedexData->canDetectLanguages = TRUE;
}

BOOL PokedexData_CanDetectLanguages(const PokedexData *pokedexData)
{
    return pokedexData->canDetectLanguages;
}

BOOL PokedexData_IsObtained(const PokedexData *pokedex)
{
    CheckPokedexIntegrity(pokedex);
    return pokedex->pokedexObtained;
}

void PokedexData_ObtainPokedex(PokedexData *pokedex)
{
    CheckPokedexIntegrity(pokedex);
    pokedex->pokedexObtained = TRUE;
}

PokedexData *SaveData_PokedexData(SaveData *saveData)
{
    PokedexData *pokedex = SaveData_SaveTable(saveData, SAVE_TABLE_ENTRY_POKEDEX);
    return pokedex;
}

u32 PokedexData_GetDisplayForm(const PokedexData *pokedexData, int species, int formIndex)
{
    CheckPokedexIntegrity(pokedexData);

    switch (species) {
    case SPECIES_UNOWN:
        if (formIndex < PokedexData_NumFormsSeen_Unown(pokedexData)) {
            return PokedexData_GetForm_Unown(pokedexData, formIndex);
        }
        break;
    case SPECIES_SHELLOS:
        if (formIndex < PokedexData_NumFormsSeen_Shellos(pokedexData)) {
            return PokedexData_GetForm_Shellos(pokedexData, formIndex);
        }
        break;
    case SPECIES_GASTRODON:
        if (formIndex < PokedexData_NumFormsSeen_Gastrodon(pokedexData)) {
            return PokedexData_GetForm_Gastrodon(pokedexData, formIndex);
        }
        break;
    case SPECIES_BURMY:
        if (formIndex < PokedexData_NumFormsSeen_Burmy(pokedexData)) {
            return PokedexData_GetForm_Burmy(pokedexData, formIndex);
        }
        break;
    case SPECIES_WORMADAM:
        if (formIndex < PokedexData_NumFormsSeen_Wormadam(pokedexData)) {
            return PokedexData_GetForm_Wormadam(pokedexData, formIndex);
        }
        break;
    case SPECIES_DEOXYS:
        if (formIndex < PokedexData_NumFormsSeen_Deoxys(pokedexData)) {
            return PokedexData_GetForm_Deoxys(pokedexData, formIndex);
        }
        break;
    case SPECIES_SHAYMIN:
        if (formIndex < NumFormsSeen_TwoForms(pokedexData, SPECIES_SHAYMIN)) {
            return GetForm_TwoForms(pokedexData, SPECIES_SHAYMIN, formIndex);
        }
        break;
    case SPECIES_GIRATINA:
        if (formIndex < NumFormsSeen_TwoForms(pokedexData, SPECIES_GIRATINA)) {
            return GetForm_TwoForms(pokedexData, SPECIES_GIRATINA, formIndex);
        }
        break;
    case SPECIES_ROTOM:
        if (formIndex < NumFormsSeen_Rotom(pokedexData, SPECIES_ROTOM)) {
            return GetForm_Rotom(pokedexData, SPECIES_ROTOM, formIndex);
        }
        break;
    default:
        break;
    }

    return 0;
}

u32 PokedexData_NumFormsSeen(const PokedexData *pokedex, int species)
{
    CheckPokedexIntegrity(pokedex);

    switch (species) {
    case SPECIES_UNOWN:
        return PokedexData_NumFormsSeen_Unown(pokedex);
    case SPECIES_SHELLOS:
        return PokedexData_NumFormsSeen_Shellos(pokedex);
    case SPECIES_GASTRODON:
        return PokedexData_NumFormsSeen_Gastrodon(pokedex);
    case SPECIES_BURMY:
        return PokedexData_NumFormsSeen_Burmy(pokedex);
    case SPECIES_WORMADAM:
        return PokedexData_NumFormsSeen_Wormadam(pokedex);
    case SPECIES_DEOXYS:
        return PokedexData_NumFormsSeen_Deoxys(pokedex);
    case SPECIES_SHAYMIN:
        return NumFormsSeen_TwoForms(pokedex, SPECIES_SHAYMIN);
    case SPECIES_GIRATINA:
        return NumFormsSeen_TwoForms(pokedex, SPECIES_GIRATINA);
    case SPECIES_ROTOM:
        return NumFormsSeen_Rotom(pokedex, SPECIES_ROTOM);
    default:
        break;
    }

    return 1;
}
