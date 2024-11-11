#ifndef POKEPLATINUM_STRUCT_OV22_0225A428_H
#define POKEPLATINUM_STRUCT_OV22_0225A428_H

#include "overlay022/struct_ov22_022597BC.h"
#include "overlay022/struct_ov22_0225A914.h"
#include "overlay022/struct_ov22_0225AB54.h"

#include "bg_window.h"
#include "cell_actor.h"
#include "sprite_resource.h"
#include "strbuf.h"

typedef struct {
    UnkStruct_ov22_022597BC unk_00;
    Window *unk_2C;
    CellActorCollection *unk_30;
    SpriteResourceCollection **unk_34;
    UnkStruct_ov22_0225A914 unk_38;
    UnkStruct_ov22_0225AB54 unk_A0;
    Window *unk_134;
    int unk_138;
    int unk_13C;
    u32 unk_140;
    Strbuf *unk_144;
} UnkStruct_ov22_0225A428;

#endif // POKEPLATINUM_STRUCT_OV22_0225A428_H
