#include "simple_logger.h"
#include "gf2d_graphics.h"

#include "k_monster.h"
#include "k_local.h"
#include "k_item.h"
#include "k_entity.h"
#include "k_field.h"
#include "k_save.h"
#include "k_event_scripts.h"
#include "k_text.h"
#include "k_npc.h"

typedef struct EntityManager {
	u32 maxEnts;
	Edict* entities;
	Warp* warps;
}EntityManager;

static EntityManager entity_manager;

Edict* ent_new() {
	u32 i;
	for (i = 0; i < entity_manager.maxEnts; i++) {
		if (!entity_manager.entities[i].inUse) {
			entity_manager.entities[i].inUse = 1;
			entity_manager.entities[i].draw_scale.x = 1;
			entity_manager.entities[i].draw_scale.y = 1;
			entity_manager.entities[i].draw_offset = vector2d(-8, 0);
			entity_manager.entities[i].Think = ent_think_generic;
			entity_manager.entities[i].OnCollide = ent_collide_generic;
			entity_manager.entities[i].localID = i;
			entity_manager.entities[i].moveq = newq();
			if (!entity_manager.entities[i].moveq) slog("Could not create new movement queue");
			slog("New entity created");
			return &entity_manager.entities[i];
		}
	}
	return NULL;
}

void ent_draw(Edict* ent) {
	if (!ent->sprite) {
		//slog("Entity has no sprite to draw");
		return;
	}
	ent->draw_position.x = game.camera_offset_x - player->position.x + ent->position.x + ent->draw_offset.x;
	ent->draw_position.y = game.camera_offset_y - player->position.y + ent->position.y + ent->draw_offset.y;
	gf2d_sprite_draw(
		ent->sprite,
		ent->draw_position,
		&ent->draw_scale,
		NULL,
		NULL,
		NULL,
		NULL,
		ent->facing*4+(int)ent->frame
	);
}

void ent_free(Edict* ent) {
	if (ent == NULL) {
		slog("Null ptr provided, nothing to do");
		return;
	}
	if (ent->sprite != NULL) {
		gf2d_sprite_free(ent->sprite);
	}
	memset(ent, 0, sizeof(ent));
}

void ent_manager_close(void) {
	ent_manager_clear();
	if (entity_manager.entities != NULL)
	{
		free(entity_manager.entities);
	}
	slog("Entity manager closed");
}

void ent_manager_init(u32 maxEnts) {
	if (maxEnts == 0) {
		slog("0 entity error");
		return;
	}
	if (entity_manager.entities != NULL) {
		slog("Entity manager already initialized");
		return;
	}
	entity_manager.maxEnts = maxEnts;
	entity_manager.entities = gfc_allocate_array(sizeof(Edict), maxEnts);
	atexit(ent_manager_close);
	slog("Entity manager initialized");
}

void ent_manager_clear() {
	u32 i;
	for (i = 0; i < entity_manager.maxEnts; i++) {
		ent_free(&entity_manager.entities[i]);
	}
}

void ent_think(Edict* ent) {
	if (ent->inUse)
		ent->Think(ent);
	ent->targetCell = world_to_cell(ent->targetPosition);
	ent->cellPos = world_to_cell(ent->position);
	ent_move(ent);
}

void ent_think_generic(Edict* ent) {

}

void ent_collide_generic(Edict* ent, Direction dir) {
	switch (dir) {
	case DIR_N:
		ent->facing = DIR_S;
		break;
	case DIR_S:
		ent->facing = DIR_N;
		break;
	case DIR_E:
		ent->facing = DIR_W;
		break;
	case DIR_W:
		ent->facing = DIR_E;
	}

	ent->OnTalk(ent, dir);
}

void ent_manager_think_all() {
	u32 i;
	for (i = 0; i < entity_manager.maxEnts; i++) {
		if (entity_manager.entities[i].inUse) ent_think(&entity_manager.entities[i]);
	}
}

void ent_manager_draw_all() {
	u32 i;
	for (i = 0; i < entity_manager.maxEnts; i++) {
		if (entity_manager.entities[i].inUse) ent_draw(&entity_manager.entities[i]);
	}
}


