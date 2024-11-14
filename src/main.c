#include <chip8.h>
#include <stdio.h>

int main() {
    int cycles = 5;
    Chip8 chip8;
    chip8.ram[0] = 0x60;
    chip8.ram[1] = 0xFA;
    chip8.ram[2] = 0x70;
    chip8.ram[3] = 0x01;

    cycle_cpu(&chip8, cycles);

    printf("\nV0: 0x%04X\n", chip8.v[0]);
    printf("V1: 0x%04X\n", chip8.v[1]);

    return 0;
}
