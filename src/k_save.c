#include <stdio.h>

#include "simple_json.h"
#include "simple_logger.h"
#include "k_save.h"

SaveBlock1 *LoadSave(char* filename) {
	SJson *json;
	time_t time;
	char *map, *timec;

	if (!filename) return NULL;
	json = sj_load(filename);
	if (!json) {
		NewSave(filename);
		return &save1;
	}

	timec = sj_get_string_value(sj_object_get_value(json,"time"));
	time = SDL_strtoll(timec, timec+sizeof(timec), 10);
	map = sj_get_string_value(sj_object_get_value(json, "map"));
	gfc_word_cpy(save1.client.name, sj_get_string_value(sj_object_get_value(json, "player_name")));
	sj_get_integer_value(sj_object_get_value(json, "player_id"), &save1.client.id);
	sj_get_bool_value(sj_object_get_value(json, "gender"), &save1.client.id+2);
	sj_get_integer_value(sj_object_get_value(json, "player_x"), &save1.position.x);
	sj_get_integer_value(sj_object_get_value(json, "player_y"), &save1.position.y);
	save1.map = LoadMap(map);


	sj_free(json);
	return &save1;
}

void NewSave(char* filename) {
	SJson* json = sj_object_new();
	save1.playTime = 0;
	save1.client.id= (int)(65535*gfc_random());
	gfc_word_cpy(save1.client.name, "Ben");//Placeholder
	save1.client.gender = 0;//Placeholder
	save1.position = cell(0,0);
	slog("Initializing save");
	save1.map = LoadMap("maps/testfield.json");
	if (save1.map == NULL) {
		slog("Map not loaded");
	}
	else {
		slog(save1.map->name);
		slog(save1.map->tileset.filename);
	}

	party.party = gfc_allocate_array(sizeof(MonDict),6);
	party.partyPersonal = gfc_allocate_array(sizeof(PersonalDict), 6);

	sj_object_insert(json, "time", sj_new_str("0"));
	sj_object_insert(json, "map", sj_new_str("maps/testfield.json"));
	sj_object_insert(json, "player_name", sj_new_str(save1.client.name));
	sj_object_insert(json, "player_id", sj_new_int(save1.client.id));
	sj_object_insert(json, "gender", sj_new_bool(save1.client.gender));
	sj_object_insert(json, "player_x", sj_new_int(0));
	sj_object_insert(json, "player_y", sj_new_int(0));
	sj_save(json, filename);
	sj_free(json);
}

void WriteSave(char* filename) {
	SJson* json;
	SJString* str=NULL;
	char buff[sizeof(time_t) * 8 + 1];

	if (!filename) return;
	json = sj_load(filename);
	if (!json) return;

	/*sj_string_set(str, SDL_lltoa(save1.playTime,buff,10));
	sj_object_insert(json, "time", sj_string_to_value(str));
	sj_string_set(str, save1.map->name);
	sj_object_insert(json, "map", sj_string_to_value(str));
	sj_string_set(str, save1.client.name);
	sj_object_insert(json, "player_name", sj_string_to_value(str));
	sj_object_insert(json, "player_id", sj_new_int(save1.client.id));
	sj_object_insert(json, "gender", sj_new_bool(save1.client.gender));
	save1.position = player->cellPos;
	sj_object_insert(json, "player_x", sj_new_int(save1.position.x));
	sj_object_insert(json, "player_y", sj_new_int(save1.position.y));

	sj_save(json, filename);

	sj_string_free(str);*/
	sj_free(json);
}