void setmovement(Edict* ent, Point8 mvmt) {
	if (!empty(ent->moveq)) {
		qclear(ent->moveq);
	}
	if (mvmt.x != 0 && mvmt.y != 0) {
		enque(ent->moveq, (Point8) { 0, mvmt.y });
		enque(ent->moveq, (Point8) { mvmt.x, 0 });
	}
	else {
		enque(ent->moveq, mvmt);
	}
}

void ent_move(Edict* ent) {
	if (ent->moving == 17) ent->moving = 0;
	if (0<ent->moving&&ent->moving<17)
	{
		if (!cell_cmp(ent->targetCell, ent->cellPos) && (ent->velocity >= 1 || ent->moving % 2 == 1)) {
			switch (ent->facing) {
			case DIR_S:
				ent->position.y += ent->velocity;
				break;
			case DIR_N:
				ent->position.y -= ent->velocity;
				break;
			case DIR_W:
				ent->position.x += ent->velocity;
				break;
			case DIR_E:
				default:
				ent->position.x -= ent->velocity;
			}
			ent->moving++;
		}
		return;
	};

	Point8 movement;
	if (empty(ent->moveq))
		return;

	movement = pop(ent->moveq);
	ent->moving = 1;
	
	cell_add(ent->targetCell, ent->cellPos, movement);
	TileCollision coll = GetCollisionAt(save1.map, ent->targetCell);
	if (coll==COLL_IMPASSIBLE||(coll!=ent->z&&coll!=COLL_TRANSITION&&ent->z!=COLL_TRANSITION)) {
		ent->targetCell = ent->cellPos;
		return;
	}
	else {
		ent->targetPosition = cell_to_world(ent->targetCell);
	}
}

void coll_mukchuk(Edict* ent, Direction dir)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"","You found a Mukchuk!",NULL);
	party.partyPersonal[4] = *monster_new(SPECIES_MUKCHUK, 10, 0, Random32());
	party.party[4] = *monster_set_dict(party.partyPersonal + 4);
	ent_free(ent);
}
void coll_tree(Edict* ent, Direction dir) {
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "", "You found a Sappurr!", NULL);
	party.partyPersonal[1] = *monster_new(SPECIES_SAPPURR, 5, 0, Random32());
	party.party[1] = *monster_set_dict(party.partyPersonal + 1);
	ent_free(ent);
}

void talk_showme_egglet(Edict* ent, Direction dir) {
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Kid", "Can you show me an Egglet?", NULL);
	if (party.partyPersonal[2].species == SPECIES_EGGLET) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Kid", "Oh, that's it! Here, take this!", NULL);
		GiveItem("Restorade");
	}
}
void talk_treehugger(Edict* ent, Direction dir) {
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Tree Guy", "Hey man, go touch that tree.", NULL);
}
void talk_find_mukchuk(Edict* ent, Direction dir) {
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Farmer", "There's a monster scurrying around underground. Try and find it for me.", NULL);
}
void talk_riddler(Edict* ent, Direction dir) {
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Other Kid", "Hey, what type is Egglet?",NULL);
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Other Kid", "That's right, Air!", NULL);
	party.partyPersonal[2] = *monster_new(SPECIES_EGGLET, 5, 0, Random32());
	party.party[2] = *monster_set_dict(party.partyPersonal + 2);
}
void talk_give_restorade(Edict* ent, Direction dir) {
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Fran", "Hey, my Squoink's low on health. Can you give me a Restorade?", NULL);
	if (RemoveItem(ITEM_RESTORADE)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Fran", "Oh, thanks man, I owe you one! Here, have a Pyruff.", NULL);
		party.partyPersonal[3] = *monster_new(SPECIES_PYRUFF, 5, 0, Random32());
		party.party[3] = *monster_set_dict(party.partyPersonal + 3);
	}
}


int Colliding(Rect a, Rect b) {
	return SDL_HasIntersection(&a, &b);
}

