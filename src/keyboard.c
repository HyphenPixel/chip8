#include <keyboard.h>

int getKey(void) {
    for (int i = 0; i < 16; i++)
        if (IsKeyPressed(keyMap[i]))
            return i;
    return -1;
}