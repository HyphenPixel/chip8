#include <display.h>
#include <chip8.h>

Canvas* initCanvas(Color c1, Color c2) {
    Canvas* canvas = (Canvas*)malloc(sizeof(Canvas));
    canvas->color[0] = c1;
    canvas->color[1] = c2;
    canvas->surface = LoadRenderTexture(64, 32);
    canvas->surface_source = (Rectangle){0.0f, 0.0f, (float)canvas->surface.texture.width, -(float)canvas->surface.texture.height};
    canvas->surface_position = (Rectangle){0.0f, 0.0f, GetScreenWidth(), GetScreenHeight()};
    canvas->origin = (Vector2){0.0f, 0.0f};
    return canvas;
}

void update(Canvas* canvas) {

}

void draw(Canvas* canvas, Chip8* cpu) {
    BeginTextureMode(canvas->surface);
        ClearBackground(canvas->color[1]);
        for (size_t i = 0; i < sizeof(cpu->vram)/sizeof(cpu->vram[0]); ++i) {
            size_t x = i % 64;
            size_t y = i / 64;
            if (cpu->vram[i] == 1)
                DrawPixel(x, y, canvas->color[0]);
        }
    EndTextureMode();

    BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(canvas->surface.texture, canvas->surface_source, canvas->surface_position, canvas->origin, 0.0f, WHITE);
        DrawFPS(8, 8);
    EndDrawing();
}

void cleanupCanvas(Canvas* canvas) {
    UnloadRenderTexture(canvas->surface);
    free(canvas);
}
