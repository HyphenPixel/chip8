#pragma once
#include <raylib.h>
#include <chip8.h>

typedef struct {
    Color           color[2];
    Vector2         origin;
    RenderTexture2D surface;
    Rectangle       surface_source;
    Rectangle       surface_position;
} Canvas;

Canvas initCanvas(Color c1, Color c2);
void update(Canvas* canvas);
void draw(Canvas* canvas, Chip8* cpu);
void cleanupCanvas(Canvas* canvas);