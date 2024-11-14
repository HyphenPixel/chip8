#include <chip8.h>
#include <stdio.h>
#include <display.h>

int main(int argc, char *argv[]) {
    srand((uint8_t)time(NULL));
    int cycles = 10;
    Chip8 chip8 = initChip8();
    
    loadRom(&chip8, "program.c8");

    cycle_cpu(&chip8, cycles);

    Canvas c = initCanvas(WHITE, BLACK);
    for (int i = 0x200; i < 0x220; ++i)
        printf("0x%04x : 0x%04x\n", i, chip8.ram[i]);
    while (!WindowShouldClose())
        draw(&c, &chip8);
    cleanupCanvas(&c);
    return 0;
}
