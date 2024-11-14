#include <display.h>
#include <chip8.h>

Canvas initCanvas(Color c1, Color c2) {
    InitWindow(1280, 720, "Chip8");
    SetTargetFPS(60);
    Canvas c = {0};
    c.color[0] = c1;
    c.color[1] = c2;
    c.surface = LoadRenderTexture(64, 32);
    c.surface_source = (Rectangle){0.0f, 0.0f, (float)c.surface.texture.width, -(float)c.surface.texture.height};
    c.surface_position = (Rectangle){0.0f, 0.0f, GetScreenWidth(), GetScreenHeight()};
    c.origin = (Vector2){0.0f, 0.0f};
    return c;
}

void update(Canvas* canvas) {

}

void draw(Canvas* canvas, Chip8* cpu) {
    BeginTextureMode(canvas->surface);
        ClearBackground(canvas->color[1]);
        for (size_t i = 0; i < sizeof(cpu->vram)/sizeof(cpu->vram[0]); ++i)
            if (cpu->vram[i] == 1) {
                size_t x = i % 64;
                size_t y = i / 64;
                DrawPixel(x, y, canvas->color[0]);
            }
    EndTextureMode();

    BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(canvas->surface.texture, canvas->surface_source, canvas->surface_position, canvas->origin, 0.0f, WHITE);
    EndDrawing();
}

void cleanupCanvas(Canvas* canvas) {
    UnloadRenderTexture(canvas->surface);
    CloseWindow();
}
