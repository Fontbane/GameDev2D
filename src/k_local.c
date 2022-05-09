#include "k_local.h"

u64 Random(u64 min, u64 max) {
	return (u64)(gfc_random() * max)%min;
}

u8 CoinFlip() {
	return game.random & 0x000F;
}

u32 Random32() {
	return (u32)(gfc_random()*SDL_MAX_UINT32);
}

void SeedRNG() {
	game.randomSeed=(int)(gfc_random()*SDL_MAX_UINT64);
}

void game_init() {
	game.steps = 0;
	game.time = 0;
	game.state = GAMESTATE_FIELD;
	game.camera_offset_x = 128;
	game.camera_offset_y = 96;
}