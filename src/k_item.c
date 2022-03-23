#include "simple_json.h"

#include "k_save.h"
#include "k_item.h"

void item_use_heal(Item* item) {

}

void item_use_capsule(Item* item) {

}

int RemoveItem(char* name) {
	if (SDL_strcmp(save1.inventory[0].name, name) == 0)
		return 1;
	else return 0;
}

void GiveItem(char* name) {
	save1.inventory[0] = GetItemFromJson(name);
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Item Get", name, NULL);
};

Item GetItemFromJson(char* name) {
	SJson* json;
	Item* i;
	i = (Item*)malloc(sizeof(Item));
	if (i == NULL) return;
	json = sj_object_new();
	json = sj_load("config/items.json");
	gfc_word_cpy(i->name,name);
	json = sj_object_get_value(json, name);
	sj_get_integer_value(sj_object_get_value(json, "id"), &i->id);
	if (sj_object_get_value(json, "useInBattle")) i->useInBattle = true;
	if (sj_object_get_value(json, "useOnField")) i->useOnField = true;
	sj_get_integer_value(sj_object_get_value(json, "price"), &i->price);
	sj_get_integer_value(sj_object_get_value(json, "args"), &i->useargs);
	char* usefunc = sj_get_string_value(sj_object_get_value(json, "usefunc"));

	if (SDL_strcmp(usefunc, "use_heal")==0) i->Use = item_use_heal;
	if (SDL_strcmp(usefunc, "use_capsule") == 0) i->Use = item_use_capsule;
	return *i;
}