#ifndef __K_PALETTE_H__
#define __K_PALETTE_H__

#include <SDL_image.h>

#include "gf2d_sprite.h"
#include "gfc_hashmap.h"
#include "k_monster.h"

typedef SDL_Palette Palette;
HashMap *gPalettes;

void Palettes_Init();

//Converts filename into key for palette hashmap
char* GetPalNameFromFilename(char* filename);

Sprite* GetMonsterSprite(MonDict mon);

Palette* GetMonsterPalette(char* monsterName);

Palette* GetMonsterShinyPalette(char* monsterName);

//Gets the palette from the hashmap
Palette* GetPalette(char* palname);

//Loads formatted palette from .pal file, inserts it into the gPalettes hashmap, then returns the palette
Palette* LoadPalette(char* filename, char nColors);

//Loads a sprite with specified palette
Sprite* LoadSpriteWithPalette(char* spritefile, Palette* pal);

//Loads a sprite with specified palette in multiple frames
Sprite* LoadSpriteFramesWithPalette(char* filename, Palette* pal,
    Sint32 frameWidth,
    Sint32 frameHeight,
    Sint32 framesPerLine);

#endif