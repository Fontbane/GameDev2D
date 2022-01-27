#include "k_monster.h"
#include "k_technique.h"

const Technique gTechniques[NUM_TECHNIQUES] = {
	[TECH_NONE] = {0}

};

const struct BaseStats gBaseStats[NUM_SPECIES] = {
	[SPECIES_SAPPURR] = {
		48, 60, 50, 40, 42, 72,
		.xpYield = 80,
		.atkYield = 1,
		.genderParam = GENDER_MALE_7_8TH,
		.type1 = T_PLANT,
		.type2 = T_MYSTERY,
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