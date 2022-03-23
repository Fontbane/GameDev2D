#ifndef __K_ENTITY_H__
#define __K_ENTITY_H__

#define WORLD_TO_CELL(c) {(int) c.x / 16, (int) c.y/16}
#define CELL_TO_WORLD(c) {c.x * 16.0f, c.y * 16.0f}
#define cell_add(dst, a, b) (dst.x=a.x+b.x, dst.y=a.y+b.y)
#define cell_cmp(a, b) (a.x==b.x&&a.y==b.y)

#include "gf2d_sprite.h"

#include "k_global.h"
#include "k_crop.h"
#include "queue.h"

Point8 cell(s8 x, s8 y);

Point8 world_to_cell(Vector2D pos);
Vector2D cell_to_world(Point8 pos);

typedef struct WARP_S {
	u16 mapID; //0 to stay on the same map
	u8 connection; //the id of them warp this one connects to
	u8 warpEffect; //any special fx the warp does, like spinning the player
} Warp;

typedef struct NPC {
	u16 tamerID; //Also used for items
	u8 sight;
} NPC;

typedef struct CropPlant {
	Fruit cropID;
	u8 dryness;
	u8 yield;
	FruitStage stage;

	time_t planted;
} CropPlant;//12 bytes


typedef struct Camera {
	float zoom;
} Camera;

typedef struct Player {
	TextWord name;
	u16 id;
	u8 gender : 1;
	u8 costume : 7;
} Player;

typedef union EventInfo {
	Player p;
	Warp w;
	Camera c;
	NPC n;
	CropPlant f;
}EventInfo;

typedef struct MovementInfo {
	Point		startp;
	Point		endp;
	Vector2D	starto;
	Vector2D	endo;
	float		velocity;
	float		accel;
	float		distance;
	Queue		moveq;


	u16			following;	/*<ID of entity to copy movements of*/
	float		wait;
	float		remaining_distance;
}MovementInfo;


enum {
	EV_NPC,
	EV_PLAYER,
	EV_CROP,
	EV_CAMERA,
	EV_ITEM,
	EV_TAMER
};

typedef struct EDICT_S{
	u16			localID;		/**<local ID of the entity. 0 should always be the camera, 1 the player*/
	u8			eventType;
	u8			z;

	Point8		cellPos;
	Point8		targetCell;

	Sprite*		sprite;
	Vector2D	draw_position;
	Vector2D	draw_offset;
	Vector2D	draw_scale;
	Vector2D	position;
	Rect8		hitbox;
	Vector2D	targetPosition;
	Direction	facing;
	float		frame;

	EventInfo*	info;
	MovementInfo moveinfo;
	float		velocity;
	Queue		moveq;

	/*FLAGS*/
	u16			inUse : 1;
	u16			collidable : 1;
	u16			moving : 1;

	void (*Think)(struct EDICT_S* self);
	void (*Move)(struct EDICT_S* self, Direction dir);
	void (*OnTalk)(struct EDICT_S* self, Direction dir);
	void (*OnCollide)(struct EDICT_S* self, Direction dir);
} Edict;

Edict *player;
Edict *boy;

Edict* ent_new();

void ent_move(Edict* ent);

void ent_draw(Edict* ent);

void ent_free(Edict* ent);

void ent_manager_init(u32 maxEnts);

void ent_manager_clear();

void ent_think_generic(Edict* ent);
void ent_collide_generic(Edict* ent, Direction dir);

void coll_mukchuk(Edict* ent, Direction dir);
void coll_tree(Edict* ent, Direction dir);

void talk_showme_egglet(Edict* ent, Direction dir);
void talk_treehugger(Edict* ent, Direction dir);
void talk_find_mukchuk(Edict* ent, Direction dir);
void talk_riddler(Edict* ent, Direction dir);
void talk_give_restorade(Edict* ent, Direction dir);

void ent_think(Edict* ent);
void ent_manager_think_all();
void ent_manager_draw_all();

int Colliding(Rect a, Rect b);

/**
* WarpPlayer
* @brief: Warps the player to a different location, possibly loading a new map
* @param: warp the struct for the warp event, contains map ID and position as well as any effect
*/
void WarpPlayer(Warp warp);
extern CropPlant PlantFruit(u8 fruitID);
extern void WaterFruit(CropPlant* plant);
extern void PickFruit(CropPlant* plant);

u16 CheckEntCollision(Point8 pt, Direction dir);

#endif