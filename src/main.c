#include <chip8.h>
#include <stdio.h>
#include <display.h>
#include <keyboard.h>

int main(int argc, char *argv[]) {
    int cycles = 0xFFF;
    Chip8 chip8 = initChip8();
    
    if (argc > 0)
        loadRom(&chip8, argv[1]);

    Canvas c = initCanvas(WHITE, BLACK);

    while (!WindowShouldClose()) {
        chip8.delay_timer--;
        chip8.sound_timer--;
        getKey();
        cycle_cpu(&chip8);
        draw(&c, &chip8);
    }
    cleanupCanvas(&c);
    return 0;
}
