#ifndef __K_MONSTER_H__
#define __K_MONSTER_H__

#include "gfc_vector.h"
#include "k_global.h"
#include "k_species.h"

typedef struct PersonalDict {//stuff that can change and is used in calculations
    u16 species;    /*0x00*/
    u16 traitPtsA : 4;//Stubborn
    u16 traitPtsB : 4;//Outgoing
    u16 traitPtsC : 4;//Courage
    u16 traitPtsD : 4;//Kind

    u32 exp;        /*0x04*/
    u32 hpTV : 4;        /*0x08*/
    u32 attackTV : 4;    
    u32 defTV : 4;
    u32 magattackTV : 4;
    u32 magdefTV : 4;
    u32 agilityTV : 4;
    u32 hpReserves : 4;
    u32 agiReserves : 4;

    u8 smart; /*0x0C*/
    u8 cool; /*0x0D*/
    u8 cute; /*0x0E*/
    u8 beauty; /*0x0F*/

    u32 personality; /*0x10*/

    u8 tameness; /*0x14*/
    u8 bond; /*0x15*/
    u8 form; /*0x16*/
    u8 shiny : 1; /*0x17*/
    u8 event : 1;
    u8 ability : 1;
    u8 abilityHidden : 1;
    u8 xpShares : 2;
    u8 padding : 2;

    u16 techniques[4];/*0x18 TODO:Separate for saving*/
    u32 unlockedTechs;/*A bitfield with flags to tell which of the 32 techniques in the learnset are unlocked*/
} PersonalDict;//Size: 0x20, 32 bytes


typedef struct MonDict {
    u16 species;    /*0x00*/
    u8 form;        /*0x02*/
    u8 level : 7;   /*0x03*/
    u8 gender : 1;

    u16 maxHP;      /*0x04*/
    u16 currentHP;  /*0x06*/

    u16 attack;     /*0x08*/
    u16 defense;    /*0x0A*/

    u16 m_atttack;  /*0x0C*/
    u16 m_defense;  /*0x0E*/

    u16 agility;    /*0x10*/
    Type type1;     /*0x12*/
    Type type2;     /*0x13*/

    u8 ability;     /*0x14*/
    u8 trait;       /*0x15*/
    u8 knack;       /*0x16*/
    u8 shiny:1;       /*0x17*/
    u8 id;

    u16 techniques[4];/*0x18*/


    //Temporary in-battle data
    u16 status;     /*0x20*/
    s8 atkStage;   /*0x22*/
    s8 defStage;   /*0x23*/

    s8 matkStage;  /*0x24*/
    s8 mdefStage;   /*0x25*/
    s8 agiStage;    /*0x26*/
    u8 nrg;         /*0x27*/

    u8 x;           /*0x28*/
    u8 y;           /*0x29*/
    u16 heldItem;

} MonDict;//Size: 0x2A 44 bytes

struct BaseStats {
    u8 hp; /*0x00*/
    u8 attack; /*0x01*/
    u8 defense; /*0x02*/
    u8 m_attack; /*0x03*/
    u8 m_defense; /*0x04*/
    u8 agility; /*0x05*/
    u8 type1;/*0x06*/
    u8 type2;/*0x07*/

    u16 xpYield; /*0x08*/
    u16 hpYield : 2; /*0x0A*/
    u16 atkYield : 2;
    u16 defYield : 2;
    u16 matkYield : 2;
    u16 mdefYield : 2;
    u16 agiYield : 2;
    u16 xpGrowth : 4;

    u8 ability1; /*0x0C*/
    u8 ability2; /*0x0D*/
    u8 abilityH1; /*0x0E*/
    u8 abilityH2; /*0x0F*/

    u16 weight;/*0x10*/
    //in decagrams
    u16 height;/*0x12*/
    //in decimeters

    u8 catchrate;/*0x14*/
    u8 color : 4;/*0x15*/
    u8 rarity : 4;
    u8 family1 : 4;/*0x16*/
    u8 family2 : 4;
    u8 genderParam; /*0x17*/
    //chance of female/248


    u32 item_common : 10;/*0x18*/   //50%
    u32 item_rare : 10;             //10%
    u32 item_veryrare : 10;         //5%
    u32 noFlip : 2;

    u8 forms;/*0x1C*/
    u8 flags;/*0x1D*/
    u16 statsToInherit;/*0x1E*/
};//Size: 0x20, 32 bytes

#define GROWTH_SLOW         0
#define GROWTH_MEDIUM       1
#define GROWTH_FAST         2

#define GENDER_ALL_MALE     0
#define GENDER_MALE_7_8TH   31
#define GENDER_MALE_3_4TH   62
#define GENDER_HALF         124
#define GENDER_MALE_1_4TH   186
#define GENDER_MALE_1_8TH   217
#define GENDER_ALL_FEMALE   248
#define GENDER_NONE         255

typedef struct levelTech {
    u8 level;
    u16 techID;
} levelTech;

typedef levelTech* Moveset;

#define STATUS_NORMAL   1
#define STATUS_STUN     2
#define STATUS_BURN     3
#define STATUS_FREEZE   4
#define STATUS_POISON   5
#define STATUS_DIZZY    6
#define STATUS_DROWSY   7
#define STATUS_GROUNDED 1<<4
#define STATUS_LUCKY    1<<5
#define STATUS_ROOTED   1<<6

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

typedef enum Stat {
    STAT_HP,
    STAT_ATTACK,
    STAT_DEFENSE,
    STAT_MAGIC_ATTACK,
    STAT_MAGIC_DEFENSE,
    STAT_AGILITY,
    STAT_NRG_START
} Stat;


#define NUM_SPECIES SPECIES_MAX

extern const u32 LvToXP_Medium[101];

extern const int gTypeMatchupTable[16][16];

extern const struct BaseStats *gBaseStats;

extern const char* gBattlerSprites[NUM_SPECIES];

const char* gMonsterNames[NUM_SPECIES];

void ExtractBaseStats(char* filename);

u16 CalculateStat(PersonalDict pers, Stat stat);
u8 GetLevel(PersonalDict pers);

PersonalDict* monster_new(u16 species, u8 level, u8 form, u32 personality);
MonDict* mondict_new(u16 species, u8 level, u8 form, u8 flags, u8 ability, u16 techniques[4]);
MonDict* monster_set_dict(PersonalDict* pers);
PersonalDict* MonsterRead(char* filename);
PersonalDict* MonsterSave(char* filename);
void monster_manager_init();

#endif