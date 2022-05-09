#ifndef __K_NPC_H__
#define __K_NPC_H__

#include "k_entity.h"
#include "gfc_hashmap.h"

void tamer_think(Edict* self);
void tamer_see(Edict* self, Edict* other);

u64 gTrainerFlags1;
u64 gTrainerFlags2;
u64 gTrainerFlags3;
u64 gTrainerFlags4;

HashMap* gTamerIDs;

#endif