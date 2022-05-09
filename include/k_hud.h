#ifndef __K_HUD_H__
#define	__K_HUD_H__

#include "gf2d_windows.h"
#include "k_global.h"

typedef struct MSGBOX_S {
	TextWord title;
	TextLine lines[8];
	Sprite* sprite;
	u8 state;
	u8 numlines;
}MessageBox;

typedef struct MULTISELECT_S {
	u8 numoptions;
	u8 state;
	TextWord* options;
	TextLine* optionDesc;
	Sprite** optionSprites;
}Multiselect;

Sprite** partySprites;

void menus_init();
void DrawHUD();
void msgbox(char* msgID);
void msgbox_update(Window* win, List* updateList);
void msgbox_draw(Window* win);
void multiselect(char* msgID, char *selection);
int promptkeyboard(char* prompt, TextWord* response);

#endif