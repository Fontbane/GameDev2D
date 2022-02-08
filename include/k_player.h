#ifndef __K_PLAYER_H__
#define __K_PLAYER_H__

#include "k_entity.h"

Sprite *ben, *fran;

enum {
	PCOS_WALK,
	PCOS_RUN,
	PCOS_BIKE,
	PCOS_SURF,
	PCOS_FISH,
	PCOS_ITEM,
	PCOS_PJ,
	PCOS_MAX
} Costume;

void player_draw(Edict* player);

void player_init();
void player_think(Edict* self);
void player_move(Vector2D movement);


#endif // __K_PLAYER_H__