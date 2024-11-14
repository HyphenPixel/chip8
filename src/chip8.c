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
            printf("\nPC: Skipped \t| OP: 0x%04x \n", cpu->ram[cpu->pc-1]);
            printf("PC: Skipped \t| OP: 0x%04x", cpu->ram[cpu->pc]);
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
        default:
            printf("Invalid or not implemented", cpu->ram[cpu->pc]);
    }
}

void execute_instruction(Chip8* cpu) {
    uint16_t opcode = fetch_opcode(cpu);
    decode_opcode(cpu, opcode);
}

void cycle_cpu(Chip8* cpu, uint64_t cycles) {
    for (size_t i = 0; i < cycles; ++i) {
        printf("PC: 0x%04x \t| OP: 0x%04x ", cpu->pc, cpu->ram[cpu->pc]);
        execute_instruction(cpu);
        printf("\n");
    }
}