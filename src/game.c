#include <SDL.h>
#include <stdio.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gf2d_draw.h"
#include "gf2d_space.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;

    int mx,my,i;
    float mf = 0;
    Sprite *mouse = NULL;
    Vector4D mouseColor = {255,255,100,200};
    Space *space = NULL;
    static Body body[10000];// not a pointer!
    Shape shape[4];// not a pointer!
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
        gf2d_graphics_set_frame_delay(16);
        gf2d_sprite_init(1024);
        SDL_ShowCursor(SDL_DISABLE);

    /*demo setup*/
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);

    space = gf2d_space_new_full(
        3,
        gf2d_rect(0,0,1200,700),
        0.1,
        vector2d(0,0.1),
        1,
        0.1);

    shape[2] = gf2d_shape_circle(0,0, 10);
    shape[3] = gf2d_shape_circle(10,0, 15);
    shape[1] = gf2d_shape_rect(-32,-32,64,64);
    shape[0] = gf2d_shape_rect(-16,-16, 32,32);

    gf2d_space_add_static_shape(space,gf2d_shape_rect(200,500, 512,32));
    gf2d_space_add_static_shape(space,gf2d_shape_rect(600,50, 30,500));
    gf2d_space_add_static_shape(space,gf2d_shape_circle(100,300, 15));
    /* Stress test*/
        gf2d_body_set(
            &body[0],
            "body",
            1,
            0,
            0,
            0,
            vector2d(256,256),
            vector2d(3.1,4.3),
            10,
            1,
            1,  //elasticity
            &shape[0],
            NULL,
            NULL);
        gf2d_space_add_body(space,&body[0]);
    for (i = 1; i < 0;i++)
    {
        gf2d_body_set(
            &body[i],
            "body",
            1,
            0,
            0,
            0,
            vector2d(256 + 128*gf2d_crandom(),256 + 128*gf2d_crandom()),
            vector2d(3 + gf2d_crandom() * 3,3 + gf2d_crandom() *3),
            10,
            1,
            1,  //elasticity
            &shape[i%2],
            NULL,
            NULL);
        gf2d_space_add_body(space,&body[i]);
    }
    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;


        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
        //backgrounds drawn first
//        gf2d_sprite_draw_image(sprite,vector2d(0,0));
        gf2d_space_update(space);

        gf2d_space_draw(space,vector2d(0,0));
        //UI elements last
        gf2d_sprite_draw(
            mouse,
            vector2d(mx,my),
            NULL,
            NULL,
            NULL,
            NULL,
            &mouseColor,
            (int)mf);
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
//        slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }

    gf2d_space_free(space);
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
