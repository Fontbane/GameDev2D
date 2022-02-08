#ifndef __K_FIELD_H__
#define __K_FIELD_H__

#include "k_entity.h"

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
    MB_SLOPE,
    MB_SOIL
} MetatileBehavior;

typedef struct Block {
    Sprite sprite;
    Vector2D flip;
    MetatileBehavior behavior;
    u8 flags;
	void (*OnTalk)();
	void (*OnCollide)();
} Block;

extern Block CrackedTile;
extern Block TallGrass;
extern Block WaterEdge;
extern Block Ledge;

typedef struct Tileset {
	Sprite*     sheet;
	Block*      tiles;
    u16         tileCount;
    TextLine    filename;
    u8          refCount;
} Tileset;

Tileset gTilesets[16];
extern void SetUpTilesets();

void tile_manager_init(u8 tilesetCount);
void tileset_load(char* filename);
void tileset_free(Tileset tileset);
void tileset_draw(Tileset tileset, u16 tile, Vector2D position);

typedef struct MapTile_s{
    u16 blockID:12;
	u16 collision:4;
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
	u16             id;
	u16             outdoors : 1;
	Tileset         tileset;
    MapConnection   north;
    MapConnection   south;
    MapConnection   west;
    MapConnection   east;
    u8              height;
    u8              width;
    Block*          border;
    Encounter*      encounters; //Array of 12 random encounters with odds
                                //20% 20% 10% 10% 10% 10% 5% 5% 4% 4% 1% 1%
    Edict*          entities;
    MapTile*        layout;
} Map;

void GetZ(Vector2D pos);
extern void RenderMap(Map map);

TileCollision GetCollisionAt(Map map, Vector2D position);

Map gmap;


#endif