#include "global.h"
#include "monster.h"
#include "technique.h"

const Technique gTechniques[NUM_TECHNIQUES] = {
	[TECH_NONE] = {0}

};

const struct BaseStats gBaseStats[NUM_SPECIES] = {
	[SPECIES_SAPPURR] = {
		48, 60, 50, 40, 42, 72,
		80,
		YLD_ATK_1,
		GENDER_MALE_7_8TH,
		0,0,0,0,
		T_PLANT, T_MYSTERY,
		45
	},
	[SPECIES_PYRUFF] = {
		50, 62, 50, 45, 45, 60,
		80,
		YLD_ATK_1,
		GENDER_MALE_7_8TH,
		0,0,0,0,
		T_FLAME, T_MYSTERY,
		45
	}
};