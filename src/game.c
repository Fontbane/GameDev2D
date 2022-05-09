#include <SDL.h>
#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gfc_input.h"
#include "gfc_audio.h"
#include "gf2d_windows.h"
#include "gf2d_font.h"

#include "k_player.h"
#include "k_save.h"
#include "k_menu.h"
#include "k_local.h"
#include "k_battle.h"
#include "k_hud.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    int start = 0;
    const Uint8 * keys;
    Sprite *sprite;

    //save1 = LoadSave("save/save00.sav");
    
    //int mx,my;
    //float mf = 0;
    //Sprite *mouse;
    //Vector4D mouseColor = {255,100,255,200};
    
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "Keystone",
        512,
        384,
        256,
        192,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);

    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    //gf2d_font_init("config/font.cfg");


    

    SetUpTilesets();
    ent_manager_init(256);
    NewSave("saves/save1.json");
    /*demo setup*/
    //sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    //mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16);

    monster_manager_init();

    gf2d_windows_init(16);

    game.hud=gf2d_window_new();

    player_init();

    game_init();

    menus_init();
    while (!start) {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        SDL_Event keyEvent;
        SDL_PollEvent(&keyEvent);

        gf2d_graphics_clear_screen();

        gf2d_sprite_draw_image(sprite, vector2d(0, 0));

        if (keys[SDL_SCANCODE_SPACE]) break;
        gf2d_grahics_next_frame();
        //gf2d_font_draw_line_named("Press Space to Start", "AmaticSC-Regular.ttf", gfc_color(0, 0, 0, 1), vector2d(32, 160));
    }
    gfc_audio_init(8, 8, 8, 8, 0, 0);
    gfc_sound_play(gfc_sound_load("music/TwinleafTown.wav", 1.0, 1),-1,1.0,1,1);
    //GiveDemoParty();
    
    
    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        SDL_Event keyEvent;
        SDL_PollEvent(&keyEvent);
        //gf2d_windows_update_all();
        /*update things here*/
        /*SDL_GetMouseState(&mx, &my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        */
            switch (game.state) {
            case GAMESTATE_FIELD:
                if (player->moving == 0&&!game.lockall) {//ignore input if player is already moving
                    if (keys[SDL_SCANCODE_W]) {
                        player_move(player, cell(0, -1));
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
                }

                ent_manager_think_all();

                gf2d_graphics_clear_screen();// clears drawing buffers
                // all drawing should happen betweem clear_screen and next_frame
                    //backgrounds drawn first
                //gf2d_sprite_draw_image(sprite, vector2d(0, 0));
                RenderMap(save1.map);

                ent_manager_draw_all();
            
                RenderMapLayer2(save1.map);

                DrawHUD();
                break;
            case GAMESTATE_BATTLE:
                gf2d_graphics_clear_screen();// clears drawing buffers
                // all drawing should happen betweem clear_screen and next_frame
                    //backgrounds drawn first
                //gf2d_sprite_draw_image(sprite, vector2d(0, 0));
            
                RenderBattlefield();
                break;
            case GAMESTATE_MENU:
                break;
        }
            
            //UI elements last
            /*gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);*/
            gf2d_windows_draw_all();
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
       
        if (keys[SDL_SCANCODE_ESCAPE]) game.state = GAMESTATE_MENU; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
