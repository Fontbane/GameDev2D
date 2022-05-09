#include "k_text.h"

SJson* GetTextFromJson(char* textID) {
	SJson* json, *str;
	json = sj_load("config/texts.json");
	if (!json) return NULL;
	if (str = sj_object_get_value(json, save1.language)) {
		return str;
	}
	str = sj_object_get_value(json, "english");//If string not found in chosen language, default to English
	return str;
}