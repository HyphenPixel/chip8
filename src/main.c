#include <chip8.h>
#include <stdio.h>

int main() {
    int cycles = 5;
    Chip8 chip8;
    chip8.ram[0] = 0x70;
    chip8.ram[1] = 0x01;
    chip8.ram[2] = 0x80;
    chip8.ram[3] = 0x06;
    //chip8.ram[4] = 0x80;
    //chip8.ram[5] = 0x14;

    cycle_cpu(&chip8, cycles);

    uint8_t b = CHECK_BIT(chip8.v[0], 0);
    printf("\n%d\n", b);

    printf("\nV0: 0x%04X\n", chip8.v[0]);
    printf("V1: 0x%04X\n", chip8.v[1]);
    printf("VF: 0x%04X\n", chip8.v[0xF]);

    return 0;
}
