#include "unk_0204E974.h"

#include <nitro.h>
#include <string.h>

#include "struct_decls/struct_020298B0_decl.h"

#include "field/field_system.h"

#include "pokemon.h"
#include "field_script_context.h"
#include "inlines.h"
#include "savedata.h"
#include "special_encounter.h"
#include "unk_0202854C.h"
#include "vars_flags.h"

BOOL ScrCmd_083(ScriptContext *param0)
{
    FieldSystem *fieldSystem = param0->fieldSystem;
    SaveData *v1 = fieldSystem->saveData;
    UndergroundData *v2;
    u16 v3 = ScriptContext_GetVar(param0);
    u16 v4 = ScriptContext_GetVar(param0);
    u16 *v5 = ScriptContext_GetVarPointer(param0);

    v2 = sub_020298B0(v1);
    *v5 = sub_0202895C(v2, v3);

    return 0;
}

BOOL ScrCmd_084(ScriptContext *param0)
{
    FieldSystem *fieldSystem = param0->fieldSystem;
    u16 v1 = ScriptContext_GetVar(param0);
    u16 v2 = ScriptContext_GetVar(param0);
    u16 *v3 = ScriptContext_GetVarPointer(param0);

    return 0;
}

BOOL ScrCmd_085(ScriptContext *param0)
{
    FieldSystem *fieldSystem = param0->fieldSystem;
    SaveData *v1 = fieldSystem->saveData;
    UndergroundData *v2;
    u16 v3 = ScriptContext_GetVar(param0);
    u16 v4 = ScriptContext_GetVar(param0);
    u16 *v5 = ScriptContext_GetVarPointer(param0);

    v2 = sub_020298B0(v1);
    *v5 = sub_02028984(v2, v3);

    return 0;
}

BOOL ScrCmd_086(ScriptContext *param0)
{
    FieldSystem *fieldSystem = param0->fieldSystem;
    u16 v1 = ScriptContext_GetVar(param0);
    u16 v2 = ScriptContext_GetVar(param0);
    u16 *v3 = ScriptContext_GetVarPointer(param0);

    return 0;
}

BOOL ScrCmd_087(ScriptContext *param0)
{
    FieldSystem *fieldSystem = param0->fieldSystem;
    SaveData *v1 = fieldSystem->saveData;
    UndergroundData *v2;
    u16 v3 = ScriptContext_GetVar(param0);
    u16 v4 = ScriptContext_GetVar(param0);
    u16 *v5 = ScriptContext_GetVarPointer(param0);

    v2 = sub_020298B0(v1);
    *v5 = sub_02028E84(v2, v3);

    return 0;
}

BOOL ScrCmd_088(ScriptContext *param0)
{
    FieldSystem *fieldSystem = param0->fieldSystem;
    u16 v1 = ScriptContext_GetVar(param0);
    u16 v2 = ScriptContext_GetVar(param0);
    u16 *v3 = ScriptContext_GetVarPointer(param0);

    return 0;
}

static const u32 sStatus[] = {
    MON_CONDITION_NONE,
    MON_CONDITION_BURN,
    MON_CONDITION_FREEZE,
    MON_CONDITION_PARALYSIS,
    MON_CONDITION_POISON
};

BOOL ScrCmd_089(ScriptContext *ctx)  // Set status of selected mon
{
    FieldSystem *fieldSystem = ctx->fieldSystem;
    Pokemon *pokemon;
    u16 partySlot = ScriptContext_GetVar(ctx);
    u16 statusID = ScriptContext_GetVar(ctx);

    pokemon = Party_GetPokemonBySlotIndex(Party_GetFromSavedata(fieldSystem->saveData), partySlot);

    u32 status = sStatus[statusID];

    if(Pokemon_CanApplyStatus(pokemon, status)) {
        Pokemon_SetValue(pokemon, MON_DATA_STATUS_CONDITION, &status);
    }

    return FALSE;
}

