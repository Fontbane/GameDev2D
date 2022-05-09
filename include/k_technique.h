#ifndef __TECHNIQUE_H__
#define __TECHNIQUE_H__

#include "k_global.h"

typedef enum {
    CAT_PHYSICAL,
    CAT_SPECIAL,
    CAT_STATUS
} Category;

typedef struct Technique {
    char* id;
    u8 range : 4;
    u8 radius : 4;
    Type type;
    Category category;
    u8 power;
    s8 nrgcost;
    u8 effect;
    u8 effectchance;
    u8 args;
    u16 flags;
} Technique;

#define TFLAG_CONTACT       0x0001
#define TFLAG_PROTECTABLE   0x0002
#define TFLAG_SOUND         0x0004
#define TFLAG_PROJECTILE    0x0008
#define TFLAG_CONE          0x0010
#define TFLAG_CIRCLE        0x0020

#define TMASK_MYSTERY   1
#define TMASK_BASIC     2
#define TMASK_PLANT     4
#define TMASK_FLAME     8
#define TMASK_AQUA      16
#define TMASK_AIR       32
#define TMASK_ELECTRIC  64
#define TMASK_TOXIC     128
#define TMASK_BUG       256
#define TMASK_EARTH     512
#define TMASK_METAL     1024
#define TMASK_ICE       2048
#define TMASK_MIND      4096
#define TMASK_BRAWL     8192
#define TMASK_GHOST     16384
#define TMASK_DRAGON    32768

enum {
    TECH_NONE,
    TECH_SCRATCH,
    TECH_SLAP,
    TECH_POUND,
    TECH_BOLT_KICK,
    TECH_BLAZE_KICK,
    TECH_JUMP_KICK,
    TECH_SUPER_PUNCH,
    TECH_SPIN_OUT,
    TECH_FIREBOMB,
    TECH_ROARING_WATER,
    TECH_WATER_GUN,
    TECH_BUBBLES,
    TECH_AIR_STRIKE,
    TECH_BUG_BITE,
    TECH_ROUNDHOUSE,
    TECH_FIRE_PUNCH,
    TECH_ICE_PUNCH,
    TECH_BOLT_PUNCH,
    TECH_CRUNCH,
    TECH_VENOM_FANG,
    TECH_FLAME_FANG,
    TECH_CRACKLE_FANG,
    TECH_ICE_FANG,
    TECH_SHADOW_CLAW,
    TECH_SHADOW_PULSE,
    TECH_AQUA_PULSE,
    TECH_GRASS_PULSE,
    TECH_LEAF_SWIRL,
    TECH_LEAF_SLICE,
    TECH_AQUA_BLADE,
    TECH_DRAGON_FANG,
    TECH_DEJA_VU,
    TECH_ECHO,
    TECH_GRAVITY,
    TECH_EARTHQUAKE,
    TECH_FLAMEBAR,
    TECH_CHAIN_ZAP,
    TECH_CHAIN_WHIP,
    TECH_FLAME_WHIP,
    TECH_ICE_WHIP,
    TECH_STINGER,
    TECH_JOLT,

    TECH_MAX
};

enum {
    TEFFECT_HIT,
    TEFFECT_STATUS,
    TEFFECT_STAT_UP,
    TEFFECT_TARG_STAT_DROP,
    TEFFECT_TOP_DOWN,
    TEFFECT_RECOIL
};

#define NUM_TECHNIQUES 250

extern const Technique gTechniques[NUM_TECHNIQUES];


#endif