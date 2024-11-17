#include <chip8.h>
#include <stdio.h>
#include <display.h>
#include <keyboard.h>


// -----------------------------
// from https://www.raylib.com/examples.html
#define MAX_SAMPLES               512
#define MAX_SAMPLES_PER_UPDATE   4096
// Cycles per second (hz)
float frequency = 440.0f;
// Audio frequency, for smoothing
float audioFrequency = 440.0f;
// Previous value, used to test if sine needs to be rewritten, and to smoothly modulate frequency
float oldFrequency = 1.0f;
// Index for audio rendering
float sineIdx = 0.0f;
// Audio input processing callback
void AudioInputCallback(void *buffer, unsigned int frames){
    audioFrequency = frequency + (audioFrequency - frequency)*0.95f;
    float incr = audioFrequency/44100.0f;
    short *d = (short *)buffer;
    for (unsigned int i = 0; i < frames; i++){
        d[i] = (short)(32000.0f*sinf(2*PI*sineIdx));
        sineIdx += incr;
        if (sineIdx > 1.0f) sineIdx -= 1.0f;
    }
}
// -----------------------------

int main(int argc, char *argv[]) {
    // -------------------------
    // from https://www.raylib.com/examples.html
    InitAudioDevice();  // Initialize audio device
    SetAudioStreamBufferSizeDefault(MAX_SAMPLES_PER_UPDATE);
    // Init raw audio stream (sample rate: 44100, sample size: 16bit-short, channels: 1-mono)
    AudioStream stream = LoadAudioStream(44100, 16, 1);
    SetAudioStreamCallback(stream, AudioInputCallback);
    // Buffer for the single cycle waveform we are synthesizing
    short *data = (short *)malloc(sizeof(short)*MAX_SAMPLES);
    // Frame buffer, describing the waveform when repeated over the course of a frame
    short *writeBuf = (short *)malloc(sizeof(short)*MAX_SAMPLES_PER_UPDATE);
    int waveLength = 1;
    // -------------------------

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
        if (frequency != oldFrequency)
        {
            // Compute wavelength. Limit size in both directions.
            //int oldWavelength = waveLength;
            waveLength = (int)(22050/frequency);
            if (waveLength > MAX_SAMPLES/2) waveLength = MAX_SAMPLES/2;
            if (waveLength < 1) waveLength = 1;

            // Write sine wave
            for (int i = 0; i < waveLength*2; i++)
            {
                data[i] = (short)(sinf(((2*PI*(float)i/waveLength)))*32000);
            }
            // Make sure the rest of the line is flat
            for (int j = waveLength*2; j < MAX_SAMPLES; j++)
            {
                data[j] = (short)0;
            }

            // Scale read cursor's position to minimize transition artifacts
            //readCursor = (int)(readCursor * ((float)waveLength / (float)oldWavelength));
            oldFrequency = frequency;
        }
        if (cpu->sound_timer > 0)
            PlayAudioStream(stream);
        else
            StopAudioStream(stream);
        cycle_cpu(cpu);
        draw(canvas, cpu);
    }

    // Cleanup
    cleanupCanvas(canvas);
    free(cpu);
    free(data);

    UnloadAudioStream(stream);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
