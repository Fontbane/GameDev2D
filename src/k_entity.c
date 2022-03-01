#include "simple_logger.h"

#include "k_entity.h"
#include "k_field.h"
#include "k_save.h"

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
			entity_manager.entities[i].draw_offset = vector2d(-8, 0);
			entity_manager.entities[i].Think = ent_think_generic;
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
	vector2d_add(ent->draw_position, ent->position, ent->draw_offset);
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

void ent_move(Edict* ent) {
	if (ent->moving)
	{
		if (!cell_cmp(ent->targetCell, ent->cellPos)) {
			switch (ent->facing) {
			case DIR_S:
			case DIR_N:
				ent->position.y += ent->moveinfo.velocity;
				break;
			case DIR_W:
			case DIR_E:
				ent->position.x += ent->moveinfo.velocity;
			}
		}
		else {
			ent->moving = 0;
		}
		return;
	};

	Point8 movement;
	if (empty(&ent->moveq))
		return;

	movement = pop(&ent->moveq);
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

int Colliding(Rect a, Rect b) {
	return SDL_HasIntersection(&a, &b);
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