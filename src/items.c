#include "items.h"
#include "level.h"
#include "simple_logger.h"

typedef struct
{
    Item *items;
    Uint32 count;
}ItemManager;

static ItemManager item_manager = {NULL,0};

void item_draw(Entity *self);
void item_think(Entity *self);
void item_update(Entity *self);
int  item_touch(Entity *self,Entity *other);
void item_damage(Entity *self,int amount, Entity *source);
void item_die(Entity *self);

void item_manager_close()
{
    if (item_manager.items != NULL)
    {
        free (item_manager.items);
    }
    slog("item manager closed");
}

void item_manager_init()
{
    item_manager.count = 0;
    item_manager.items = NULL;
    atexit(item_manager_close);
    slog("item manager initialized");
}

Item *item_get_by_name(char *name)
{
    int i = 0;
    for (i = 0; i < item_manager.count;i++)
    {
        if (gf2d_line_cmp(item_manager.items[i].name,name) == 0)
        {
            return &item_manager.items[i];
        }
    }
    slog("failed to find an item named '%s'",name);
    return NULL;
}

Entity *item_spawn(char *name,Vector2D position)
{
    Entity *self;
    Item *item;
    item = item_get_by_name(name);
    if (!item)
    {
        return NULL;
    }
    self = gf2d_entity_new();
    if (!self)
    {
        return NULL;
    }
    gf2d_line_cpy(self->name,item->name);
    self->parent = NULL;
    
    self->shape = gf2d_shape_rect(-32, -16, 64, 48);
    gf2d_body_set(
        &self->body,
        item->name,
        0,
        2,
        position,
        vector2d(0,0),
        10,
        0,
        0,
        &self->shape,
        self,
        NULL,
        NULL);

    self->sprite = gf2d_sprite_load_all(item->sprite,item->spriteWidth,item->spriteHeight,item->spriteFPL);

    self->frame = (gf2d_random()*16);
    self->al = gf2d_action_list_load(item->actor);
    gf2d_line_cpy(self->action,"idle");
    
    vector2d_copy(self->position,position);
    
    vector2d_set(self->scale,1,1);
    vector2d_set(self->scaleCenter,64,64);
    vector3d_set(self->rotation,64,64,0);
    self->color = gf2d_color8(255,255,255,255);
    
    self->pe = gf2d_particle_emitter_new(50);
    
    self->think = item_think;
    self->draw = item_draw;
    self->update = item_update;
    self->touch = item_touch;
    self->damage = item_damage;
    self->die = item_die;
    self->free = level_remove_entity;

    return self;
}

void item_draw(Entity *self)
{
    
}

void item_think(Entity *self)
{
    
}

void item_update(Entity *self)
{
    
}

int  item_touch(Entity *self,Entity *other)
{
    return 0;
}

void item_damage(Entity *self,int amount, Entity *source)
{
    return;
}

void item_die(Entity *self)
{
    
}

int item_file_get_count(FILE *file)
{
    char buf[512];
    int count = 0;
    if (!file)return 0;
    rewind(file);
    while(fscanf(file, "%s", buf) != EOF)
    {
        if(strcmp(buf,"name:") == 0)
        {
            count++;
        }
        fgets(buf, sizeof(buf), file);
    }
    return count;
}

void item_file_load_items(FILE *file,Item *item)
{
    char buf[512];
    if (!file)return;
    rewind(file);
    item--;
    while(fscanf(file, "%s", buf) != EOF)
    {
        if(strcmp(buf,"name:") == 0)
        {
            item++;
            fscanf(file,"%s",(char*)&item->name);
            continue;
        }
        if(strcmp(buf,"sprite:") == 0)
        {
            fscanf(file,"%s",(char*)&item->sprite);
            continue;
        }
        if(strcmp(buf,"spriteHeight:") == 0)
        {
            fscanf(file,"%i",&item->spriteHeight);
            continue;
        }
        if(strcmp(buf,"spriteWidth:") == 0)
        {
            fscanf(file,"%i",&item->spriteWidth);
            continue;
        }
        if(strcmp(buf,"spriteFPL:") == 0)
        {
            fscanf(file,"%i",&item->spriteFPL);
            continue;
        }
        if(strcmp(buf,"actor:") == 0)
        {
            fscanf(file,"%s",(char*)&item->actor);
            continue;
        }
        if(strcmp(buf,"count:") == 0)
        {
            fscanf(file,"%i",&item->count);
            continue;
        }
        if(strcmp(buf,"radius:") == 0)
        {
            fscanf(file,"%f",&item->radius);
            continue;
        }
        fgets(buf, sizeof(buf), file);
    }
}


void item_load_list(char *filename)
{
    FILE *file;
    int count;
    file = fopen(filename,"r");
     if (!file)
     {
         slog("failed to load item file: %s",filename);
         return;
     }
     count = item_file_get_count(file);
     if (!count)
     {
         slog("no items loaded!");
         fclose(file);
         return;
     }
     slog("attempting to allocate %i items",count);
     item_manager.items = (Item*)malloc(sizeof(Item)*count);
    if (!item_manager.items)
    {
        slog("failed to allocate memory for item list");
        fclose(file);
        return;
    }
     memset(item_manager.items,0,sizeof(Item)*count);
     item_file_load_items(file,item_manager.items);
     fclose(file);
}


/*eol@eof*/