BOOL ScrCmd_08A(ScriptContext *ctx)  // Set HP of selected mon
{
    FieldSystem *fieldSystem = ctx->fieldSystem;
    Pokemon *pokemon;
    u16 partySlot = ScriptContext_GetVar(ctx);
    u16 HP = ScriptContext_GetVar(ctx);

    pokemon = Party_GetPokemonBySlotIndex(Party_GetFromSavedata(fieldSystem->saveData), partySlot);
    u16 maxHP = Pokemon_GetValue(pokemon, MON_DATA_MAX_HP, NULL);

    if (HP <= maxHP) {
        Pokemon_SetValue(pokemon, MON_DATA_CURRENT_HP, &HP);
    } else {
        Pokemon_SetValue(pokemon, MON_DATA_CURRENT_HP, &maxHP);
    }

    return FALSE;
}

BOOL ScrCmd_08B(ScriptContext *ctx)  // Max IVs of selected mon in selected stat
{
    FieldSystem *fieldSystem = ctx->fieldSystem;
    Pokemon *pokemon;
    u16 partySlot = ScriptContext_GetVar(ctx);
    u16 stat = ScriptContext_GetVar(ctx);

    pokemon = Party_GetPokemonBySlotIndex(Party_GetFromSavedata(fieldSystem->saveData), partySlot);
    int ivs = 31;

    Pokemon_SetValue(pokemon, MON_DATA_HP_IV + stat, &ivs);
    Pokemon_CalcStats(pokemon);

    return FALSE;
}

BOOL ScrCmd_08C(ScriptContext *ctx)  // Change nature of selected mon to specific nature
{
    FieldSystem *fieldSystem = ctx->fieldSystem;
    Pokemon *pokemon;
    u16 partySlot = ScriptContext_GetVar(ctx);
    u16 nature = ScriptContext_GetVar(ctx);

    pokemon = Party_GetPokemonBySlotIndex(Party_GetFromSavedata(fieldSystem->saveData), partySlot);

    return FALSE;
}

BOOL ScrCmd_08D(ScriptContext *ctx)  // Min IVs of selected mon in selected stat
{
    FieldSystem *fieldSystem = ctx->fieldSystem;
    Pokemon *pokemon;
    u16 partySlot = ScriptContext_GetVar(ctx);
    u16 stat = ScriptContext_GetVar(ctx);

    pokemon = Party_GetPokemonBySlotIndex(Party_GetFromSavedata(fieldSystem->saveData), partySlot);
    int ivs = 0;

    Pokemon_SetValue(pokemon, MON_DATA_HP_IV + stat, &ivs);
    Pokemon_CalcStats(pokemon);

    return FALSE;
}

BOOL ScrCmd_08E(ScriptContext *ctx) // Change field weather
{
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u16 weather = ScriptContext_GetVar(ctx);

    FieldOverworldState_SetWeather(SaveData_GetFieldOverworldState(fieldSystem->saveData), weather);
    
    return FALSE;
}

BOOL ScrCmd_08F(ScriptContext *param0)
{
    FieldSystem *fieldSystem = param0->fieldSystem;
    SaveData *v1 = fieldSystem->saveData;
    UndergroundData *v2;
    u16 v3 = ScriptContext_GetVar(param0);
    u16 v4 = ScriptContext_GetVar(param0);
    u16 *v5 = ScriptContext_GetVarPointer(param0);

    v2 = sub_020298B0(v1);
    *v5 = sub_02028CB0(v2, v3, v4);
    return 0;
}

BOOL ScrCmd_090(ScriptContext *ctx)  // Update Repellent
{
    FieldSystem *fieldSystem = ctx->fieldSystem;
    SaveData *saveData = fieldSystem->saveData;

    u8 *repelSteps = SpecialEncounter_GetRepelSteps(SaveData_GetSpecialEncounters(saveData));

    *repelSteps = 1 - *repelSteps;

    return FALSE;
}

BOOL ScrCmd_091(ScriptContext *ctx)  // Get Repellent State
{
    FieldSystem *fieldSystem = ctx->fieldSystem;
    SaveData *saveData = fieldSystem->saveData;
    u16 *returnVar = ScriptContext_GetVarPointer(ctx);

    *returnVar = (u16)(*SpecialEncounter_GetRepelSteps(SaveData_GetSpecialEncounters(saveData)));

    return FALSE;
}

BOOL ScrCmd_092(ScriptContext *ctx) // Change battle background
{
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u16 background = ScriptContext_GetVar(ctx);

    u32 headerID = fieldSystem->location->mapId;

    setVar(16430, background);

    return FALSE;
}
