#include "simple_logger.h"

#include "k_palette.h"

void Palettes_Init() {
	gPalettes = gfc_hashmap_new();
}

//Converts filename into key for palette hashmap
char* GetPalNameFromFilename(char* filename) {
	if (!filename) {
		slog("Func GetPalNameFromFilename: Palette filename not specified");
		return NULL;
	}
	char* lastSlash = strrchr(filename, '/')-1;
	while (strchr(lastSlash,'/') != NULL && lastSlash[0] != '/') lastSlash--;
	lastSlash++;
	lastSlash[strchr(lastSlash, '/') - lastSlash] = '_';
    strchr(lastSlash, '.')[0] = '\0';
	return lastSlash;
}

Sprite* GetMonsterSprite(MonDict mon) {
    char* filename = "images/battlers/";
    char* fileend = "/front.pcx";
    strcat(filename, gBaseStats[mon.species].name);
    strcat(filename, fileend);
    if (mon.shiny) {
        return LoadSpriteWithPalette(filename, GetMonsterShinyPalette(gBaseStats[mon.species].name));
    }
    return LoadSpriteWithPalette(filename, GetMonsterPalette(gBaseStats[mon.species].name));
}

Palette* GetMonsterPalette(char* monsterName) {
    char* filename = "images/battlers/";
    char* fileend = "/normal.pal";
    char* palname = "_normal";
    strcat(filename, monsterName);
    strcat(filename, fileend);
    strcat(monsterName, palname);
    Palette* pal;
    if (!(pal = GetPalette(monsterName))) {
        LoadPalette(filename, 16);
    }
    return GetPalette(monsterName);
}

Palette* GetMonsterShinyPalette(char* monsterName) {
    char* filename = "images/battlers/";
    char* fileend = "/shiny.pal";
    char* palname = "_shiny";
    Palette* pal;
    strcat(filename, monsterName);
    strcat(filename, fileend);
    strcat(monsterName, palname);
    if (!(pal = GetPalette(monsterName))) {
        LoadPalette(filename, 16);
    }
    return GetPalette(monsterName);
}

//Gets the palette from the hashmap
Palette* GetPalette(char* palname) {
    return gfc_hashmap_get(gPalettes, palname);
}

//Loads formatted palette from .pal file, inserts it into the gPalettes hashmap, then returns the palette
Palette* LoadPalette(char* filename, char nColors) {
	FILE* file;
	if (!filename) {
		slog("Func LoadPalette: No filename provided.");
		return NULL;
	}
    Palette* pal=SDL_AllocPalette(nColors);
	SDL_Color* colors;
	int rgb[3];
	int c, i;
	char trash[20];
	colors = calloc(nColors, sizeof(SDL_Color));
	file = fopen(filename, "r");
	if (!file) {
		slog("Func LoadPalette: Could not open file %s", filename);
		return NULL;
	}
	if (!colors) {
		slog("Func LoadPalette: Wasn't able to allocate memory for colors or palette");
		return NULL;
	}
	fgets(trash, 20, file);
	fgets(trash, 20, file);
	fgets(trash, 20, file);
	c = 0;
	while (!feof(file) && c < nColors) {
		for (i = 0; i < 3; i++) {
			fscanf(file, "%d", &rgb[i]);
		}
		colors[c].r = rgb[0];
		colors[c].g = rgb[1];
		colors[c].b = rgb[2];
	}
	colors[0].a = 0; //First color in a palette is transparent
	slog("%d colors loaded from %s", c, filename);
    SDL_SetPaletteColors(pal, colors, 0, nColors);
	gfc_hashmap_insert(gPalettes, GetPalNameFromFilename(filename), pal);
    free(trash);
	return pal;
}

Sprite* LoadSpriteWithPalette(char* filename, Palette* pal) {
    return LoadSpriteFramesWithPalette(filename, pal, -1, -1, 1);
}

//Loads a sprite with specified palette
Sprite* LoadSpriteFramesWithPalette(char* filename, Palette* pal, Sint32 frameWidth,
    Sint32 frameHeight,
    Sint32 framesPerLine) {
    SDL_Surface* surface = NULL;
    Sprite* sprite = NULL;

    sprite = gf2d_sprite_get_by_filename(filename);
    if (sprite != NULL)
    {
        // found a copy already in memory
        sprite->ref_count++;
        return sprite;
    }

    sprite = gf2d_sprite_new();
    if (!sprite)
    {
        return NULL;
    }

    surface = IMG_Load(filename);
    if (!surface)
    {
        slog("failed to load sprite image %s", filename);
        gf2d_sprite_free(sprite);
        return NULL;
    }

    surface = gf2d_graphics_screen_convert(&surface);
    if (!surface)
    {
        slog("failed to load sprite image %s", filename);
        gf2d_sprite_free(sprite);
        return NULL;
    }
    ;
    if (SDL_SetSurfacePalette(&surface, pal) != 0) {
        slog("Func LoadSpriteFramesWithPalette: Something went wrong setting the surface palette");
        return NULL;
    }

    sprite->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surface);
    if (!sprite->texture)
    {
        slog("failed to load sprite image %s", filename);
        gf2d_sprite_free(sprite);
        SDL_FreeSurface(surface);
        return NULL;
    }
    SDL_SetTextureBlendMode(sprite->texture, SDL_BLENDMODE_BLEND);
    SDL_UpdateTexture(sprite->texture,
        NULL,
        surface->pixels,
        surface->pitch);
    if (frameHeight == -1)
    {
        sprite->frame_h = surface->h;
    }
    else sprite->frame_h = frameHeight;
    if (frameWidth == -1)
    {
        sprite->frame_w = surface->w;
    }
    else sprite->frame_w = frameWidth;
    sprite->frames_per_line = framesPerLine;
    gfc_line_cpy(sprite->filepath, filename);

    SDL_FreeSurface(surface);
    return sprite;
}