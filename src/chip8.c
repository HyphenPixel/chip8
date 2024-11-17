#include <chip8.h>

#define LOAD_ADDRESS 0x200
#define FONT_SPRITE_HEIGHT 5
#define FONT_START_ADDRESS 0x000

void loadRom(Chip8* cpu, const char* romFile) {
    FILE* rom = fopen(romFile, "rb");
    if (rom == NULL) {
        perror("Error opening file");
        return;
    }
    fseek(rom, 0, SEEK_END);
    size_t romSize = ftell(rom);
    fseek(rom, 0, SEEK_SET);

    // Most chip8 programs start at address 0x200
    if (romSize > (sizeof(cpu->ram) - LOAD_ADDRESS)) {
        fprintf(stderr, "ERROR: ROM is to large for available memory!\n");
        fclose(rom);
        return;
    }

    fread(&cpu->ram[LOAD_ADDRESS], sizeof(unsigned char), romSize, rom);

    fclose(rom);
}

Chip8* initChip8(void) {
    Chip8* cpu = (Chip8 *)malloc(sizeof(Chip8));
    memset(cpu, 0, sizeof(Chip8)); // Zero out struct data

    cpu->pc = 0x200;

    if (FONT_START_ADDRESS + sizeof(font)/sizeof(font[0]) > sizeof(cpu->ram)/sizeof(cpu->ram[0]) == 0)
        for (size_t i = 0; i < sizeof(font)/sizeof(font[0]); ++i)
            cpu->ram[FONT_START_ADDRESS + i] = font[i]; 
            
    return cpu;
}

u16 fetch_opcode(Chip8* cpu) {
    if (cpu->pc >= 0xFFF)
        return 0;
    u8 high = cpu->ram[cpu->pc];
    cpu->pc++;
    u8 low = cpu->ram[cpu->pc];
    cpu->pc++;
    return TO_16BIT(high, low);
}

