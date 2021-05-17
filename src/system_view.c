#include <simple_logger.h>

#include "gf2d_mouse.h"
#include "gf2d_draw.h"

#include "camera.h"
#include "system_view.h"

typedef struct
{
    System *system;
}SystemWindowData;


int system_view_draw(Window *win)
{
    Planet *planet;
    Vector2D mouseposition;
    SystemWindowData *data;
    if (!win)return 0;
    if (!win->data)return 0;
    data = (SystemWindowData*)win->data;
    system_draw_system_background(data->system);
    system_draw_system_lines(data->system,camera_get_offset());
    system_draw_system_view(data->system,camera_get_offset());
    
    
    mouseposition = camera_get_mouse_position();
    planet = system_get_nearest_planet(data->system,NULL,mouseposition,100);
    if (planet)
    {
        gf2d_draw_circle(camera_position_to_screen(planet->systemPosition), (int)(planet->drawSize * 0.5), vector4d(100,255,255,255));
    }

    return 1;
}

int system_view_free(Window *win)
{
    SystemWindowData *data;
    if (!win)return 0;
    if (!win->data)return 0;
    data = (SystemWindowData*)win->data;
    free(data);
    return 0;
}

int system_view_update(Window *win,List *updateList)
{
    SystemWindowData *data;
    if (!win)return 0;
    if (!win->data)return 0;
    data = (SystemWindowData*)win->data;
    camera_update_by_keys();
    return 0;
}

Window *system_view_window(System *system)
{
    Window *win;
    SystemWindowData *data;
    win = gf2d_window_load("menus/system_view.json");
    if (!win)
    {
        slog("failed to load system view window");
        return NULL;
    }
    win->no_draw_generic = 1;
    win->draw = system_view_draw;
    win->update = system_view_update;
    win->free_data = system_view_free;
    data = gfc_allocate_array(sizeof(SystemWindowData),1);
    data->system = system;
    win->data = data;
    return win;

}


/*file's end*/