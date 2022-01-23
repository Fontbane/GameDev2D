#ifndef __FIELD_H__
#define __FIELD_H__

#include "global.h"
#include "entity.h"
#include "gf2d_sprite.h"

typedef enum {
    COLL_IMPASSIBLE,
    COLL_WATER,
    COLL_2,
    COLL_3,
    COLL_4,
    COLL_5,
    COLL_6,
    COLL_TRANSITION,
    COLL_MULTILEVEL
} TileCollision;

typedef enum MetatileBehavior{
    MB_NONE,
    MB_CRACKED,
    MB_HOLE,
    MB_GRASS,
    MB_MUD,
    MB_WATER,
    MB_LEDGE,
    MB_CONVEYER,
    MB_SLOPE
} MetatileBehavior;

typedef struct Block {
	Vector2D flip;
    MetatileBehavior behavior;
	void (*OnTalk)();
	void (*OnCollide)();
} Block;

extern Block CrackedTile;
extern Block TallGrass;
extern Block WaterEdge;
extern Block Ledge;

typedef struct Tileset {
	Sprite sheet;
	Block* tiles;
} Tileset;

typedef struct {
	u16 tileID : 10;
	u16 collision : 6;
} MapTile;

typedef struct MapConnection {
    u16 mapID;
    Vector2D offset;
} MapConnection;

typedef struct Encounter {
    u16 species : 9;
    u16 level : 7;
} Encounter;


typedef struct Map{
	u16 id;
	u16 outdoors : 1;
	Tileset tileset;
    MapConnection north;
    MapConnection south;
    MapConnection west;
    MapConnection east;
    Block* border;
    Encounter* encounters; //Array of 12 random encounters with odds
                           //20% 20% 10% 10% 10% 10% 5% 5% 4% 4% 1% 1%
    Edict* entities;

} Map;

void WarpPlayer(Warp warp);
void ClearEnts(void);


#endif