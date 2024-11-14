#include <chip8.h>
#include <stdio.h>
#include <display.h>

int main() {
    srand((uint8_t)time(NULL));
    int cycles = 10;
    Chip8 chip8 = {0};
    memset(chip8.vram, 0, sizeof(chip8.vram));
    chip8.ram[0] = 0xA0;
    chip8.ram[1] = 0x0F;
    chip8.ram[2] = 0xD0;
    chip8.ram[3] = 0x08;
    chip8.ram[0xF] =    0b00100100;
    chip8.ram[010] =    0b00100100;
    chip8.ram[0x11] =   0b10000001;
    chip8.ram[0x12] =   0b01111110;

    cycle_cpu(&chip8, cycles);

    Canvas c = initCanvas(WHITE, BLACK);
    while (!WindowShouldClose())
        draw(&c, &chip8);
    cleanupCanvas(&c);
    return 0;
}
