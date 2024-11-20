/**
 * @file chip8.h
 * @brief Header file for the CHIP-8 interpreter.
 *
 * Contains the CHIP-8 structure, font data, and declarations for
 * initialization, ROM loading, and CPU operation functions.
 */

#pragma once
#include "chip8.h"
#include <stdint.h>
#include <raylib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/**
 * @brief Checks if a specific bit is set in a register.
 *
 * @param reg The register to check.
 * @param bit_to_check The bit position to check (0-indexed).
 * @return Non-zero if the bit is set, 0 otherwise.
 */
#define CHECK_BIT(reg, bit_to_check) ((reg & (1 << bit_to_check)) != 0)

/**
 * @brief Sets a specific bit in a register.
 *
 * @param reg The register to modify.
 * @param bit_to_set The bit position to set (0-indexed).
 */
#define SET_BIT(reg, bit_to_set) (reg |= bit_to_set)

/**
 * @brief Clears a specific bit in a register.
 *
 * @param reg The register to modify.
 * @param bit_to_clear The bit position to clear (0-indexed).
 */
#define CLEAR_BIT(reg, bit_to_clear) (reg &= ~bit_to_clear)

/**
 * @brief Combines two 8-bit values into a 16-bit value.
 *
 * @param high_byte The high byte (most significant 8 bits).
 * @param low_byte The low byte (least significant 8 bits).
 * @return A 16-bit value created by combining the high and low bytes.
 */
#define TO_16BIT(high_byte, low_byte) ((high_byte << 8) | low_byte)

/**
 * @brief Retrieves a specific nibble (4 bits) from a value.
 *
 * @param value The value to extract the nibble from.
 * @param nibble The nibble index (0 = least significant, 1 = second, etc.).
 * @return The 4-bit nibble as an integer.
 */
#define GET_NIBBLE(value, nibble)  (((value) >> ((nibble) * 4)) & 0xF)

/**
 * @brief Masks a value to 12 bits.
 *
 * @param value The value to mask.
 * @return The value masked to its least significant 12 bits.
 */
#define TO_12BIT(value) (value & 0x0FFF)

/**
 * @brief Retrieves a specific byte from a value.
 * 
 * @param value The value to extract the byte from.
 * @param byte The byte index (0 = least significant, 1 = second, etc.).
 * @return The 8-bit byte as an integer.
 */
#define GET_BYTE(value, byte)  (((value) >> ((byte) * 8)) & 0xFF)

#define u8 uint8_t
#define u16 uint16_t

/**
 * @brief CHIP-8 font set.
 *
 * Contains the hexadecimal sprites for characters 0-F, each 5 bytes tall.
 */
static const u8 font[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

/**
 * @brief CHIP-8 key map.
 *
 * Maps modern keyboard keys to CHIP-8 keypad keys
 */
static const int keyMap[16] = {
    KEY_X, KEY_ONE, KEY_TWO, KEY_THREE,
    KEY_Q, KEY_W, KEY_E, KEY_A, 
    KEY_S, KEY_D, KEY_Z, KEY_C, 
    KEY_FOUR, KEY_R, KEY_F, KEY_V
};

/**
 * @struct Chip8
 * @brief Represents the CHIP-8 virtual machine state.
 *
 * Stores memory, registers, stack, timers, and program state for emulation.
 */
typedef struct {
    u8      ram[4096];      /**< CHIP-8 RAM (4 KB). */
    u8      vram[64 * 32];  /**< Display memory (64x32 pixels, monochrome). */
    u8      v[16];          /**< General-purpose registers (V0 to VF). */
    u8      sound_timer;    /**< Sound timer, decrements at 60 Hz. */
    u8      delay_timer;    /**< Delay timer, decrements at 60 Hz. */
    u8      sp;             /**< Stack pointer. */
    u16     stack[16];      /**< Call stack. */
    u16     i;              /**< Index register. */
    u16     pc;             /**< Program counter. */
    u8      halted;         /**< Halted flag, indicates if execution is stopped. */
    u8      waiting;        /**< Indicates if the CPU is waiting for key input. */
    u8      waitReg;        /**< The register to store the key value when waiting. */
} Chip8;

#ifdef DEBUG
void debug(Chip8* cpu);
#endif

/**
 * @brief Initializes a new CHIP-8 instance.
 *
 * Allocates memory for the CHIP-8 structure and sets it to its initial state.
 * 
 * @return A pointer to the initialized CHIP-8 structure.
 */
Chip8* initChip8(void);

/**
 * @brief Loads a ROM file into the CHIP-8's memory.
 *
 * @param cpu Pointer to the CHIP-8 structure.
 * @param romFile Path to the ROM file to be loaded.
 */
void loadRom(Chip8* cpu, const char* romFile);

/**
 * @brief Fetches the next opcode from memory.
 *
 * @param cpu Pointer to the CHIP-8 structure.
 * @return The 16-bit opcode fetched from memory.
 */
u16 fetch_opcode(Chip8* cpu);

/**
 * @brief Decodes and executes a given opcode.
 *
 * @param cpu Pointer to the CHIP-8 structure.
 * @param opcode The 16-bit opcode to decode.
 */
void decode_opcode(Chip8* cpu, u16 opcode);

/**
 * @brief Executes the instruction represented by the opcode.
 *
 * Modifies the CHIP-8's state based on the opcode.
 *
 * @param cpu Pointer to the CHIP-8 structure.
 */
void execute_instruction(Chip8* cpu);

/**
 * @brief Advances the CHIP-8 emulation by one CPU cycle.
 *
 * This function fetches, decodes, and executes one instruction,
 * and updates timers.
 *
 * @param cpu Pointer to the CHIP-8 structure.
 */
void cycle_cpu(Chip8* cpu);