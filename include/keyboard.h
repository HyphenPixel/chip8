#pragma once
#include <raylib.h>

static const int keyMap[16] = {
    KEY_X, KEY_ONE, KEY_TWO, KEY_THREE,
    KEY_Q, KEY_W, KEY_E, KEY_A, 
    KEY_S, KEY_D, KEY_Z, KEY_C, 
    KEY_FOUR, KEY_R, KEY_F, KEY_V
};

int getKey(void);