void decode_opcode(Chip8* cpu, u16 opcode) {
    u8 vx = GET_NIBBLE(opcode, 2);
    u8 vy = GET_NIBBLE(opcode, 1);
    switch (GET_NIBBLE(opcode, 3)) {
        case 0:
            switch (TO_12BIT(opcode)) {
                case 0x00E0:
                    memset(cpu->vram, 0, sizeof(cpu->vram));
                    break;
                case 0x00EE:
                    if (cpu->sp - 1 < 0) {
                        cpu->halted = 1;
                        fprintf(stderr, "Halted");
                    } else {
                        cpu->sp--;
                        cpu->pc = cpu->stack[cpu->sp];
                    }
                    break;
                default:
                    printf("Invalid or not implemented");
            }
            break;
        case 1:
            cpu->pc = TO_12BIT(opcode);
            break;
        case 2:
            cpu->stack[cpu->sp] = cpu->pc;
            cpu->sp++;
            cpu->pc = TO_12BIT(opcode);
            break;
        case 3:
            if (cpu->v[vx] == GET_BYTE(opcode, 0))
                cpu->pc += 2;
            break;
        case 4:
            if (cpu->v[vx] != GET_BYTE(opcode, 0))
                cpu->pc += 2;
            break;
        case 5:
            if (cpu->v[vx] == cpu->v[vy])
                cpu->pc += 2;
            break;
        case 6:
            cpu->v[vx] = GET_BYTE(opcode, 0);
            break;
        case 7:
            cpu->v[vx] += GET_BYTE(opcode, 0);
            break;
        case 8:
            switch (GET_NIBBLE(opcode, 0)) {
                case 0:
                    cpu->v[vx] += cpu->v[vy];
                    break;
                case 1:
                    cpu->v[vx] |= cpu->v[vy];
                    break;
                case 2:
                    cpu->v[vx] &= cpu->v[vy];
                    break;
                case 3:
                    cpu->v[vx] ^= cpu->v[vy];
                    break;
                case 4:
                    cpu->v[0xF] = cpu->v[vx] + cpu->v[vy] > 0xFF ? 1 : 0;
                    cpu->v[vx] += cpu->v[vy];
                    break;
                case 5: // ! May be incorrect
                    cpu->v[0xF] = (cpu->v[vx]) > (cpu->v[vy]) ? 1 : 0;
                    cpu->v[vx] -= cpu->v[vy];
                    break;
                case 6:
                    cpu->v[0xF] = CHECK_BIT(cpu->v[vx], 0) ? 1 : 0;
                    cpu->v[vx] >>= 1;
                    break;
                case 7: // ! May be incorrect
                    cpu->v[0xF] = (cpu->v[vx]) < (cpu->v[vy]) ? 1 : 0;
                    cpu->v[vx] -= cpu->v[vy];
                    break;
                case 0xE:
                    cpu->v[0xF] = CHECK_BIT(cpu->v[vx], 7) ? 1 : 0;
                    cpu->v[vx] <<= 1;
                    break;
                default:
                    printf("Invalid or not implemented");
            }
            break;
        case 9:
            if (cpu->v[vx] != cpu->v[vy])
                cpu->pc += 2;
            break;
        case 0xA:
            cpu->i = TO_12BIT(opcode);
            break;
        case 0xB:
            cpu->pc = TO_12BIT(opcode) + cpu->v[0];
            break;
        case 0xC:
            cpu->v[vx] = (u8)rand() & GET_BYTE(opcode, 0);
            break;
        case 0xD:
            cpu->v[0xF] = 0;
            
            for (int row = 0; row < GET_NIBBLE(opcode, 0); ++row) {
                u8 spriteByte = cpu->ram[cpu->i + row];
                for (int col = 0; col < 8; ++col) {
                    u8 spritePixel = (spriteByte >> (7 - col)) & 1;
                    int x = (cpu->v[vx] + col) % 64;
                    int y = (cpu->v[vy] + row) % 32;

                    int screenIndex = y * 64 + x;

                    if (spritePixel && cpu->vram[screenIndex] == 1)
                        cpu->v[0xF] = 1;
        
                    cpu->vram[screenIndex] ^= spritePixel;
                }
            }
            break;
        case 0xE:
            switch (GET_BYTE(opcode, 0)) {
                case 0x9E:
                    if (keyMap[GetKeyPressed()] == cpu->v[vx])
                        cpu->pc += 2;
                    break;
                case 0xA1:
                    if (keyMap[GetKeyPressed()] != cpu->v[vx])
                        cpu->pc += 2;
                    break;
                default:
                    printf("Invalid or not implemented");
            }
            break;
        case 0xF:
            switch (GET_BYTE(opcode, 0)) {
                case 0x07:
                    cpu->v[vx] = cpu->delay_timer;
                    break;
                case 0x0A:
                    cpu->waiting = 1;
                    cpu->waitReg = vx;
                    break;
                case 0x15:
                    cpu->delay_timer = cpu->v[vx];
                    break;
                case 0x18:
                    cpu->sound_timer = cpu->v[vx];
                    break;
                case 0x1E:
                    cpu->i += cpu->v[vx];
                    break;
                case 0x29:
                    cpu->i = FONT_START_ADDRESS + (cpu->v[vx] * FONT_SPRITE_HEIGHT);
                    break;
                case 0x33:
                    uint8_t value = cpu->v[vx];
                    uint8_t digits[3] = { 
                        value / 100, 
                        (value / 10) % 10,
                        value % 10
                        };
                    for (size_t i = 0; i < 3; ++i) {
                        if (cpu->i + i >= sizeof(cpu->ram)/sizeof(cpu->ram[0])){
                            fprintf(stderr, "Error: Memory access out of bounds");
                            return;
                        }
                        cpu->ram[cpu->i + i] = digits[i];
                    }
                    break;
                case 0x55:
                    for (size_t i = 0; i < sizeof(cpu->v)/sizeof(cpu->v[0]); ++i)
                        cpu->ram[cpu->i + i] = cpu->v[i];
                    break;
                case 0x65:
                    for (size_t i = 0; i < sizeof(cpu->v)/sizeof(cpu->v[0]); ++i)
                        cpu->v[i] = cpu->ram[cpu->i + i];
                    break;
                default:
                    printf("Invalid or not implemented");
            }
            break;
        default:
            printf("Invalid or not implemented");
    }
}

void execute_instruction(Chip8* cpu) {
    u16 opcode = fetch_opcode(cpu);
    decode_opcode(cpu, opcode);
}

#ifdef DEBUG
void debug(Chip8* cpu) {
    printf("(0x%01X, 0x%01X)\n", GET_NIBBLE(cpu->ram[cpu->pc], 0), GET_NIBBLE(cpu->ram[cpu->pc + 1], 1));
    printf("PC: 0x%03X S: %d [0x%03X] | OP: 0x%02X%02X ", cpu->pc, cpu->sp, cpu->stack[cpu->sp - 1], cpu->ram[cpu->pc], cpu->ram[cpu->pc + 1]);
    for (size_t i = 0; i < sizeof(cpu->v)/sizeof(cpu->v[0]); ++i) {
        if (i % 4 == 0)
            printf("\n");
        printf("V%lX: 0x%02X ",i , cpu->v[i]);
    }
    printf("\nI: 0x%04X\n", cpu->i);
    sleep(1);
}
#endif

void cycle_cpu(Chip8* cpu) {
    if (cpu->halted == 0 && cpu->waiting == 0) {
        if (cpu->delay_timer > 0)
            --cpu->delay_timer;
        if (cpu->sound_timer > 0)
            --cpu->sound_timer;

        srand((u8)time(NULL));

        #ifdef DEBUG
        debug(cpu);
        #endif

        execute_instruction(cpu);

        #ifdef DEBUG
        printf("\n");
        #endif
    } else if (cpu->waiting == 1) { // TODO
        for(size_t i = 0; i < sizeof(keyMap)/sizeof(keyMap[0]); ++i)
            if (IsKeyDown(keyMap[i])) {
                cpu->v[cpu->waitReg] = i;
                cpu->waiting = false;
                break;
            } else {
                cpu->waiting = true;
            }
    }
}