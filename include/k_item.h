#ifndef __K_ITEM_H__
#define __K_ITEM_H__

#include "k_crop.h"
#include "k_global.h"
#include "gfc_text.h"

typedef enum {
	POCKET_MISC,
	POCKET_TREASURE,
	POCKET_CROP,
	POCKET_POWER_UP,
	POCKET_MEDICINE,
	POCKET_CAPSULE,
	POCKET_KEY
} Pocket;

typedef struct ITEM_S {
	u16 id;
	TextWord name;
	Pocket pocket;
	Fruit crop;
	CropType cropType;
	u16 price;//times 10
	u8 useOnField : 1;
	u8 useInBattle : 1;
	u16 useargs;
	void (*Use)(struct ITEM_S* item);
}Item;

int RemoveItem(char *name);
void GiveItem(char* name);
Item GetItemFromJson(char* name);
Item* gItems;

#endif // !__K_ITEM_H__
