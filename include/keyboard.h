#pragma once
#include <raylib.h>

static const int keyMap[16] = {
    KEY_ONE, KEY_TWO, KEY_THREE, KEY_FOUR,  // 1, 2, 3, C
    KEY_Q, KEY_W, KEY_E, KEY_R,             // 4, 5, 6, D
    KEY_A, KEY_S, KEY_D, KEY_F,             // 7, 8, 9, E
    KEY_Z, KEY_X, KEY_C, KEY_V              // A, 0, B, F
};

int getKey(void);