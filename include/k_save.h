#include "simple_json.h"

#include "k_field.h"
#include "k_monster.h"

typedef struct TheBalls {
	Map* map;
	Point8 position;
	Player client;
	time_t playTime;
	u64 quests;
} SaveBlock1;

typedef struct SaveBlock2 {
	MonDict* party;
	PersonalDict* partyPersonal;
	PersonalDict* boxMons;
}SaveBlock2;

typedef struct Options {
	u8 textSpeed;
	u8 textFrame;
	float soundLv;
}Options;

SaveBlock1 save1;

extern SaveBlock1	*LoadSave(char* filename);
extern void			WriteSave(char* filename);
extern void			NewSave(char* filename);