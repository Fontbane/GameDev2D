#ifndef __GLOBAL_H__
#define __GLOBAL_H__


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef signed char s8;
typedef short s16;
typedef long s32;

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

enum {
    COLL_IMPASSIBLE,
    COLL_WATER,
    COLL_2,
    COLL_3,
    COLL_4,
    COLL_5,
    COLL_6,
    COLL_TRANSITION,
    COLL_MULTILEVEL
};

enum {
    MB_NONE,
    MB_CRACKED,
    MB_HOLE,
    MB_GRASS,
    MB_MUD,
    MB_WATER,
    MB_LEDGE,
    MB_CONVEYER,
    MB_SLOPE
};

#define ACTION_NONE 0X00
#define ACTION_MOVE 0X01
#define ACTION_TECH 0X02
#define ACTION_ITEM 0X03
#define ACTION_SWAP 0X04
#define ACTION_RUN  0X08


#endif