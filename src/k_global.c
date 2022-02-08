#include "k_global.h"
#include "k_monster.h"
#include "k_technique.h"



MonDict GetParticipant(u8 who) {
    return gBattle.participants[who];
}

const int gTypeMatchupTable[16][16] = {//Effectiveness multiplier is this number divided by 2. Resistance is 1, SE is 4, immune is 0
    //MYSTERY   BASIC   PLANT   FLAME   AQUA    AIR     ELECTR  TOXIC   BUG     EARTH   METAL   ICE     MIND    BRAWL   GHOST   DRAGON
    { 2,        2,      2,      2,      2,      2,      2,      2,      2,      2,      2,      2,      2,      2,      2,      2   },//MYSTERY
    { 2,        2,      2,      2,      2,      2,      2,      2,      2,      2,      2,      2,      2,      4,      2,      2   },//BASIC
    { 2,        2,      1,      4,      1,      4,      1,      2,      4,      1,      2,      4,      2,      2,      2,      2   },//PLANT
    { 2,        2,      1,      1,      4,      4,      2,      2,      1,      4,      1,      1,      2,      2,      2,      2   },//FLAME
    { 2,        2,      4,      1,      1,      4,      2,      2,      2,      2,      1,      1,      2,      2,      2,      2   },//AQUA
    { 2,        2,      2,      2,      2,      2,      4,      2,      1,      0,      2,      4,      2,      1,      2,      2   },//AIR
    { 2,        2,      2,      2,      2,      1,      1,      2,      2,      4,      1,      2,      2,      2,      2,      2   },//ELECTRIC
    { 2,        2,      1,      2,      2,      2,      2,      1,      2,      4,      2,      2,      4,      1,      2,      2   },//TOXIC
    { 2,        2,      1,      4,      2,      2,      2,      4,      2,      1,      2,      4,      2,      2,      2,      2   },//BUG
    { 2,        2,      4,      2,      4,      2,      0,      1,      2,      2,      1,      4,      2,      2,      2,      2   },//EARTH
    { 2,        1,      1,      4,      2,      1,      2,      0,      1,      4,      1,      1,      1,      4,      2,      1   },//METAL
    { 2,        2,      2,      4,      1,      2,      2,      2,      2,      2,      4,      1,      2,      4,      2,      2   },//ICE
    { 2,        2,      2,      2,      2,      2,      2,      2,      4,      2,      2,      2,      1,      1,      4,      2   },//MIND
    { 2,        2,      2,      2,      2,      2,      2,      4,      1,      2,      1,      2,      4,      2,      2,      2   },//BRAWL
    { 2,        0,      2,      2,      2,      2,      2,      1,      1,      2,      2,      1,      2,      0,      4,      2   },//GHOST
    { 2,        2,      1,      1,      1,      2,      1,      2,      2,      2,      2,      4,      2,      2,      2,      4   }//DRAGON
};

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

void CalculateAttack(MonDict attacker, MonDict target, u16 techID, u16 *atk, u16 *def) {
    Technique tech = gTechniques[techID];
    if (tech.category == CAT_PHYSICAL || tech.effect == TEFFECT_TOP_DOWN) {
        *atk = attacker.attack * ((attacker.atkStage >= 0) ? (2 + attacker.atkStage) / 2 : 2 / (2 + attacker.atkStage));
        if (attacker.status & STATUS_BURN) *atk /= 2;
        *def = target.defense * ((target.defStage >= 0) ? (2 + target.defStage) / 2 : 2 / (2 + target.defStage));
    }
    else if (tech.category == CAT_SPECIAL) {
        *atk = attacker.m_atttack * ((attacker.matkStage >= 0) ? (2 + attacker.matkStage) / 2 : 2 / (2 + attacker.matkStage));
        if (attacker.status & STATUS_FREEZE) *atk /= 2;
        *def = target.m_defense * ((target.mdefStage >= 0) ? (2 + target.mdefStage) / 2 : 2 / (2 + target.mdefStage));
    }
}

u16 DealDamage(MonDict attacker, MonDict target, u16 damage) {
    if (damage >= target.currentHP) {
        damage = target.currentHP;
    }
    target.currentHP -= damage;
    return damage;
}

Vector2D directions[4] = {
    [DIR_S] = {  0,  1  },
    [DIR_W] = { -1,  0  },
    [DIR_N] = {  0, -1  },
    [DIR_E] = {  1,  0  }
};

#define MF_DOWN     0x0001
#define MF_UP       0x0010
#define MF_LEFT     0x0100
#define MF_RIGHT    0x1000