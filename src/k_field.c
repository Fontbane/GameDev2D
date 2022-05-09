#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gfc_text.h"
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
	gfc_line_cpy(gTilesets[0].filename, "images/tiles/kt01.png");
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
	SJson *json;
	char* spriteFile = NULL;
	Map *map = NULL;
	int t;
	MapTile tile=0;
	TileCollision coll=0;
	int height, width;

	map = map_new();
	if (!map)return NULL;
	slog("attempting to load json file %s", jsonfile);
	if (!jsonfile) return NULL;
	json = sj_load(jsonfile);
	if (!json) {
		slog("Couldn't load map file %s",jsonfile);
		return NULL;
	}
	map->name = jsonfile;
	spriteFile = sj_get_string_value(sj_object_get_value(json, "tileset"));
	sj_get_integer_value(sj_object_get_value(json, "height"), &height);
	sj_get_integer_value(sj_object_get_value(json, "width"), &width);
	map->height = height;
	map->width = width;
	map->layout = (MapTile*)calloc(height*width,sizeof(MapTile));
	map->layer2 = (MapTile*)calloc(height*width,sizeof(MapTile));
	map->collision = (TileCollision*)calloc(height*width,sizeof(TileCollision));

	for (t = 0; t < height * width; t++) {
		sj_get_integer_value(sj_array_get_nth(sj_object_get_value(json, "layer_1"),t), map->layout + t);
	}
	for (t = 0; t < height * width; t++) {
		sj_get_integer_value(sj_array_get_nth(sj_object_get_value(json, "layer_2"), t), map->layer2 + t);
	}
	for (t = 0; t < height * width; t++) {
		sj_get_integer_value(sj_array_get_nth(sj_object_get_value(json, "collisions"), t), map->collision + t);
	}

	LoadEntitiesFromJson(json);

	/*FILE* file;
	u16 *buff, *buff2;
	u8 *buffc;
	long length;
	
	file = fopen(mapFile, "rb");
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	slog("Read %d bytes for layout", length);
	rewind(file);
	buff = (u16*)malloc(length * sizeof(u16));
	if (buff) fread(buff, length, 1, file);
	fclose(file);
	map->layout = buff;

	mapFile = sj_get_string_value(sj_object_get_value(json, "layer2"));
	file = fopen(mapFile, "rb");
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	slog("Read %d bytes for layer 2", length);
	rewind(file);
	buff2 = (u16*)malloc(length * sizeof(u16));
	if (buff2) fread(buff2, length, 1, file);
	fclose(file);
	map->layer2 = buff2;

	mapFile = sj_get_string_value(sj_object_get_value(json, "collision"));
	file = fopen(mapFile, "rb");
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	slog("Read %d bytes for collision", length);
	rewind(file);
	buffc = (u8*)malloc(length * sizeof(u8));
	if (buffc) fread(buffc, length, 1, file);
	fclose(file);
	map->collision = (TileCollision*)buffc;*/

	map->tileset = gTilesets[0];

	sj_free(json);
	return map;
}

void RenderMap(Map* map) {
	int y, x, ry, rx, t;
	if (!map) return;
	for (ry=0, y = max(player->cellPos.y-6,0); y < min(player->cellPos.y + 6,map->height); ry++, y++) {
		for (rx=0, x = max(player->cellPos.x - 8,0), t=y*map->width+x; x < min(player->cellPos.x + 8,map->width); rx++, x++, t++) {
			gf2d_sprite_draw(
				map->tileset.sheet,
				vector2d(16 * rx, 16 * ry),
				&tileScale,
				NULL,
				NULL,
				NULL,
				NULL,
				map->layout[t]-1);
		}
	}
}

void RenderMapLayer2(Map* map) {
	int y, x, ry, rx, t;
	if (!map) return;
	for (ry = 0, y = max(player->cellPos.y - 6, 0); y < min(player->cellPos.y + 6, map->height); ry++, y++) {
		for (rx = 0, x = max(player->cellPos.x - 8, 0), t = y * map->width + x; x < min(player->cellPos.x + 8, map->width); rx++, x++) {
			if (map->layer2[t] == 0) {
				continue;
			}
			gf2d_sprite_draw(
				map->tileset.sheet,
				vector2d(16 * rx, 16 * ry),
				&tileScale,
				NULL,
				NULL,
				NULL,
				NULL,
				map->layer2[t++]-1);
		}
	}
}

TileCollision GetCollisionAt(Map *map, Point8 position) {
	if (!map) return 0;
	return map->collision[position.y*map->width+position.x];
}

void PrintLayout(Map* map) {
	int i, j;
	for (i = 0; i < map->height; i++) {
		for (j = 0; j < map->width; j++) {
			slog("%d ",map->collision[i * map->width + j]);
		}
	}
}

