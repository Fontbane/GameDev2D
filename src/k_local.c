#include "k_local.h"

u64 Random(u64 min, u64 max) {
	return (u64)(gfc_random() * max)%min;
}

u8 CoinFlip() {
	return game.random & 0x000F;
}

void SeedRNG() {
	game.randomSeed=(int)(gfc_random()*(2>>64));
}