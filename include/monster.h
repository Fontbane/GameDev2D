#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "global.h"
#include "species.h"
#include "gfc_vector.h"

typedef struct PersonalDict {//stuff that can change and is used in calculations
    u16 species;
    struct BaseStats* baseStats;
    u8 level : 7;
    u8 gender : 1;
    u32 exp;
    u8 hpTV;
    u8 attackTV;
    u8 defTV;
    u8 magattackTV;
    u8 magdefTV;
    u8 agilityTV;
    u8 hpReserves : 4;
    u8 agiReserves : 4;
    u8 nrgReserves;
    u16 traitPtsA : 4;//Stubborn
    u16 traitPtsB : 4;//Outgoing
    u16 traitPtsC : 4;//Courage
    u16 traitPtsD : 4;//Kind
    u8 smart;
    u8 cool;
    u8 tough;
    u8 cute;
    u8 beauty;
    u8 tastes;
    u8 tameness;
    u8 bond;
    u8 knack;

    u32 personality;

    u8 shiny : 1;
    u8 event:1;
    u8 ability : 1;
    u8 abilityHidden : 1;
    u8 xpShares : 2;
    u8 padding : 2;

    u8 form;
} PersonalDict;

typedef struct MonDict {
    PersonalDict* personal;
    u16 maxHP;
    u16 currentHP;
    u16 attack;
    u16 defense;
    u16 m_atttack;
    u16 m_defense;
    u16 agility;
    Type type1;
    Type type2;

    u8 ability;
    u8 trait;

    u16 techniques[4];

    u16 additionalmemory[8];//move memory, including egg moves

    //Temporary in-battle data
    u16 status;
    s16 atkStage;
    s16 defStage;
    s16 matkStage;
    s16 mdefStage;
    s16 agiStage;

    Vector2D position;

} MonDict;

struct BaseStats {
    u16 species; /*0x00*/
    u8 hp; /*0x02*/
    u8 attack; /*0x03*/
    u8 defense; /*0x04*/
    u8 m_atttack; /*0x05*/
    u8 m_defense; /*0x06*/
    u8 agility; /*0x07*/

    u8 xpYield; /*0x08*/
    u16 hpYield : 2; /*0x09*/
    u16 atkYield : 2;
    u16 defYield : 2;
    u16 matkYield : 2;
    u16 mdefYield : 2;
    u16 agiYield : 2;
    u16 padding : 4;

    u8 genderParam; /*0x0B*/
    //chance of female/248

    u8 ability1; /*0x0C*/
    u8 ability2; /*0x0D*/
    u8 abilityH1; /*0x0E*/
    u8 abilityH2; /*0x0F*/

    u8 type1 : 4;/*0x10*/
    u8 type2 : 4;

    u8 catchrate;/*0x11*/

    u8 color : 4;/*0x12*/
    u8 rarity : 4;

    u16 weight;/*0x13*/
    //in kg, times 10

    u8 family1 : 4;/*0x15*/
    u8 family2 : 4;

    u32 item_common : 10;/*0x16*/   //50%
    u32 item_rare : 10;             //10%
    u32 item_veryrare : 10;         //5%
    u32 noFlip : 2;

    u8 forms;/*0x1A*/
    u8 flags;/*0x1B*/
    u16 statsToInherit;/*0x1C*/
};//Size: 0x1E, 31 bytes

#define GENDER_ALL_MALE 0
#define GENDER_MALE_7_8TH 31
#define GENDER_MALE_3_4TH 62
#define GENDER_HALF 124
#define GENDER_MALE_1_4TH 186
#define GENDER_MALE_1_8TH 217
#define GENDER_ALL_FEMALE 248
#define GENDER_NONE 255

#define YLD_ATK_1 0b0001000000000000

typedef struct levelTech {
    u8 level;
    u16 techID;
} levelTech;

typedef levelTech* Moveset;

enum {
    FAM_MAMMAL,
    FAM_BUG,
    FAM_BIRD,
    FAM_DRAGON,
    FAM_FISH,
    FAM_SEMIAQUATIC,
    FAM_FAIRY,
    FAM_AMORPHOUS,
    FAM_DITTO
};

typedef struct {
    u16 id;
    u8 tamerClass;
    MonDict* party;
} Tamer;

struct AITamer {
    u16 item : 12;
    u16 itemCount : 4;
    u8 aiFlags;
};

typedef struct {
    u8 fieldsize;
    u8 fieldtype;
    u8 battleType;
    MonDict* participants;
    Tamer* tamers;
    u8 turnCount;
} Battle;

Battle gBattle;

#define BATTLE_TYPE_WILD    0
#define BATTLE_TYPE_TAMER   1
#define BATTLE_TYPE_ARENA   2
#define BATTLE_TYPE_ELITE   3
#define BATTLE_TYPE_RIVAL   4
#define BATTLE_TYPE_RARE    5

typedef struct BattlefieldState {
    u16 flags;
    MonDict* participants;
};

typedef struct BattleMon {
    MonDict mon;
    s8 atkStage;
    s8 defStage;
    s8 matkStage;
    s8 mdefStage;
    s8 agiStage;
    u8 nrg;
    u16 currentHP;
    Vector2D pos;
    u8 status;
    u16 flags;
};

typedef struct Action {
    u8 who;
    Vector2D startPos;
    Vector2D movePos;
    u16 tech;
    Vector2D targetPos;
    u8 actionFlag;
};

#define NUM_SPECIES SPECIES_MAX


/**
 * @brief Gets the information of the specified participant in a battle
 * @param who the index of the battle participant
 * @returns dictionary of specified battler
 */
MonDict GetParticipant(u8 who);

u16 CalculateDamage(MonDict attacker, MonDict target, u16 techID, u8 dist);

/**
 * @brief Calculates relevant attack and defense stats needed for damage calculation
 * @param attacker
 * @param target
 * @param techID the technique being used
 * @param atk the output attack stat
 * @param def the output defense stat
 */
void CalculateAttack(MonDict attacker, MonDict target, u16 techID, u16* atk, u16* def);

extern const int gTypeMatchupTable[16][16];

extern const struct BaseStats gBaseStats[NUM_SPECIES];

#endif