u16 CheckEntCollision(Point8 pt, Direction dir) {
	unsigned int i;
	for (i = 0; i < entity_manager.maxEnts; i++) {
		if (entity_manager.entities[i].inUse && entity_manager.entities[i].cellPos.x == pt.x && entity_manager.entities[i].cellPos.y == pt.y) {
			entity_manager.entities[i].OnCollide(entity_manager.entities+i,dir);
			return i;
		}
	}
	return SDL_FALSE;
}

void WarpPlayer(Edict* ent, Direction dir) {
	int i;
	lockall();
	for (i = 0; entity_manager.entities[i].eventType != EV_TRIGGER; i++) {
		if (entity_manager.entities[i].eventType == EV_WARP && entity_manager.entities[i].info.w.id == ent->info.w.connection) {
			player->cellPos = entity_manager.entities[i].cellPos;
			player->position = entity_manager.entities[i].position;
			player->targetCell = player->cellPos;
			player->targetPosition = player->position;
			player->facing = dir;
			unlockall();
			slog("Warping");
			return;
		}
	}
}

Point8 cell(s8 x, s8 y) {
	Point8 c= { x, y };
	return c;
}

Point8 world_to_cell(Vector2D pos) {
	Point8 c = { (int)pos.x / 16, (int)pos.y / 16 };
	return c;
}

Vector2D cell_to_world(Point8 pos) {
	Vector2D v = { pos.x * 16.0f, pos.y * 16.0f };
	return v;
}

