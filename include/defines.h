#pragma once

#define CHECK_BIT(reg, bit_to_check) (reg & (1 << bit_to_check) != 0)
#define SET_BIT(reg, bit_to_set) (reg |= bit_to_set)
#define CLEAR_BIT(reg, bit_to_clear) (reg &= ~bit_to_clear)
#define TO_16BIT(high_byte, low_byte) ((high_byte << 8) | low_byte)
#define GET_NIBBLE(value, nibble)  (((value) >> ((nibble) * 4)) & 0xF)
#define TO_12BIT(value) (value & 0x0FFF)
#define GET_BYTE(value, byte)  (((value) >> ((byte) * 8)) & 0xFF)
