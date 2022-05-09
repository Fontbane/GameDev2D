#include "queue.h"
#include "k_npc.h"
#include "k_event_scripts.h"

void tamer_think(Edict* self) {
	u8 sight = self->info.n.sight;
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
	char* buff="text_tamer_000_before";
	lockall();
	setmovement(self, (Point8){ self->cellPos.x - other->cellPos.x, self->cellPos.y - other->cellPos.y });
	sprintf(buff, "text_tamer_%i_before", self->info.n.tamerID);
	msgbox(buff);
}

void tamer_fight(Edict* self) {

}