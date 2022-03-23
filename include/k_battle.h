#ifndef __K_BATTLE_H__
#define	__K_BATTLE_H__
#include "k_global.h"
#include "k_entity.h"
#include "k_monster.h"

typedef struct {
    u16 id;
    u8 tamerClass;
    MonDict party[6];
} Tamer;

struct AITamer {
    u16 item : 12;
    u16 itemCount : 4;
    u8 aiFlags;
}AITamer;

typedef struct BattleRender {
    Sprite* bg;
    Sprite* participants[4];
    Sprite* tamers[4];
}BattleRender;

typedef struct {
    u8 fieldsizex;
    u8 fieldsizey;
    u8 fieldtype;
    u8 battleType;
    u8 numMons;
    MonDict* participants[4];
    Tamer tamers[4];
    u8 turnCount;
    BattleRender render;
} Battle;

Battle gBattle;

#define BATTLE_TYPE_WILD    0
#define BATTLE_TYPE_TAMER   1
#define BATTLE_TYPE_ARENA   2
#define BATTLE_TYPE_ELITE   3
#define BATTLE_TYPE_RIVAL   4
#define BATTLE_TYPE_RARE    5

enum {
    FIELD_PLAINS,
    FIELD_BUILDING,
    FIELD_MAX
};

char* battlefieldbgs[FIELD_MAX];

typedef struct BattlefieldState {
    u16 flags;
    MonDict* participants;
}BattlefieldState;
typedef struct Action {
    u8 who;
    Vector2D startPos;
    Vector2D movePos;
    u16 tech;
    Vector2D targetPos;
    u8 actionFlag;
}Action;

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

void RenderBattlefield();

#endif // !__K_BATTLE_H__
