#ifndef __K_ITEM_H__
#define __K_ITEM_H__

#include "k_crop.h"
#include "k_global.h"
#include "gfc_text.h"
#include "gfc_hashmap.h"

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

enum {
	ITEM_NONE,
	ITEM_RESTORADE,
	ITEM_RESTORADE_SILVER,
	ITEM_RESTORADE_GOLD,
	ITEM_MILK_FAT,
	ITEM_PROTEIN_POWDER,
	ITEM_CALCIUM_TABLET,
	ITEM_COBALAMIN_PILL,
	ITEM_ZINC_SPRAY,
	ITEM_CARBO_CRUNCH,
	ITEM_CAPSULE,
	ITEM_NUGGET,
	ITEM_PEARL,
	ITEM_MAX
};

int RemoveItem(u16 itemID);
void GiveItem(u16 itemID);
Item GetItemFromJson(u16 itemID);
u16 ItemIDFromJson(char* name);
Item gItems[ITEM_MAX];
HashMap* gItemUseFuncs;
HashMap* gItemIDs;
TextLine gItemInternal[ITEM_MAX];

#endif // !__K_ITEM_H__
