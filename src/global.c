#include "global.h"
#include "monster.h"
#include "technique.h"



MonDict GetParticipant(u8 who) {
    return gBattle.participants[who];
}

const int gTypeMatchupTable[16][16] = {//Effectiveness multiplier is this number divided by 2. Resistance is 1, SE is 4, immune is 0
    //MYSTERY   BASIC   PLANT   FLAME   AQUA    AIR     ELECTRIC    TOXIC   BUG     EARTH   METAL   ICE     MIND    BRAWL   GHOST   DRAGON
    { 2,        2,      2,      2,      2,      2,      2,          2,      2,      2,      2,      2,      2,      2,      2,      2   },//MYSTERY
    { 2,        2,      2,      2,      2,      2,      2,          2,      2,      2,      2,      2,      2,      4,      2,      2   },//BASIC
    { 2,        2,      1,      4,      1,      4,      1,          2,      4,      1,      2,      4,      2,      2,      2,      2   },//PLANT
    { 2,        2,      1,      1,      4,      4,      2,          2,      1,      4,      1,      1,      2,      2,      2,      2   },//FLAME
    { 2,        2,      4,      1,      1,      4,      2,          2,      2,      2,      1,      1,      2,      2,      2,      2   },//AQUA
    { 2,        2,      2,      2,      2,      2,      4,          2,      1,      0,      2,      4,      2,      1,      2,      2   },//AIR
    { 2,        2,      2,      2,      2,      1,      1,          2,      2,      4,      1,      2,      2,      2,      2,      2   },//ELECTRIC
    { 2,        2,      1,      2,      2,      2,      2,          1,      2,      4,      2,      2,      4,      1,      2,      2   },//TOXIC
    { 2,        2,      1,      4,      2,      2,      2,          4,      2,      1,      2,      4,      2,      2,      2,      2   },//BUG
    { 2,        2,      4,      2,      4,      2,      0,          1,      2,      2,      1,      4,      2,      2,      2,      2   },//EARTH
    { 2,        1,      1,      4,      2,      1,      2,          0,      1,      4,      1,      1,      1,      4,      2,      1   },//METAL
    { 2,        2,      2,      4,      1,      2,      2,          2,      2,      2,      4,      1,      2,      4,      2,      2   },//ICE
    { 2,        2,      2,      2,      2,      2,      2,          2,      4,      2,      2,      2,      1,      1,      4,      2   },//MIND
    { 2,        2,      2,      2,      2,      2,      2,          4,      1,      2,      1,      2,      4,      2,      2,      2   },//BRAWL
    { 2,        0,      2,      2,      2,      2,      2,          1,      1,      2,      2,      1,      2,      0,      4,      2   },//GHOST
    { 2,        2,      1,      1,      1,      2,      1,          2,      2,      2,      2,      4,      2,      2,      2,      4   }//DRAGON
};


