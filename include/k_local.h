#ifndef __LOCAL_H__
#define __LOCAL_H__

#include "k_field.h"
#include "gf2d_windows.h"

typedef struct GameLocal {
	u64		framenum;
	time_t	time;
	char	*map;
	Edict*	entities;
	Edict*	camera;
	Edict*	player;
	u32		steps;
	int		camera_offset_x;
	int		camera_offset_y;

	GameState state;
	Bool	lockall;

	u64		randomSeed;
	u64		random;

	Window* hud;

} GameLocal;



extern u64 Random(u64 min, u64 max);
extern u32 Random32();
extern void SeedRNG();

void game_init();

GameLocal game;
Map level;

#endif