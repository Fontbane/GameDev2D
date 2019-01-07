#include "gf2d_entity.h"
#include "camera.h"
#include "entity_common.h"
#include "level.h"

int entity_camera_view(Entity *self)
{
    Rect r,c;
    if (!self)return 0;
    c = camera_get_dimensions();
    r = gf2d_shape_get_bounds(self->shape);
    vector2d_add(r,r,self->position);
    return gf2d_rect_overlap(r,c);
}


int entity_roof_check(Entity *self, float width)
{
    Rect r;
    Collision c;
    if (!self)return 0;
    r = gf2d_shape_get_bounds(self->shape);
    r.x += 0.1;
    r.w -= 0.2;
    r.y -= (0.1 + width);
    r.h = width;
    vector2d_add(r,r,self->position);
    c = gf2d_space_shape_test(level_get_space(),gf2d_shape_from_rect(r));
    if (c.shape != NULL)
    {
        gf2d_shape_draw(*c.shape,gf2d_color(255,255,0,255),vector2d(0,0));
    }
    return c.collided;
}


int entity_ground_check(Entity *self, float width)
{
    Rect r;
    Collision c;
    if (!self)return 0;
    r = gf2d_shape_get_bounds(self->shape);
    r.x += 0.1;
    r.w -= 0.2;
    r.y += r.h + 0.1;
    r.h = width;
    vector2d_add(r,r,self->position);
    c = gf2d_space_shape_test(level_get_space(),gf2d_shape_from_rect(r));
    if (c.shape != NULL)
    {
        gf2d_shape_draw(*c.shape,gf2d_color(0,255,255,255),vector2d(0,0));
    }
    return c.collided;
}

int entity_left_check(Entity *self, float width)
{
    Rect r;
    Collision c;
    if (!self)return 0;
    r = gf2d_shape_get_bounds(self->shape);
    r.x -= (0.1 + width);
    r.w = width;
    r.y += 0.1;
    r.h -= 0.2;
    vector2d_add(r,r,self->position);
    c = gf2d_space_shape_test(level_get_space(),gf2d_shape_from_rect(r));
    if (c.shape != NULL)
    {
        gf2d_shape_draw(*c.shape,gf2d_color(255,0,255,255),vector2d(0,0));
    }
    return c.collided;
}

int entity_right_check(Entity *self, float width)
{
    Rect r;
    Collision c;
    if (!self)return 0;
    r = gf2d_shape_get_bounds(self->shape);
    r.x += r.w + 0.11;
    r.w = width;
    r.y += 0.1;
    r.h -= 0.2;
    vector2d_add(r,r,self->position);
    c = gf2d_space_shape_test(level_get_space(),gf2d_shape_from_rect(r));
    if (c.shape != NULL)
    {
        gf2d_shape_draw(*c.shape,gf2d_color(255,0,0,255),vector2d(0,0));
    }
    return c.collided;
}

void entity_world_snap(Entity *self)
{
    if (entity_ground_check(self,0.1))
    {
        self->position.y -= 0.1;
    }
    if (entity_roof_check(self,0.1))
    {
        self->position.y += 0.1;
    }
    if (entity_right_check(self,0.1))
    {
        self->position.x -= 0.1;
    }
    if (entity_left_check(self,0.1))
    {
        self->position.x += 0.1;
    }    
}

/*eol@eof*/