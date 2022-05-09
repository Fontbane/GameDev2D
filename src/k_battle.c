#include "k_battle.h"


#include "k_monster.h"
#include "k_technique.h"
#include "k_save.h"
#include "k_local.h"
#include "k_palette.h"



MonDict GetParticipant(u8 who) {
    return *gBattle.participants[who];
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

char* battlefieldbgs[FIELD_MAX] = {
    "images/backgrounds/bf_plains.png"
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

void RenderBattlefield() {
    Sprite* field;
    Sprite *participants[4];
    int i = 0;
    field = gf2d_sprite_load_image(battlefieldbgs[gBattle.fieldtype]);
    for (; i < gBattle.numMons; i++) {
        participants[i] = GetMonsterSprite(*gBattle.participants[i]);
    }
    for (i = 0; i < gBattle.numMons; i++) {
        gf2d_sprite_draw_image(participants[i], vector2d(gBattle.participants[i]->x*64, gBattle.participants[i]->y*64+128));
    }
}

void StartWildBattle(u16 species, u8 level, u8 bg) {
    MonDict* wild = monster_set_dict(monster_new(species, level, 0, Random32()));
    gBattle.fieldsizex = 7;
    gBattle.fieldsizey = 4;
    gBattle.battleType = BATTLE_TYPE_WILD;
    gBattle.fieldtype = bg;
    gBattle.numMons = 2;
    gBattle.turnCount = 0;
    gBattle.participants[0] = party.party;
    gBattle.participants[1] = wild;
    gBattle.participants[0]->x = 3;
    gBattle.participants[0]->y = 2;
    gBattle.participants[1]->x = 5;
    gBattle.participants[1]->y = 2;
}