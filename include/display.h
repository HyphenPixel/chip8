/**
 * @file display.h
 * @brief This file contains definitions and declarations for the Canvas structure
 *        and functions to manage and render a drawing canvas for a CHIP-8 emulator.
 */
#pragma once
#include <raylib.h>
#include "chip8.h"

/**
 * @struct Canvas
 * @brief Represents a drawing canvas for the CHIP-8 emulator.
 *
 * This structure contains properties for defining a renderable canvas,
 * including its colors, origin, rendering surface, and positioning information.
 */
typedef struct {
    Color           color[2];            /**< Background and foreground colors for the canvas. */
    Vector2         origin;              /**< Origin of the canvas in 2D space. */
    RenderTexture2D surface;             /**< Rendering surface for the canvas. */
    Rectangle       surface_source;      /**< Source rectangle for rendering operations. */
    Rectangle       surface_position;    /**< Destination rectangle for rendering operations. */
} Canvas;

/**
 * @brief Initializes a new Canvas object with the specified colors.
 *
 * @param c1 The first color (background).
 * @param c2 The second color (foreground).
 * @return A pointer to the newly created Canvas object.
 */
Canvas* initCanvas(Color c1, Color c2);

/**
 * @brief Draws the contents of the CHIP-8 emulator onto the canvas.
 *
 * @param canvas Pointer to the Canvas object to draw on.
 * @param cpu Pointer to the CHIP-8 CPU state to render.
 */
void draw(Canvas* canvas, Chip8* cpu);

/**
 * @brief Cleans up resources associated with a Canvas object.
 *
 * @param canvas Pointer to the Canvas object to clean up.
 */
void cleanupCanvas(Canvas* canvas);
