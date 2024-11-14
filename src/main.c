#include <chip8.h>
#include <stdio.h>

int main() {
    srand((uint8_t)time(NULL));
    int cycles = 10;
    Chip8 chip8 = {0};
    chip8.ram[0] = 0x70;
    chip8.ram[1] = 0x01;
    chip8.ram[2] = 0xB1;
    chip8.ram[3] = 0x00;
    chip8.ram[0x101] = 0x70;
    chip8.ram[0x102] = 0xFF;
    //chip8.ram[4] = 0x80;
    //chip8.ram[5] = 0x14;

    cycle_cpu(&chip8, cycles);

    printf("\nV0: 0x%03X\n", chip8.v[0]);
    printf("V1: 0x%03X\n", chip8.v[1]);
    printf("VF: 0x%03X\n", chip8.v[0xF]);
    printf("I: 0x%03X\n", chip8.i);

    return 0;
}
