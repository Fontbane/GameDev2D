#include "k_npc.h"

void tamer_think(Edict* self) {
	u8 sight = self->info->n.sight;
	switch (self->facing) {
		case DIR_N:
		case DIR_S:
			if (self->position.x == player->position.x &&
				self->position.y - player->position.y < sight * directions[self->facing].y)
					tamer_see(self,&player);
		case DIR_W:
		case DIR_E:
			if (self->position.y == player->position.y &&
				self->position.x - player->position.x < sight * directions[self->facing].x)
					tamer_see(self,&player);
	}
}

void tamer_see(Edict* self, Edict* other) {

}

void tamer_fight(Edict* self) {

}