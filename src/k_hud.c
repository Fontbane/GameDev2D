#include "simple_logger.h"
#include "gf2d_elements.h"
#include "gf2d_element_label.h"

#include "k_hud.h"
#include "k_entity.h"
#include "k_text.h"
#include "k_palette.h"
#include "k_local.h"

void menus_init() {
	partySprites = gfc_allocate_array(sizeof(Sprite*), 6);
}

void msgbox(char* msgID) {
	if (!msgID) return;
	SJson *json, *arr;
	MessageBox msg;
	char* buff;
	int i, c;
	if (strcmp(strchr(msgID, 4), "text")) {
		json = GetTextFromJson(msgID);
		buff = sj_get_string_value(json);
		gfc_line_cpy(msg.lines, buff);
		msg.state = 0;

		sj_free(json);
		free(buff);

		Window* window = gf2d_window_new();
		window->data = &msg;
		window->update = msgbox_update;
		window->draw = msgbox_draw;
		return;
	}
	json=sj_load("msgbox.json");
	if (!json) return;
	json = sj_object_get_value(json, msgID);
	if (buff = sj_get_string_value(GetTextFromJson(sj_get_string_value(sj_object_get_value(json, "title"))))) {
		gfc_word_cpy(&msg.title, buff);
	}
	if (arr=GetTextFromJson(sj_get_string_value(sj_object_get_value(json, "lines")))) {
		if ((msg.numlines=sj_array_get_count(arr)) > 8) {
			slog("Too many lines in %s. Lines after 8 will be cut off.", msgID);
		}
		for (i = min(sj_array_get_count(arr),8), c=0; c<i; c++) {
			buff = sj_get_string_value(sj_array_get_nth(arr, c));
			gfc_line_cpy(msg.lines + c, buff);
		}
	}
	if (buff = sj_get_string_value(sj_object_get_value(json, "sprite"))) {
		msg.sprite = gf2d_sprite_load_image(buff);
	}
	msg.state = 0;

	sj_free(json);
	sj_free(arr);
	free(buff);

	Color white = gfc_color(1, 1, 1, 1);
	
	if (msg.lines) {
		Element* el;
		LabelElement *l;
		el = gf2d_element_new();
		l = gf2d_element_label_new_full(msg.lines[0],white,0,0,0,0);
		gf2d_element_make_label(el, l);
		el->bounds.x = 0;
		el->bounds.y = 128;
		el->bounds.w = 256;
		el->bounds.h = 64;
		el->data = &msg;
		gfc_line_cpy(el->name,"msgbox_line");
		gf2d_window_add_element(game.hud, el);
	}
	if (msg.title) {
		Element* el;
		LabelElement* l;
		el = gf2d_element_new();
		l = gf2d_element_label_new_full(msg.title, white, 0, 0, 0, 0);
		gf2d_element_make_label(el, l);
		el->bounds.x = 0;
		el->bounds.y = 96;
		el->bounds.w = 80;
		el->bounds.h = 32;
		el->data = &msg;
		gfc_line_cpy(el->name, "msgbox_title");
		gf2d_window_add_element(game.hud, el);
	}

};

void msgbox_update(Window* win, List* updateList) {

	
}

void msgbox_draw(Window* win) {

}

void DrawHUD() {
	int i;
	Vector2D drawpos = vector2d(8, 0);
	Vector2D drawscale = vector2d(1, 1);
	for (i = 0; i < 6; i++) {
		u16 species = party.partyPersonal[i].species;
		if (species == 0 || species > SPECIES_MAX) {
			continue;
		}
		if (partySprites[i]->ref_count == 0) {
			partySprites[i] = GetMonsterSprite(party.party[i]);
		}
		gf2d_sprite_draw(partySprites[i], drawpos, &drawscale, NULL, NULL, NULL, NULL, 0);
		drawpos.x += 32;
	}
}