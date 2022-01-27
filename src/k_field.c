#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "k_field.h"

Block CrackedTile = {
	0,
	.flip= 0,
	MB_CRACKED
};

Block TallGrass = {
	0
};

void SetUpTilesets() {
	gTilesets[0].sheet = gf2d_sprite_load_all("images/tiles/kt01.png", 256, 2048, 16);
	gTilesets[0].tiles[1] = TallGrass;
}

