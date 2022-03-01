#include "k_player.h"
#include "k_field.h"
#include "k_save.h"
#include "gf2d_draw.h"
#include "simple_logger.h"

void player_init() {
	ben = gf2d_sprite_load_all("images/objects/Ben.png", 32, 32, 4);
	fran = gf2d_sprite_load_all("images/objects/Fran.png", 32, 32, 4);
	player = ent_new();
	player->sprite = ben;//placeholder

	player->position = vector2d(0,0);
	player->targetPosition = vector2d(0,0);
	player->cellPos = cell(0,0);
	player->targetCell = cell(0, 0);
	player->draw_offset = vector2d(-8, 0);
	player->Think = player_think;
	player->inUse = 1;
	player->z = 1;

	slog("Player initialized");
}

void player_think(Edict* self) {
	const Uint8* keys;
	keys = SDL_GetKeyboardState(NULL);
	
	player->frame += 0.1f;
	if (player->frame >= 4.0f) player->frame = 0;

	if (fabs(vector2d_magnitude_between(player->position, player->targetPosition))>=0.01f) {
		player->moveinfo.velocity=directions[player->facing].x* 0.5f+ directions[player->facing].y * 0.5f ;
		switch (player->facing) {
			case DIR_S:
			case DIR_N:
				player->position.y += player->moveinfo.velocity;
				break;
			case DIR_W:
			case DIR_E:
				player->position.x += player->moveinfo.velocity;
		}

	}
}

void player_move(Edict* ent, Point8 movement) {
	cell_add(ent->targetCell, ent->cellPos, movement);
	TileCollision coll = GetCollisionAt(save1.map, ent->targetCell);
	if (coll == COLL_IMPASSIBLE || (coll != ent->z && coll != COLL_TRANSITION && ent->z != COLL_TRANSITION)) {
		ent->targetCell = ent->cellPos;
		//slog("%d, %d", ent->z, coll);
	}
	else {
		ent->targetPosition = cell_to_world(ent->targetCell);
	}
}

void player_draw(Edict* player) {

};