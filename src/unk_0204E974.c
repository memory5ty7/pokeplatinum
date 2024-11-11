#include "unk_0204E974.h"

#include <nitro.h>
#include <string.h>

#include "struct_decls/struct_020298B0_decl.h"

#include "field/field_system.h"

#include "field_script_context.h"
#include "inlines.h"
#include "savedata.h"
#include "unk_0202854C.h"
#include "../include/constants/battle/condition.h"
#include "../include/constants/pokemon.h"

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

BOOL ScrCmd_089(ScriptContext *ctx)  // Status Party Mon
{
    FieldSystem *fieldSystem = FieldTask_GetFieldSystem(ctx->task);
    u16 v1 = ScriptContext_GetVar(param0); // Status ID
    u16 v2 = ScriptContext_GetVar(param0); // Party Mon Index
    u16 *v3 = ScriptContext_GetVarPointer(param0);
    Party party = fieldSystem->saveData;
    Pokemon* mon = Party_getPokemonByindex(v2);

    BOOL retValue = false;
    if (Pokemon_GetValue(mon, MON_DATA_SPECIES_EXISTS, NULL) == 0) {
        u32 type_1 = Pokemon_GetValue(mon, MON_DATA_TYPE_1,null);
        u32 type_2 = Pokemon_GetValue(mon, MON_DATA_TYPE_1,null);
        u32 ability = Pokemon_GetValue(mon, MON_DATA_ABILITY,null);
            switch(v2){
                case 0:
                    if(type_1!=TYPE_POISON && type_2!=TYPE_POISON && type_1!=TYPE_STEEL && type_2!=TYPE_STEEL && ability!=ABILITY_IMMUNITY){
                        Pokemon_SetValue(mon, MON_DATA_STATUS_CONDITION, MON_CONDITION_POISON);
                        retValue = true;
                    }
                    break; 
                case 1:
                    if(type_1!=TYPE_FIRE && type_2!=TYPE_FIRE && ability!=ABILITY_WATER_VEIL){
                        Pokemon_SetValue(mon, MON_DATA_STATUS_CONDITION, MON_CONDITION_BURN);
                        retValue = true;
                    }
                    break; 
                case 2:
                    if(ability!=ABILITY_VITAL_SPIRIT && ability!=ABILITY_INSOMNIA){
                        Pokemon_SetValue(mon, MON_DATA_STATUS_CONDITION, MON_CONDITION_SLEEP);
                        retValue = true;
                    }
                    break; 
                case 3:
                    if(type_1!=TYPE_ICE && type_2!=TYPE_ICE && ability!=ABILITY_MAGMA_ARMOR){
                        Pokemon_SetValue(mon, MON_DATA_STATUS_CONDITION, MON_CONDITION_FREEZE);
                        retValue = true;
                    }
                    break; 
                case 4:
                    Pokemon_SetValue(mon, MON_DATA_STATUS_CONDITION, MON_CONDITION_PARALYSIS);
                    retValue = true;
                    break; 
                case 5:
                    if(type_1!=TYPE_POISON && type_2!=TYPE_POISON && type_1!=TYPE_STEEL && type_2!=TYPE_STEEL && ability!=ABILITY_IMMUNITY){
                        Pokemon_SetValue(mon, MON_DATA_STATUS_CONDITION, MON_CONDITION_TOXIC);
                        retValue = true;
                    }
                    break;                      
                default:
                    break;
            }
    }
    *v3 = retValue;
    return false;
}

BOOL ScrCmd_08A(ScriptContext *param0)  // Damage Party Mon
{
    FieldSystem *fieldSystem = FieldTask_GetFieldSystem(ctx->task);
    u16 v1 = ScriptContext_GetVar(param0);  // Damage
    u16 v2 = ScriptContext_GetVar(param0);  // Party Mon Index
    u16 *v3 = ScriptContext_GetVarPointer(param0);
    Party party = fieldSystem->saveData;
    Pokemon* mon = Party_getPokemonByindex(v2);

    return 0;
}

BOOL ScrCmd_08B(ScriptContext *param0)
{
    FieldSystem *fieldSystem = param0->fieldSystem;
    SaveData *v1 = fieldSystem->saveData;
    UndergroundData *v2;
    u16 v3 = ScriptContext_GetVar(param0);
    u16 v4 = ScriptContext_GetVar(param0);
    u16 *v5 = ScriptContext_GetVarPointer(param0);

    v2 = sub_020298B0(v1);

    return 0;
}

BOOL ScrCmd_08C(ScriptContext *param0)
{
    FieldSystem *fieldSystem = param0->fieldSystem;
    u16 v1 = ScriptContext_GetVar(param0);
    u16 v2 = ScriptContext_GetVar(param0);
    u16 *v3 = ScriptContext_GetVarPointer(param0);

    return 0;
}

BOOL ScrCmd_08D(ScriptContext *param0)
{
    FieldSystem *fieldSystem = param0->fieldSystem;
    u16 v1 = ScriptContext_GetVar(param0);
    u16 v2 = ScriptContext_GetVar(param0);
    u16 *v3 = ScriptContext_GetVarPointer(param0);

    return 0;
}

BOOL ScrCmd_08E(ScriptContext *param0)
{
    FieldSystem *fieldSystem = param0->fieldSystem;
    u16 v1 = ScriptContext_GetVar(param0);
    u16 v2 = ScriptContext_GetVar(param0);
    u16 *v3 = ScriptContext_GetVarPointer(param0);

    return 0;
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

BOOL ScrCmd_090(ScriptContext *param0)
{
    FieldSystem *fieldSystem = param0->fieldSystem;
    u16 v1 = ScriptContext_GetVar(param0);
    u16 v2 = ScriptContext_GetVar(param0);
    u16 *v3 = ScriptContext_GetVarPointer(param0);

    return 0;
}

BOOL ScrCmd_091(ScriptContext *param0)
{
    FieldSystem *fieldSystem = param0->fieldSystem;
    u16 v1 = ScriptContext_GetVar(param0);
    u16 v2 = ScriptContext_GetVar(param0);
    u16 *v3 = ScriptContext_GetVarPointer(param0);

    return 0;
}

BOOL ScrCmd_092(ScriptContext *param0)
{
    FieldSystem *fieldSystem = param0->fieldSystem;
    u16 v1 = ScriptContext_GetVar(param0);
    u16 v2 = ScriptContext_GetVar(param0);
    u16 *v3 = ScriptContext_GetVarPointer(param0);

    return 0;
}
