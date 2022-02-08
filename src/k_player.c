#include "k_player.h"
#include "k_field.h"
#include "gf2d_draw.h"
#include "simple_logger.h"

void player_init() {
	ben = gf2d_sprite_load_all("images/objects/Ben.png", 32, 32, 4);
	fran = gf2d_sprite_load_all("images/objects/Fran.png", 32, 32, 4);
	player = ent_new();
	player->sprite = ben;

	player->position = vector2d(0,0);
	player->targetPosition = vector2d(0,0);
	player->draw_offset = vector2d(-8, 0);
	player->Think = player_think;
	player->inUse = 1;

	slog("Player initialized");
}

void player_think(Edict* self) {
	const Uint8* keys;
	keys = SDL_GetKeyboardState(NULL);
	
	player->frame += 0.1f;
	if (player->frame >= 4.0f) player->frame = 0;

	if (fabs(vector2d_magnitude_between(player->position, player->targetPosition))>=0.01f) {
		//slog("Moving");
		vector2d_scale(player->velocity, directions[player->facing], 0.5f);
		vector2d_add(player->position, player->position, player->velocity);
	}
}

void player_move(Vector2D movement) {
	Vector2D target;
	vector2d_add(target, player->position, movement);
	if (/*GetCollisionAt(gmap, target) == player->z*/0==0) {
		player->targetPosition = target;
	}
}

void player_draw(Edict* player) {

};