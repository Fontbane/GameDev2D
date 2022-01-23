#ifndef __LOCAL_H__
#define __LOCAL_H__

#include "entity.h"
#include "field.h"

typedef struct GameLocal {
	Map		map;
	Edict*	entities;
	Edict*	camera;
	Edict*	player;
	time_t	time;
	u32		steps;

	u64		randomSeed;
	u64		random;

} GameLocal;

u32 Random(int min, int max);

GameLocal game;
Map level;

#endif