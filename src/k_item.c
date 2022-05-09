#include "simple_json.h"
#include "simple_logger.h"

#include "k_save.h"
#include "k_item.h"
#include "k_hud.h"

TextLine gItemInternal[ITEM_MAX] = {
	"None",
	"restorade",
	"restorade plus",
	"restorade gold",
	"milk",
	"protein powder",
	"calcium tablet",
	"mystic powder",
	"zinc",
	"sugar packet",
	"capsule",
	"nugget",
	"pearl"
};

void item_use_heal(Item* item) {

}

void item_use_capsule(Item* item) {

}

int RemoveItem(u16 itemID) {
	if (save1.inventory[itemID]>0)
		return save1.inventory[itemID]--;
	else return 0;
}

void GiveItem(u16 itemID) {
	save1.inventory[itemID]++;
	msgbox("text_itemget");
	msgbox(GetItemFromJson(itemID).name);
};

Item GetItemFromJson(u16 itemID) {
	SJson* json;
	Item* i;
	i = (Item*)malloc(sizeof(Item));
	if (i == NULL) return (Item){ 0 };
	json = sj_object_new();
	json = sj_load("config/items.json");
	json = sj_object_get_value(json, gItemInternal[itemID]);
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

u16 ItemIDFromJson(char* name) {
	SJson* json;
	slog("Getting id for %s", name);
	int id=0;
	json = sj_object_new();
	json = sj_load("config/items.json");
	sj_get_integer_value(sj_object_get_value(json, "name"), &id);
	return (u16)id;
}