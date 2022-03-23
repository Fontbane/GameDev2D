#include "simple_logger.h"
#include "simple_json.h"

#include "k_monster.h"
#include "k_technique.h"
#include "k_save.h"
#include "k_local.h"

typedef struct {
	MonDict *monsters;
	u8		 monsternum;
}MonsterManager;

typedef struct
{
	const char* name;
	const char* spritefile;
	int startinghealth;
}MONSTERDATA;


const MONSTERDATA _monster_data[] =
{
	{0},
	{
		"sappurr",
		"images/battlers/Sappurr/front.png",
		10
	},
	{0}
};

MONSTERDATA* monster_get_by_name(const char* name)
{
	MONSTERDATA* it = NULL;
	for (it = _monster_data; it && it->name != 0; it++)
	{
		if (strcmp(name, it->name) == 0)return it;
	}
	return NULL;
}

u8 GetLevel(PersonalDict pers) {
	return (u8)SDL_sqrt(pers.exp);
}

MonsterManager monsterManager;

const Technique gTechniques[NUM_TECHNIQUES] = {
	[TECH_NONE] = {0}

};

const struct BaseStats gBaseStats[NUM_SPECIES] = {
	{0},
	{
		"Sappurr",
		"images/battlers/Sappurr/front.png",

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
	},{
		"Camofline",
		"images/battlers/Sappurr/front.png",

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
	},{
		"Vampiroot",
		"images/battlers/Sappurr/front.png",

		.hp = 76,
		.attack = 105,
		.defense = 75,
		.m_attack = 80,
		.m_defense = 75,
		.agility = 109,


		.type1 = T_PLANT,
		.type2 = T_MIND,

		.xpYield = 80,
		.xpGrowth = GROWTH_MEDIUM,
		.atkYield = 3,

		.genderParam = GENDER_MALE_7_8TH,
		.catchrate = 45
	},
	{
		"Pyruff",
		"images/battlers/Pyruff/front.png",

		.hp = 50,
		.attack = 62,
		.defense = 50,
		.m_attack = 45,
		.m_defense = 45,
		.agility = 60,

		.type1 = T_FLAME,

		.xpYield = 80,
		.atkYield = 1,
		.genderParam = GENDER_MALE_7_8TH,
		.catchrate = 45
	},
	{
		"Flarewolf",
		"images/battlers/Pyruff/front.png",

		.hp = 50,
		.attack = 62,
		.defense = 50,
		.m_attack = 45,
		.m_defense = 45,
		.agility = 60,

		.type1 = T_FLAME,

		.xpYield = 80,
		.atkYield = 1,
		.genderParam = GENDER_MALE_7_8TH,
		.catchrate = 45
	},
	{
		"Lycarson",
		"images/battlers/Pyruff/front.png",

		.hp = 83,
		.attack = 102,
		.defense = 80,
		.m_attack = 94,
		.m_defense = 80,
		.agility = 95,

		.type1 = T_FLAME,

		.xpYield = 80,
		.atkYield = 1,
		.genderParam = GENDER_MALE_7_8TH,
		.catchrate = 45
	},
	{
		"Squoink",
		"images/battlers/Squoink/front.png",
		58,
		50,
		50,
		58,
		52,
		44,

		T_AQUA
	},
	{
		"Woghash",
		"images/battlers/Squoink/front.png",
		58,
		50,
		50,
		58,
		52,
		44,

		T_AQUA
	},
	{
		"Soluboar",
		"images/battlers/Squoink/front.png",
		100,
		80,
		80,
		100,
		100,
		70,

		T_AQUA,
		T_TOXIC
	},
	{
		"Egglet",
		"images/battlers/Egglet/front.png",
		50,
		54,
		50,
		50,
		50,
		58,

		T_AIR
	},
	{
		"Reagle",
		"images/battlers/Egglet/front.png",
		50,
		54,
		50,
		50,
		50,
		58,

		T_AIR
	},
	{
		"Reagalia",
		"images/battlers/Egglet/front.png",
		80,
		110,
		80,
		80,
		80,
		100,

		T_AIR
	},
	{
		"Mukchuk",
		"images/battlers/Egglet/front.png",
		40,
		50,
		35,
		20,
		25,
		55,

		T_BASIC
	},
	{0}
};

const char* gBattlerSprites[NUM_SPECIES] = {
	[SPECIES_SAPPURR] = "images/battlers/Sappurr/front.png",
	[SPECIES_CAMOFLINE] = "images/battlers/Sappurr/front.png",
	[SPECIES_VAMPIROOT] = "images/battlers/Sappurr/front.png",
	[SPECIES_PYRUFF] = "images/battlers/Sappurr/front.png",
	[SPECIES_FLAREWOLF] = "images/battlers/Sappurr/front.png",
	[SPECIES_LYCARSON] = "images/battlers/Sappurr/front.png",
	[SPECIES_SQUOINK] = "images/battlers/Squoink/front.png"
};

const char* GetBattleSprite(u16 species) {
	return gBattlerSprites[species];
}


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
	return 0;
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
	p->exp = level * level;
	return p;
}

void mondict_free(MonDict* mon) {
	free(mon);
	slog("%d monsters", --monsterManager.monsternum);
}

void GiveDemoParty() {
	party.partyPersonal[0] = *monster_new(SPECIES_SQUOINK, 5, 0, Random32());
	int i;
	for (i = 0; i < 1; i++) {
		party.party[i] = *monster_set_dict(party.partyPersonal + i);
	}
}