#include "k_event_scripts.h"
#include "k_hud.h"
#include "k_item.h"
#include "simple_logger.h"

void ES_item_find(Edict* ent, Direction dir) {
	GiveItem(ent->info.n.tamerID);
}
void ES_signpost_collide(Edict* ent, Direction dir) {
	msgbox(ent->info.s.text);
}
void ES_warp_map(Edict* ent, Direction dir) {

}
void ES_warp_local(Edict* ent, Direction dir) {
	int i;
	lockall();
	for (i = 0; level.entities[i].eventType != EV_TRIGGER; i++) {
		if (level.entities[i].eventType == EV_WARP && level.entities[i].info.w.id == ent->info.w.connection) {
			player->cellPos = level.entities[i].cellPos;
			player->position = level.entities[i].position;
			player->targetCell = player->cellPos;
			player->targetPosition = player->position;
			player->facing = dir;
			unlockall();
			slog("Warping");
			return;
		}
	}
}

void ES_window_look(Edict* ent, Direction dir) {
	msgbox("msgbox_window_looking_good");
}

void ES_mom_talk(Edict* ent, Direction dir) {
	ent->facing = dir;
	msgbox("msgbox_mom_bye");
}

void lockall() {
	game.lockall = 1;
}

void unlockall() {
	game.lockall = 0;
}

void event_scripts_init() {
	gEventScripts = gfc_hashmap_new();

	gfc_hashmap_insert(gEventScripts, "window_look", ES_window_look);
	gfc_hashmap_insert(gEventScripts, "mom_talk", ES_mom_talk);

}