#include "k_global.h"

Point directions[4] = {
    [DIR_S] = {  0,  1  },
    [DIR_W] = { -1,  0  },
    [DIR_N] = {  0, -1  },
    [DIR_E] = {  1,  0  }
};

#define MF_DOWN     0x0001
#define MF_UP       0x0010
#define MF_LEFT     0x0100
#define MF_RIGHT    0x1000