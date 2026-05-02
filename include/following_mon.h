#ifndef POKEPLATINUM_FOLLOW_MON_H
#define POKEPLATINUM_FOLLOW_MON_H

#include "pokemon.h"
#include "field/field_system_decl.h"
#include "struct_decls/struct_02061830_decl.h"
#include "struct_decls/struct_02061AB4_decl.h"

#define FOLLOW_MON_PARAM_SPECIES 0

typedef struct FollowMon {
    MapObject *mapObject;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u32 species;
    u8 gender;
    u8 unk15;
    u8 active;
    u8 shiny;
    u16 form;
    u16 dummy;
    u32 unk1C;
} FollowMon;

typedef struct FieldSystemUnk108 {
    u32 personality;
    u16 species;
    u16 isRegistered;
    Pokemon *mon;
} FieldSystemUnk108;

MapObject *FollowMon_InitMapObject(MapObjectManager *mapObjectManager, int x, int y, int direction, u32 mapNo);
void FollowMon_ChangeMon(MapObjectManager *mapObjectManager, u32 mapNo);
MapObject *FollowMon_GetMapObject(FieldSystem *fieldSystem);
u32 FollowMon_GetSpriteID(int species, u16 form, u32 gender);
void sub_02069DC8(MapObject *mapObject, BOOL enableBit);
void sub_02069DEC(MapObject *mapObject, BOOL enableBit);
u8 sub_02069E14(MapObject *mapObject);
void sub_02069E28(MapObject *mapObject, u32 a1);
void sub_02069E50(MapObject *mapObject, u8 a1);
void sub_02069E84(MapObject *mapObject, u8 a1);
u8 sub_02069EAC(MapObject *mapObject);
u8 sub_02069EC0(MapObject *mapObject);
u8 sub_02069ED4(MapObject *mapObject);
void FollowMon_SetObjectParams(MapObject *mapObject, int species, u8 form, BOOL shiny);
void sub_02069F0C(MapObject *mapObject, int species, u8 form, BOOL shiny, int a4);
void FieldSystem_SetFollowerPokeParam(FieldSystem *fieldSystem, int species, u8 form, BOOL shiny, u8 gender);
u8 FollowMon_GetSizeParamBySpecies(int species);
int FollowMon_GetSpecies(MapObject *mapObject);
BOOL FollowMon_IsActive(FieldSystem *fieldSystem);
BOOL FollowMon_IsVisible(FieldSystem *fieldSystem);
BOOL FollowMon_GetPermission(FieldSystem *fieldSystem);
BOOL FollowMon_GetPermissionBySpeciesAndMap(int species, u32 mapNo);
void sub_0206A040(MapObject *mapObject, BOOL enableBit);
void sub_0206A054(FieldSystem *fieldSystem);
FieldSystemUnk108 *FieldSystem_UnkSub108_Alloc(enum HeapID heapID);
void FieldSystem_UnkSub108_AddMonMood(FieldSystemUnk108 *unk, s8 by);
void FieldSystem_UnkSub108_SetMonMood(FieldSystemUnk108 *unk, s8 mood);
s8 FieldSystem_UnkSub108_GetMonMood(FieldSystemUnk108 *unk);
void FieldSystem_UnkSub108_Set(FieldSystemUnk108 *a0, Pokemon *mon, u16 species, u32 personality);
void FieldSystem_UnkSub108_MoveMoodTowardsNeutral(FieldSystemUnk108 *a0);
int SpeciesToOverworldModelIndexOffset(int species);
int OverworldModelLookupFormCount(int species);
BOOL OverworldModelLookupHasFemaleForm(int species);

#endif // POKEPLATINUM_FOLLOW_MON_H