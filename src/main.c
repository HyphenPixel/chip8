#include <chip8.h>
#include <stdio.h>
#include <display.h>
#include <keyboard.h>

int main(int argc, char *argv[]) {
    // Initialize the chip8 interpreter
    Chip8* cpu = initChip8();
    if (argc == 2)
        loadRom(cpu, argv[1]);
    else
        fprintf(stderr, "Too many arguments!");

    // Initialize Raylib window and Canvas struct
    InitWindow(1280, 720, "Chip8");
    SetTargetFPS(60);
    Canvas* canvas = initCanvas(WHITE, BLACK);

    // Main loop
    while (!WindowShouldClose()) {
        getKey();
        cycle_cpu(cpu);

        if (cpu->delay_timer > 0)
            --cpu->delay_timer;
        if (cpu->sound_timer > 0)
            --cpu->sound_timer;

        draw(canvas, cpu);
    }

    // Cleanup
    cleanupCanvas(canvas);
    free(cpu);

    CloseWindow();
    return 0;
}