/*const TypeInfo gTypeMatchups[T_DRAGON]{
    {
        TMASK_MYSTERY,  //associated type mask
        0,              //weakness flags
        0,              //resistance flags
        0               //immunity flags
    },{
        TMASK_BASIC,
        TMASK_BRAWL,
        0,
        0
    },{
        TMASK_PLANT,
        TMASK_FLAME + TMASK_AIR + TMASK_BUG + TMASK_ICE,
        TMASK_PLANT + TMASK_AQUA + TMASK_ELECTRIC + TMASK_EARTH,
        0
    },{
        TMASK_FLAME,
        TMASK_AQUA + TMASK_AIR + TMASK_EARTH,
        TMASK_PLANT + TMASK_FLAME + TMASK_METAL + TMASK_ICE,
        0
    },{
        TMASK_AQUA,
        TMASK_PLANT + TMASK_AIR + TMASK_ELECTRIC,
        TMASK_FLAME + TMASK_AQUA + TMASK_ICE + TMASK_METAL,
        0
    },{
        TMASK_AIR,
        TMASK_ELECTRIC + TMASK_ICE,
        TMASK_BUG + TMASK_BRAWL,
        TMASK_EARTH
    },{
        TMASK_ELECTRIC,
        TMASK_EARTH,
        TMASK_METAL + TMASK_AIR + TMASK_ELECTRIC,
        0
    },{
        TMASK_TOXIC,
        TMASK_MIND,
        TMASK_PLANT + TMASK_TOXIC + TMASK_BRAWL,
        0
    },{
        TMASK_BUG,
        TMASK_FLAME + TMASK_ICE + TMASK_TOXIC,
        TMASK_EARTH + TMASK_PLANT,
        0
    },{
        TMASK_EARTH,
        TMASK_AQUA + TMASK_PLANT + TMASK_ICE,
        TMASK_TOXIC + TMASK_METAL,
        TMASK_ELECTRIC
    }
    ,{
        TMASK_METAL,
        TMASK_FLAME + TMASK_EARTH + TMASK_BRAWL,
        TMASK_PLANT + TMASK_AIR + TMASK_BUG + TMASK_ICE + TMASK_MIND + TMASK_DRAGON,
        TMASK_TOXIC
    }
    ,{
        TMASK_ICE,
        TMASK_FLAME + TMASK_BRAWL + TMASK_METAL,
        TMASK_ICE + TMASK_AQUA,
        0
    }
    ,{
        TMASK_MIND,
        TMASK_BUG + TMASK_GHOST,
        TMASK_BRAWL + TMASK_MIND,
        0
    }
    ,{
        TMASK_BRAWL,
        TMASK_MIND + TMASK_TOXIC,
        TMASK_BUG + TMASK_METAL,
        0
    }
    ,{
        TMASK_GHOST,
        TMASK_GHOST,
        TMASK_TOXIC + TMASK_BUG + TMASK_ICE,
        TMASK_BASIC + TMASK_BRAWL
    }
    ,{
        TMASK_DRAGON,
        TMASK_DRAGON + TMASK_ICE,
        TMASK_PLANT + TMASK_FLAME + TMASK_AQUA + TMASK_ELECTRIC,
        0
    }
}*/

u16 CalculateDamage(MonDict attacker, MonDict target, u16 techID, u8 dist) {
    u16 dmg;
    u16 adjAtk, adjDef;
    CalculateAttack(attacker, target, techID, &adjAtk, &adjDef);
    Technique tech = gTechniques[techID];
    dmg = adjAtk * (tech.power / dist) / adjDef;
    if (attacker.type1 == tech.type || attacker.type2 == tech.type) dmg = dmg * 3 / 2;//Apply Same Type Attack Bonus
    dmg = dmg * gTypeMatchupTable[tech.type][target.type1] * gTypeMatchupTable[tech.type][target.type2] / 4;

    return dmg;
}

void CalculateAttack(MonDict attacker, MonDict target, u16 techID, u16* atk, u16* def) {
    Technique tech = gTechniques[techID];
    if (tech.category == CAT_PHYSICAL || tech.effect == TEFFECT_TOP_DOWN) {
        atk = attacker.attack * ((attacker.atkStage >= 0) ? (2 + attacker.atkStage) / 2 : 2 / (2 + attacker.atkStage));
        def = target.defense * ((target.defStage >= 0) ? (2 + target.defStage) / 2 : 2 / (2 + target.defStage));
    }
    else if (tech.category == CAT_SPECIAL) {
        atk = attacker.m_atttack * ((attacker.matkStage >= 0) ? (2 + attacker.matkStage) / 2 : 2 / (2 + attacker.matkStage));
        def = target.m_defense * ((target.mdefStage >= 0) ? (2 + target.mdefStage) / 2 : 2 / (2 + target.mdefStage));
    }
}

u16 DealDamage(MonDict attacker, MonDict target, u16 damage) {
    if (damage >= target.currentHP) target.currentHP = 0;
    else target.currentHP -= damage;
}

#define MF_DOWN     0x0001
#define MF_UP       0x0010
#define MF_LEFT     0x0100
#define MF_RIGHT    0x1000