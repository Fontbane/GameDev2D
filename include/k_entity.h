#ifndef __K_ENTITY_H__
#define __K_ENTITY_H__

#include "gf2d_sprite.h"
#include "k_global.h"
#include "k_crop.h"

typedef struct Edict{
	u8			localID; //local ID of the entity. 0 should always be the camera, 1 the player
	Sprite*		sprite;
	Vector3D	position;
	Vector2D	targetPosition;
	Bool		collidable;
	u8			following; //ID of entity to copy movements of
	u16			alternateID; //used for item, berry, map, tamer ID
	u16			flag; //store flags or variables

	void (*Think)();
	void (*Move)(Vector2D movement);
	void (*OnTalk)(Direction dir);
	void (*OnCollide)(Direction dir);
} Edict;

typedef struct Warp {
	Edict ent;
	Vector3D warpPosition;
	u8 warpEffect;
} Warp;

typedef struct NPC {
	Edict ent;
	u8 sight;
} NPC;

typedef struct CropPlant {
	Edict ent;
	u8 dryness;
	FruitStage stage;
	time_t planted;
} CropPlant;


typedef struct Camera {
	Edict ent;
	float zoom;
} Camera;

typedef struct Player {
	Edict ent;
	u8 gender : 1;
	u8 costume : 7;
} Player;

void WarpPlayer(Warp warp);
extern CropPlant PlantFruit(u8 fruitID);
extern void WaterFruit(CropPlant* plant);
extern void PickFruit(CropPlant* plant);

#endif