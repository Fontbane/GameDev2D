#include <stdio.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "simple_json.h"

#include "k_field.h"

typedef struct {
	Tileset*	tilesets;
	u8			tilesetCount;
}TilesetManager;

static TilesetManager tileManager = { 0 };

void tile_manager_init(u8 tilesetCount) {
	if (tilesetCount <= 0) {
		slog("Cannot init zero tilesets");
		return;
	}
	tileManager.tilesets = (Tileset*)gfc_allocate_array(sizeof(Tileset), tilesetCount);
	tileManager.tilesetCount = tilesetCount;
}

void tileset_draw(Tileset* tileset,u16 tile,Vector2D position) {
	gf2d_sprite_draw(
		tileset->sheet,
		position,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		tile
	);
}

Block CrackedTile = {
	0,
	.flip= 0,
	MB_CRACKED
};

Block TallGrass = {
	0
};

void SetUpTilesets() {
	gTilesets[0].sheet = gf2d_sprite_load_all("images/tiles/kt01.png", 16, 16, 16); //16 tiles wide 64 tiles tall
	strcpy(gTilesets[0].filename, "images/tiles/kt01.png");
}

static Vector2D tileScale = { 1, 1 };

Map* map_new() {
	Map* map;
	map = (Map*)malloc(sizeof(Map));
	if (!map)
	{
		slog("failed to allocate memory for a new tilemap");
		return NULL;
	}
	memset(map, 0, sizeof(Map));
	return map;
}

Map *LoadMap(const char* jsonfile) {
	FILE* file;
	u16* buff;
	long length;

	SJson* json;
	char* spriteFile = NULL;
	char* mapFile = NULL;
	Map *map;
	map = map_new();
	slog(jsonfile);
	int height, width;
	if (!jsonfile) return NULL;
	json = sj_load(jsonfile);
	if (!json) {
		slog("Couldn't load map file");
		return NULL;
	}
	map->name = jsonfile;
	spriteFile = sj_get_string_value(sj_object_get_value(json, "tileset"));
	mapFile = sj_get_string_value(sj_object_get_value(json, "layout"));
	sj_get_integer_value(sj_object_get_value(json, "height"), &height);
	sj_get_integer_value(sj_object_get_value(json, "width"), &width);
	map->height=height;
	map->width = width;

	file = fopen(mapFile, "rb");
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	slog("%d", length);
	rewind(file);
	buff = (u16*)malloc(length * sizeof(u16));
	if (buff) fread(buff, length, 1, file);
	fclose(file);
	map->layout = (MapTile*) buff;

	map->tileset = gTilesets[0];

	sj_free(json);
	return map;
}

void RenderMap(Map* map) {
	if (!map) return;
	for (int y = 0; y < map->height; y++) {
		for (int x = 0; x < map->width; x++) {
			MapTile* block = &map->layout[y*map->width+x];
			u16 collisions = block->collision;
			gf2d_sprite_draw(
				map->tileset.sheet,
				vector2d(16 * x, 16 * y),
				&tileScale,
				NULL,
				NULL,
				NULL,
				NULL,
				block->blockID-256);
		}
	}
}

TileCollision GetCollisionAt(Map *map, Point8 position) {
	/*Chunk chunk = map->chunks[position.y * map->width / 256 + position.x / 16];
	int i;
	for (i = 0; i < chunk.numEnts; i++) {
		if (chunk.localents[i].cellPos.x == position.x && chunk.localents[i].cellPos.y == position.y && chunk.localents[i].collidable) {
			return COLL_IMPASSIBLE;
		}
	}*/
	return map->layout[position.y * map->width + position.x].collision;
}

void PrintLayout(Map* map) {
	int i, j;
	for (i = 0; i < map->height; i++) {
		for (j = 0; j < map->width; j++) {
			slog("%d ",map->layout[i * map->width + j].collision);
		}
	}
}

