#ifndef __K_EVENT_SCRIPTS_H__
#define __K_EVENT_SCRIPTS_H__

#include "gfc_hashmap.h"
#include "k_entity.h"
#include "k_hud.h"
#include "k_local.h"

HashMap* gEventScripts;

void ES_item_find(Edict* ent, Direction dir);
void ES_signpost_collide(Edict* ent, Direction dir);
void ES_warp_map(Edict* ent, Direction dir);
void ES_warp_local(Edict* ent, Direction dir);
void ES_window_look(Edict* ent, Direction dir);
void ES_mom_talk(Edict* ent, Direction dir);
void ES_thomas_trigger(Edict* ent, Direction dir);
void ES_hugglut_battle(Edict* ent, Direction dir);
void ES_bus_ride(Edict* ent, Direction dir);

void lockall();

void unlockall();

void event_scripts_init();

#endif