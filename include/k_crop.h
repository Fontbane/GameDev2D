#ifndef __K_CROP_H__
#define __K_CROP_H__

typedef enum FruitStage {
	STAGE_SEED,
	STAGE_SPROUT,
	STAGE_SAPLING,
	STAGE_BLOOM,
	STAGE_FRUIT
}FruitStage;

typedef enum Fruit {
	FRUIT_APPLE,
	FRUIT_BLUEBERRY,
	FRUIT_STRAWBERRY,
	FRUIT_RASPBERRY,
	FRUIT_PEAR,
	FRUIT_CHERRY,
	FRUIT_GRAPE,
	FRUIT_COCOA,
	FRUIT_PAPAYA,
	FRUIT_LEMON,
	FRUIT_ORANGE,
	FRUIT_LIME,
	FRUIT_BLACKBERRY,
	FRUIT_GRAPEFRUIT,
	FRUIT_PASSIONFRUIT,
	FRUIT_DRAGONFRUIT,
	FRUIT_COCONUT,
	FRUIT_BANANA,
	FRUIT_PERSIMMON,
	FRUIT_DURIAN,
	FRUIT_EGGPLANT,
	FRUIT_PEANUT,
	FRUIT_PUMPKIN,
	FRUIT_SQUASH,
	FRUIT_PEA,
	FRUIT_TOMATO,
	FRUIT_POTATO,
	FRUIT_CARROT,
	FRUIT_CABBAGE,
	FRUIT_SPINACH,
	FRUIT_CORN,
	FRUIT_SUGARCANE,
	FRUIT_WHEAT,
	FRUIT_RICE,
	FRUIT_COFFEE,
	FRUIT_MAX
} Fruit;

typedef enum CropType {
	CROP_GRAIN,
	CROP_FRUIT,
	CROP_VEGGIE,
	CROP_STARCH,
	CROP_MAX
} CropType;

#endif