#include <chip8.h>
#include <stdio.h>
#include <display.h>
#include <keyboard.h>

int main(int argc, char *argv[]) {
    // Initialize the chip8 interpreter
    Chip8* cpu = initChip8();
    if (argc == 2) {
    loadRom(cpu, argv[1]);
    } else if (argc > 2) {
        fprintf(stderr, "Error: Too many arguments! Please provide only one filename.\n");
        return 1; // Exit with an error code
    } else {
        fprintf(stderr, "Error: No filename provided! Please provide a filename as an argument.\n");
        return 1; // Exit with an error code
    }

    // Initialize Raylib window and Canvas struct
    InitWindow(1280, 720, "Chip8");
    SetTargetFPS(60);
    Canvas* canvas = initCanvas(WHITE, BLACK);

    // Main loop
    while (!WindowShouldClose()) {
        cycle_cpu(cpu);
        draw(canvas, cpu);
    }

    // Cleanup
    cleanupCanvas(canvas);
    free(cpu);

    CloseWindow();
    return 0;
}
