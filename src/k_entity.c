#include "simple_logger.h"

#include "k_entity.h"

typedef struct EntityManager {
	u32 maxEnts;
	Edict* entities;
}EntityManager;

static EntityManager entity_manager;

Edict* ent_new() {
	u32 i;
	for (i = 0; i < entity_manager.maxEnts; i++) {
		if (!entity_manager.entities[i].inUse) {
			entity_manager.entities[i].inUse = 1;
			entity_manager.entities[i].draw_scale.x = 1;
			entity_manager.entities[i].draw_scale.y = 1;
			slog("New entity created");
			return &entity_manager.entities[i];
		}
	}
	return NULL;
}

void ent_draw(Edict* ent) {
	if (!ent->sprite) {
		slog("Entity has no sprite to draw");
		return;
	}
	gf2d_sprite_draw(
		ent->sprite,
		ent->position,
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
}

void ent_think_generic(Edict* ent) {

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