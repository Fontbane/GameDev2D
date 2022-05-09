#include "k_player.h"
#include "k_field.h"
#include "k_save.h"
#include "gf2d_draw.h"
#include "simple_logger.h"
#include "k_local.h"

void player_init() {
	ben = gf2d_sprite_load_all("images/objects/Ben.png", 32, 32, 4);
	fran = gf2d_sprite_load_all("images/objects/Fran.png", 32, 32, 4);
	player = ent_new();
	player->sprite = ben;//placeholder

	player->cellPos = cell(28, 10);
	player->targetCell = cell(28, 11);
	player->position = cell_to_world(player->cellPos);
	player->targetPosition = cell_to_world(player->targetCell);
	player->draw_position = vector2d(game.camera_offset_x, game.camera_offset_y);
	player->draw_offset = vector2d(-8, -16);
	player->Think = player_think;
	player->moving = 1;
	player->velocity = 1;
	player->z = COLL_3;

	slog("Player initialized");
}

void player_think(Edict* ent) {

	if (ent->moving == 17) {
		ent->moving = 0;
		save1.steps++;
	}
	if (0 < ent->moving && ent->moving < 17)
	{
		player->frame += player->velocity / 10;
		if (player->frame >= 4.0f) player->frame = 0;
		if (!cell_cmp(ent->targetCell, ent->cellPos) && (ent->velocity >= 1 || ent->moving % 2 == 1)) {
			switch (ent->facing) {
			case DIR_S:
				ent->position.y += ent->velocity;
				break;
			case DIR_N:
				ent->position.y -= ent->velocity;
				break;
			case DIR_W:
				ent->position.x -= ent->velocity;
				break;
			case DIR_E:
				ent->position.x += ent->velocity;
			}
			ent->moving++;
		}
		else {
			ent->moving = 0;
		}
	}
}

void player_move(Edict* ent, Point8 movement) {
	if (!player->moving) {
		cell_add(ent->targetCell, ent->cellPos, movement);
		TileCollision coll = GetCollisionAt(save1.map, ent->targetCell);
		if (coll == COLL_IMPASSIBLE || (coll != ent->z && coll != COLL_TRANSITION && ent->z != COLL_TRANSITION) ||
			CheckEntCollision(ent->targetCell, ent->facing)) {
			ent->targetCell = ent->cellPos;
			slog("%d, %d", ent->z, coll);
		}
		else {
			if (coll == COLL_TRANSITION) ent->z = COLL_TRANSITION;
			if (ent->z == COLL_TRANSITION) ent->z = coll;
			ent->targetPosition = cell_to_world(ent->targetCell);
			ent->moving = 1;
		}
	}
}

void player_draw(Edict* player) {

};