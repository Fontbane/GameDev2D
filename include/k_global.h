#ifndef __K_GLOBAL_H__
#define __K_GLOBAL_H__

#include "gfc_types.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;
typedef signed char s8;
typedef short s16;
typedef long s32;

typedef struct { s8 x, y; } Point8; 
typedef struct { u8 x, y, w, h; } Rect8;

#define HIHALF(x) x>>4
#define LOHALF(x) x&0x0F

typedef enum {
    T_MYSTERY,
    T_BASIC,
    T_PLANT,
    T_FLAME,
    T_AQUA,
    T_AIR,
    T_ELECTRIC,
    T_TOXIC,
    T_BUG,
    T_EARTH,
    T_METAL,
    T_ICE,
    T_MIND,
    T_BRAWL,
    T_GHOST,
    T_DRAGON
} Type;

typedef enum Direction{
    DIR_S,
    DIR_W,
    DIR_N,
    DIR_E
}Direction;

typedef enum GameState {
    GAMESTATE_MENU,
    GAMESTATE_FIELD,
    GAMESTATE_BATTLE
}GameState;

#define ACTION_NONE 0X00
#define ACTION_MOVE 0X01
#define ACTION_TECH 0X02
#define ACTION_ITEM 0X03
#define ACTION_SWAP 0X04
#define ACTION_RUN  0X08

extern Point directions[4];


#endif