#ifndef __K_ITEM_H__
#define __K_ITEM_H__

#include "k_crop.h"
#include "k_global.h"

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
	Pocket pocket;
	Fruit crop;
	CropType cropType;
	u8 useOnField : 1;
	u8 useInBattle : 1;
}Item;

#endif // !__K_ITEM_H__
