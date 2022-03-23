#include "k_menu.h"
#include "k_entity.h"
#include "k_save.h"
#include "simple_logger.h"

void DrawHUD() {
	int i;
	Vector2D drawpos = vector2d(8, 0);
	Vector2D drawscale = vector2d(1, 1);
	for (i = 0; i < 6; i++) {
		u16 species = party.partyPersonal[i].species;
		if (species == 0 || species > SPECIES_MAX) {
			continue;
		}
		Sprite* sprite = gf2d_sprite_load_all(gBaseStats[species].spritefile,32,64,2);
		gf2d_sprite_draw(sprite, drawpos, &drawscale, NULL, NULL, NULL, NULL, 0);
		drawpos.x += 32;
	}
}