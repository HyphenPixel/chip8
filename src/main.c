#include <chip8.h>
#include <stdio.h>

int main() {
    int cycles = 10;
    Chip8 chip8;
    chip8.ram[0] = 0x60;
    chip8.ram[1] = 0xFF;
    chip8.ram[2] = 0x30;
    chip8.ram[3] = 0xFF;
    chip8.ram[4] = 0xAA;
    chip8.ram[5] = 0xCC;
    chip8.ram[6] = 0x61;
    chip8.ram[7] = 0x9A;

    cycle_cpu(&chip8, cycles);

    printf("\nV0: 0x%04x\n", chip8.v[0]);
    printf("V1: 0x%04x\n", chip8.v[1]);

    return 0;
}
