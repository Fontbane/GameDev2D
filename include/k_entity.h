#ifndef __K_ENTITY_H__
#define __K_ENTITY_H__

#include "gf2d_sprite.h"

#include "k_global.h"
#include "k_crop.h"

typedef struct WARP_S {
	u16 mapID; //0 to stay on the same map
	struct WARP_S* connection; //the warp this one connects to
	u8 warpEffect; //any special fx the warp does, like spinning the player
} Warp;

typedef struct NPC {
	u16 tamerID;
	u8 sight;
} NPC;

typedef struct CropPlant {
	Fruit cropID;
	u8 dryness;
	CropType type;
	FruitStage stage;
	time_t planted;
} CropPlant;


typedef struct Camera {
	float zoom;
} Camera;

typedef struct Player {
	TextWord name;
	u8 gender : 1;
	u8 costume : 7;
} Player;

typedef union EventInfo {
	Player* p;
	Warp* w;
	Camera* c;
	NPC* n;
	CropPlant* f;
}EventInfo;

typedef struct EDICT_S{
	u8			localID;		/**<local ID of the entity. 0 should always be the camera, 1 the player*/
	Sprite*		sprite;
	Vector2D	draw_offset;
	Vector2D	draw_scale;
	Vector2D	position;
	Vector2D	velocity;
	Vector2D	mins, maxs;		/**<Hitbox*/
	Vector2D	targetPosition;
	Bool		collidable;
	u8			z;
	u8			following;		/**<ID of entity to copy movements of*/
	Direction	facing;
	float		frame;

	EventInfo*	info;

	/*FLAGS*/
	u16			inUse : 1;

	void (*Think)(struct EDICT_S* self);
	void (*Move)(Vector2D movement);
	void (*OnTalk)(Direction dir);
	void (*OnCollide)(Direction dir);
} Edict;

Edict *player;
Edict *boy;

Edict* ent_new();

void ent_draw(Edict* ent);

void ent_free(Edict* ent);

void ent_manager_init(u32 maxEnts);

void ent_manager_clear();

void ent_think_generic(Edict* ent);

void ent_think(Edict* ent);
void ent_manager_think_all();
void ent_manager_draw_all();

/**
* WarpPlayer
* @brief: Warps the player to a different location, possibly loading a new map
* @param: warp the struct for the warp event, contains map ID and position as well as any effect
*/
void WarpPlayer(Warp warp);
extern CropPlant PlantFruit(u8 fruitID);
extern void WaterFruit(CropPlant* plant);
extern void PickFruit(CropPlant* plant);

#endif