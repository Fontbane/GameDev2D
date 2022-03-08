#include <SDL.h>
#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gfc_input.h"

#include "k_player.h"
#include "k_save.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;

    //save1 = LoadSave("save/save00.sav");
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};

    Sprite* hatkid;
    
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "Keystone",
        960,
        640,
        480,
        320,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    SetUpTilesets();
    NewSave("saves/save1.sav");
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16);
    hatkid = gf2d_sprite_load_all("images/objects/HatKid.png", 32, 32, 4);


    Edict HatKid = {
        .localID = 2,
        .sprite = hatkid,
        .position = {0, 0},
        .draw_scale = {1, 1},
        .inUse=0
    };

    ent_manager_init(256);
    monster_manager_init();

    player_init();
    ent_free(&HatKid);
    boy = ent_new();
    boy->sprite = hatkid;
    boy->position = vector2d(128, 128);
    //PrintLayout(save1.map);
    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        SDL_Event keyEvent;
        SDL_PollEvent(&keyEvent);
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;

        if (keys[SDL_SCANCODE_W]) {
            player_move(player, cell(0,-1));
            player->facing = DIR_N;
        }
        else if (keys[SDL_SCANCODE_S]) {
            player_move(player, cell(0, 1));
            player->facing = DIR_S;
        }
        else if (keys[SDL_SCANCODE_A]) {
            player_move(player, cell(-1, 0));
            player->facing = DIR_W;
        }
        else if (keys[SDL_SCANCODE_D]) {
            player_move(player, cell(1, 0));
            player->facing = DIR_E;
        }
        if (keyEvent.type==SDL_KEYDOWN&&keys[SDL_SCANCODE_M]) {
            if (boy->inUse) {
                slog("Free the boy");
                ent_free(boy);
                boy->inUse = 0;
            }
            else {
                slog("New boy");
                boy = ent_new();
                boy->sprite = hatkid;
                boy->position = vector2d(128, 128);
            }
            
        }

        ent_manager_think_all();
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
        //gf2d_sprite_draw_image(sprite, vector2d(0, 0));
        RenderMap(save1.map);
        

        ent_manager_draw_all();
            
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
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
