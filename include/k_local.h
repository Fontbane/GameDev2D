#ifndef __LOCAL_H__
#define __LOCAL_H__

#include "k_field.h"

typedef struct GameLocal {
	u64		framenum;
	time_t	time;
	char	*map;
	Edict*	entities;
	Edict*	camera;
	Edict*	player;
	u32		steps;

	u64		randomSeed;
	u64		random;

} GameLocal;



extern u64 Random(u64 min, u64 max);
extern void SeedRNG();

GameLocal game;
Map level;

#endif