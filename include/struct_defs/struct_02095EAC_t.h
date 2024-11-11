#ifndef POKEPLATINUM_STRUCT_02095EAC_T_H
#define POKEPLATINUM_STRUCT_02095EAC_T_H

#include "struct_decls/struct_02015920_decl.h"
#include "struct_defs/struct_0200C738.h"
#include "struct_defs/struct_0203DDFC.h"
#include "struct_defs/struct_02095EAC_sub1.h"

#include "overlay058/struct_ov58_021D2754.h"
#include "overlay058/struct_ov58_021D2820.h"

#include "bg_window.h"
#include "cell_actor.h"
#include "message.h"
#include "sprite_resource.h"
#include "strbuf.h"
#include "string_template.h"
#include "trainer_info.h"

struct UnkStruct_02095EAC_t {
    BgConfig *unk_00;
    BOOL unk_04;
    UnkStruct_0203DDFC *unk_08;
    StringTemplate *unk_0C;
    MessageLoader *unk_10;
    Strbuf *unk_14[5];
    Strbuf *unk_28;
    Strbuf *unk_2C;
    int unk_30;
    CellActorCollection *unk_34;
    UnkStruct_0200C738 unk_38;
    SpriteResourceCollection *unk_1C4[4];
    SpriteResource *unk_1D4[2][4];
    CellActorResourceData unk_1F4;
    CellActorResourceData unk_218;
    CellActor *unk_23C[14];
    CellActor *unk_274[14];
    CellActor *unk_2AC[12];
    Window unk_2DC[5];
    Window unk_32C;
    Window unk_33C;
    Window unk_34C;
    Window *unk_35C[2];
    int unk_364;
    int unk_368;
    int unk_36C;
    int unk_370;
    int unk_374;
    int unk_378;
    int unk_37C;
    int unk_380;
    int unk_384;
    u8 unk_388[8][2];
    TrainerInfo *unk_398[8][2];
    u8 unk_3D8[16384];
    u16 unk_43D8;
    u8 unk_43DA;
    u8 unk_43DB;
    UnkStruct_ov58_021D2820 unk_43DC;
    UnkStruct_ov58_021D2820 unk_43E6[5];
    UnkStruct_ov58_021D2754 unk_4418[5];
    u8 *unk_442C;
    int unk_4430;
    u8 unk_4434[14400];
    UnkStruct_02095EAC_sub1 unk_7C74;
    UnkStruct_02095EAC_sub1 unk_8064[5];
    s32 unk_9414;
    u32 unk_9418;
    u16 unk_941C;
    s16 unk_941E;
    u8 unk_9420;
    UnkStruct_ov58_021D2820 unk_9421[5];
    UnkStruct_02015920 *unk_9454;
    int unk_9458;
    int unk_945C;
    int unk_9460;
    int unk_9464;
    int unk_9468;
    int unk_946C;
};

#endif // POKEPLATINUM_STRUCT_02095EAC_T_H
