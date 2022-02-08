#include "simple_json.h"

#include "k_field.h"
#include "k_monster.h"

typedef struct SaveBlock1 {
	u16 map;
	MonDict* party;
	Player client;
} SaveBlock1;

SaveBlock1 save1;

extern SaveBlock1 LoadSave(char* filename);