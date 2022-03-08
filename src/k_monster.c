#include "simple_logger.h"
#include "simple_json.h"
#include "k_monster.h"
#include "k_technique.h"

typedef struct {
	MonDict *monsters;
	u8		 monsternum;
}MonsterManager;

MonsterManager monsterManager;

const Technique gTechniques[NUM_TECHNIQUES] = {
	[TECH_NONE] = {0}

};

const struct BaseStats gBaseStats[NUM_SPECIES] = {
	[SPECIES_SAPPURR] = {
		.hp = 48,
		.attack = 60,
		.defense = 50,
		.m_attack = 40,
		.m_defense = 42,
		.agility = 72,


		.type1 = T_PLANT,

		.xpYield = 80,
		.xpGrowth = GROWTH_MEDIUM,
		.atkYield = 1,

		.genderParam = GENDER_MALE_7_8TH,
		.catchrate = 45
	},
	[SPECIES_PYRUFF] = {
		50, 62, 50, 45, 45, 60,
		.xpYield = 80,
		.atkYield = 1,
		.genderParam = GENDER_MALE_7_8TH,
		.type1 = T_FLAME,
		.type2 = T_MYSTERY,
		.catchrate = 45
	}
};


u16 CalculateStat(PersonalDict pers, Stat stat) {
	switch (stat) {
	case STAT_HP:
		return (gBaseStats[pers.species].hp + pers.hpTV + GetLevel(pers))/50 + GetLevel(pers) - 5 * pers.hpReserves;
	case STAT_ATTACK:
		return (gBaseStats[pers.species].attack + pers.attackTV + GetLevel(pers)) / 50+5;
	case STAT_DEFENSE:
		return (gBaseStats[pers.species].defense + pers.defTV + GetLevel(pers)) / 50+5;
	case STAT_MAGIC_ATTACK:
		return (gBaseStats[pers.species].m_attack + pers.magattackTV + GetLevel(pers)) / 50 + 5;
	case STAT_MAGIC_DEFENSE:
		return (gBaseStats[pers.species].m_defense + pers.magdefTV + GetLevel(pers)) / 50 + 5;
	case STAT_AGILITY:
		return (gBaseStats[pers.species].agility + pers.agilityTV + GetLevel(pers)) / 50 + 5 - 5 * pers.agiReserves;
	}
}

void monster_manager_init() {
	MonsterManager* mon = (MonsterManager*)malloc(sizeof(MonsterManager));
	if (!mon) {
		slog("Not enough memory to allocate for monster manager");
		return;
	}
	mon->monsters = gfc_allocate_array(sizeof(MonDict), 32);
	mon->monsternum = 0;
	monsterManager = *mon;
}

MonDict* monster_set_dict(PersonalDict* pers) {
	MonDict mon;
	mon.id = monsterManager.monsternum;
	monsterManager.monsters[mon.id] = mon;
	mon.species = pers->species;
	mon.ability = pers->ability?gBaseStats[mon.species].ability1:gBaseStats[mon.species].ability2;
	mon.form = pers->form;
	mon.nrg = pers->hpReserves + pers->agiReserves;
	mon.knack = pers->personality % 16;
	mon.shiny = pers->shiny;
	mon.maxHP = CalculateStat(*pers, STAT_HP);
	mon.currentHP = mon.maxHP;
	mon.attack = CalculateStat(*pers, STAT_ATTACK);
	mon.defense = CalculateStat(*pers, STAT_DEFENSE);
	mon.m_atttack = CalculateStat(*pers, STAT_MAGIC_ATTACK);
	mon.m_defense = CalculateStat(*pers, STAT_MAGIC_DEFENSE);
	mon.agility = CalculateStat(*pers, STAT_AGILITY);
	SDL_memcpy(mon.techniques, pers->techniques, 8);
	mon.type1 = gBaseStats[mon.species].type1;
	mon.type2 = gBaseStats[mon.species].type2;
	monsterManager.monsternum++;
	return monsterManager.monsters+mon.id;
}

PersonalDict* monster_new(u16 species, u8 level, u8 form, u32 personality) {
	PersonalDict* p;
	p = (PersonalDict*)malloc(sizeof(PersonalDict));
	if (!p) {
		slog("Failed to allocate memory for new mon");
		return NULL;
	}
	SDL_memset(p, 0, sizeof(PersonalDict));
	p->species = species;
	p->form = form;
	p->personality = personality;
}

void mondict_free(MonDict* mon) {
	free(mon);
	slog("%d monsters", --monsterManager.monsternum);
}