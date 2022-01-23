#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "global.h"
#include "gf2d_sprite.h"

typedef struct Edict{
	u8			localID; //local ID of the entity. 0 should always be the camera, 1 the player
	char*		entitySprite; //0 for none
	Vector3D	position;
	Vector2D	targetPosition;
	Bool		collidable;
	u8			following; //ID of entity to copy movements of

	void (*Move)(Vector2D movement);
	void (*OnTalk)(Direction dir);
	void (*OnCollide)(Direction dir);
} Edict;

typedef struct Warp {
	u16 map;
	Vector3D warpPosition;
	u8 warpEffect;
} Warp;

typedef struct Signpost {
	u16 item;
} Signpost;

typedef struct NPC {
	u16 tamerID;
	u8 sight;
} NPC;

#endif