void LoadEntitiesFromJson(SJson* json) {
	SJson* arr, * el;
	Edict* e;
	char* buff;
	int i, c, incr;
	short trash;
	incr = 0;
	slog("Loading entities");
	arr = sj_object_get_value(json, "signposts");
	if (arr) {
		slog("Loading signposts");
		for (c = sj_array_get_count(arr), i = 0; i < c; i++) {
			el = sj_array_get_nth(arr, i);
			e = ent_new();
			if (!e) slog("Something went wrong");
			e->collidable = 1;
			e->eventType = EV_SIGNPOST;
			sj_get_integer_value(sj_object_get_value(el, "x"), &e->cellPos.x);
			sj_get_integer_value(sj_object_get_value(el, "y"), &e->cellPos.y);
			buff = sj_get_string_value(sj_object_get_value(el, "text"));
			if (buff) {
				gfc_line_cpy(e->info.s.text, "please for the love of god work for me");
			}
			e->OnCollide = ES_signpost_collide;
			e->position.x = e->cellPos.x * 16.0;
			e->position.y = e->cellPos.y * 16.0;
			incr++;
			slog("fdsa");
		}
	}
	arr = sj_object_get_value(json, "npcs");
	if (arr) {
		slog("Loading npcs");
		for (c = sj_array_get_count(arr), i = 0; i < c; i++) {
			el = sj_array_get_nth(arr, i);
			e = ent_new();
			e->inUse = 1;
			e->collidable = 1;
			e->eventType = EV_NPC;
			sj_get_integer_value(sj_object_get_value(el, "x"), &e->cellPos.x);
			sj_get_integer_value(sj_object_get_value(el, "y"), &e->cellPos.y);
			e->position.x = e->cellPos.x * 16.0;
			e->position.y = e->cellPos.y * 16.0;
			if (buff = sj_get_string_value(sj_object_get_value(el, "type"))) {
				if (strcmp(buff, "Tamer") == 0) {
					e->eventType = EV_TAMER;
					if (buff = sj_get_string_value(sj_object_get_value(el, "tamer_id"))) {
						sj_get_integer_value(sj_object_get_value(el, "flag"), &e->info.n.globalFlagID);
						sj_get_integer_value(sj_object_get_value(el, "sight"), &e->info.n.sight);
						e->info.n.tamerID = *(u16*)gfc_hashmap_get(gTamerIDs, buff);
					}
					e->Think = tamer_think;
				}
				else if (strcmp(buff, "NPC") == 0) {
					e->Think = gfc_hashmap_get(gEventScripts, sj_get_string_value(sj_object_get_value(el, "think")));
				}
				e->OnCollide = gfc_hashmap_get(gEventScripts, sj_get_string_value(sj_object_get_value(el, "oncollide")));
				e->OnTalk = gfc_hashmap_get(gEventScripts, sj_get_string_value(sj_object_get_value(el, "ontalk")));
			}
			buff = sj_get_string_value(sj_object_get_value(el, "facing"));
			if (strcmp(buff, "north") == 0) e->facing = DIR_N;
			else if (strcmp(buff, "west") == 0) e->facing = DIR_W;
			else if (strcmp(buff, "east") == 0) e->facing = DIR_E;
			else e->facing = DIR_S;
			buff = sj_get_string_value(sj_object_get_value(el, "sprite"));
			e->sprite = gf2d_sprite_load_all(buff, 32, 32, 4);
			e->frame = 0.0f;
			incr++;
		}
	}
	arr = sj_object_get_value(json, "warps");
	if (arr) {
		slog("Loading warps");
		for (c = sj_array_get_count(arr), i = 0; i < c; i++) {
			el = sj_array_get_nth(arr, i);
			e = ent_new();
			e->inUse = 1;
			e->collidable = 1;
			e->eventType = EV_WARP;
			sj_get_integer_value(sj_object_get_value(el, "x"), &e->cellPos.x);
			sj_get_integer_value(sj_object_get_value(el, "y"), &e->cellPos.y);
			e->position.x = e->cellPos.x * 16.0;
			e->position.y = e->cellPos.y * 16.0;
			if (sj_object_get_value(el, "fade")) e->info.w.fadeToBlack = 1;
			if (sj_object_get_value(el, "spin")) e->info.w.spin = 1;
			if (buff = sj_get_string_value(sj_object_get_value(el, "mapID"))) {
				e->info.w.mapID = buff;
				e->OnCollide = ES_warp_map;
			}
			else {
				e->OnCollide = WarpPlayer;
			}
			sj_get_integer_value(sj_object_get_value(el, "id"), &e->info.w.id);
			sj_get_integer_value(sj_object_get_value(el, "dst"), &e->info.w.connection);
			incr++;
		}
	}
	arr = sj_object_get_value(json, "triggers");
	if (arr) {
		slog("Loading triggers");
		for (c = sj_array_get_count(arr), i = 0; i < c; i++) {
			el = sj_array_get_nth(arr, i);
			e = ent_new();
			e->inUse = 1;
			e->eventType = EV_TRIGGER;
			sj_get_integer_value(sj_object_get_value(el, "x"), &e->cellPos.x);
			sj_get_integer_value(sj_object_get_value(el, "y"), &e->cellPos.y);
			e->position.x = e->cellPos.x * 16.0;
			e->position.y = e->cellPos.y * 16.0;
			if (buff = sj_get_string_value(sj_object_get_value(el, "oncollide"))) {
				e->OnCollide = gfc_hashmap_get(gEventScripts, buff);
			}
			else {
				slog("Trigger event object has no associated function");
			}
			incr++;
		}
	}
	arr = sj_object_get_value(json, "items");
	if (arr) {
		slog("Parsing item events");
		for (c = sj_array_get_count(arr), i = 0; i < c; i++) {
			el = sj_array_get_nth(arr, i);
			e = ent_new();
			sj_get_integer_value(sj_object_get_value(el, "globalID"), &e->info.n.globalFlagID);
			if (save1.quests & e->info.n.globalFlagID) {
				continue;//skip this entity if it's been picked up already
			}
			e->inUse = 1;
			e->eventType = EV_ITEM;
			sj_get_integer_value(sj_object_get_value(el, "x"), &e->cellPos.x);
			sj_get_integer_value(sj_object_get_value(el, "y"), &e->cellPos.y);
			e->position.x = e->cellPos.x * 16.0;
			e->position.y = e->cellPos.y * 16.0;
			if (sj_get_bool_value(sj_object_get_value(el, "hidden"), &trash)) {
				e->info.n.sight = 1;//hidden items are invisible and not collidable
			}
			else {
				e->sprite = gf2d_sprite_load_image("images/objects/item.png");
				e->collidable = 1;
			}
			e->OnTalk = ES_item_find;
			buff = sj_get_string_value(sj_object_get_value(el, "itemID"));
			e->info.n.tamerID = ItemIDFromJson(buff);
			incr++;
		}
	}
	slog("All entities loaded");
}