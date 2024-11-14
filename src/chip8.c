#include <chip8.h>
#include <stdio.h>

uint16_t fetch_opcode(Chip8* cpu) {
    uint8_t high = cpu->ram[cpu->pc];
    cpu->pc++;
    uint8_t low = cpu->ram[cpu->pc];
    cpu->pc++;
    return TO_16BIT(high, low);
}

void decode_opcode(Chip8* cpu, uint16_t opcode) {
    uint8_t vx = GET_NIBBLE(opcode, 2);
    uint8_t vy = GET_NIBBLE(opcode, 1);
    switch (GET_NIBBLE(opcode, 3)) {
        case 0:
            switch (TO_12BIT(opcode)) {
                case 0x00E0:
                    memset(cpu->vram, 0, sizeof(cpu->vram));
                    break;
                case 0x00EE:
                    cpu->pc = cpu->stack[cpu->sp];
                    cpu->sp--;
                    break;
                default:
                    printf("Invalid or not implemented", cpu->ram[cpu->pc]);
            }
            break;
        case 1:
            cpu->pc = TO_12BIT(opcode);
            break;
        case 2:
            cpu->stack[cpu->sp] = cpu->ram[cpu->pc];
            cpu->sp++;
            break;
        case 3:
            if (cpu->v[GET_NIBBLE(opcode, 2)] == GET_BYTE(opcode, 0))
                cpu->pc += 2;
            break;
        case 4:
            if (cpu->v[GET_NIBBLE(opcode, 2)] != GET_BYTE(opcode, 0))
                cpu->pc += 2;
            break;
        case 5:
            if (cpu->v[GET_NIBBLE(opcode, 2)] != cpu->v[GET_NIBBLE(opcode, 1)])
                cpu->pc += 2;
            break;
        case 6:
            cpu->v[GET_NIBBLE(opcode, 2)] = GET_BYTE(opcode, 0);
            break;
        case 7:
            cpu->v[GET_NIBBLE(opcode, 2)] += GET_BYTE(opcode, 0);
            break;
        case 8:
            switch (GET_NIBBLE(opcode, 0)) {
                case 0:
                    cpu->v[GET_NIBBLE(opcode, 2)] = cpu->v[GET_NIBBLE(opcode, 1)];
                    break;
                case 1:
                    cpu->v[GET_NIBBLE(opcode, 2)] |= cpu->v[GET_NIBBLE(opcode, 1)];
                    break;
                case 2:
                    cpu->v[GET_NIBBLE(opcode, 2)] &= cpu->v[GET_NIBBLE(opcode, 1)];
                    break;
                case 3:
                    cpu->v[GET_NIBBLE(opcode, 2)] ^= cpu->v[GET_NIBBLE(opcode, 1)];
                    break;
                case 4:
                    cpu->v[0xF] = cpu->v[vx] + cpu->v[vy] > 0xFF ? 1 : 0;
                    cpu->v[vx] = cpu->v[vx] + cpu->v[vy];
                    break;
                case 5: // ! May be incorrect
                    cpu->v[0xF] = (cpu->v[vx]) > (cpu->v[vy]) ? 1 : 0;
                    cpu->v[vx] = cpu->v[vx] - cpu->v[vy];
                    break;
                case 6:
                    cpu->v[0xF] = CHECK_BIT(cpu->v[vx], 0) ? 1 : 0;
                    cpu->v[vx] >>= 1;
                    break;
                case 7:
                    cpu->v[0xF] = (cpu->v[vx]) < (cpu->v[vy]) ? 1 : 0;
                    cpu->v[vx] = cpu->v[vx] - cpu->v[vy];
                    break;
                case 0xE:
                    cpu->v[0xF] = CHECK_BIT(cpu->v[vx], 7) ? 1 : 0;
                    cpu->v[vx] <<= 1;
                    break;
                default:
                    printf("Invalid or not implemented", cpu->ram[cpu->pc]);
            }
            break;
        default:
            printf("Invalid or not implemented", cpu->ram[cpu->pc]);
    }
}

void execute_instruction(Chip8* cpu) {
    uint16_t opcode = fetch_opcode(cpu);
    decode_opcode(cpu, opcode);
}

#ifdef DEBUG
void debug(Chip8* cpu) {
    printf("PC: 0x%04X \t| OP: 0x%04X ", cpu->pc, cpu->ram[cpu->pc]);
}
#endif

void cycle_cpu(Chip8* cpu, uint64_t cycles) {
    for (size_t i = 0; i < cycles; ++i) {
        #ifdef DEBUG
        debug(cpu);
        #endif
        execute_instruction(cpu);
        printf("\n");
    }
}