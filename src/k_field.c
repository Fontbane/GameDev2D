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
	gTilesets[0].sheet = gf2d_sprite_load_all("images/tiles/kt01.png", 256, 1024, 16); //16 tiles wide 64 tiles tall
	gTilesets[0].tiles[1] = TallGrass;
}

static Vector2D tileScale = { 2, 2 };

void RenderMap(Map map) {
	for (int y = 0; y < map.height; y++) {
		for (int x = 0; x < map.width; x++) {
			MapTile* block = map.layout+y*2+x*2; //2 = sizeof(MapTile)
			u16 collisions = block->collision;
			gf2d_sprite_draw(map.tileset.sheet, vector2d(16 * x, 16 * y), &tileScale, NULL, NULL, NULL, NULL, block->blockID);
		}
	}
}

