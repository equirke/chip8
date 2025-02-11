#include <stdio.h>
#include "raylib.h"
#include "chip8.h"
#include "processor.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    if(argc < 2)
        return 1;

    FILE* fp = fopen(argv[1], "r");

    if(fp == NULL)
        return 1;

    byte program[MEMORY_SIZE];

    int program_size = fread(program, 1, MEMORY_SIZE, fp);

    fclose(fp);

    chip8 chip;

    chip8_init(&chip, program, program_size);
    
    const int screenWidth = 640;
    const int screenHeight = 320;
    const int framerate = 60;
    int hertz = 500;
    int steps_per_frame = hertz / framerate;

    InitWindow(screenWidth, screenHeight, "Chip8");

    SetTargetFPS(framerate);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        for(int i = 0; i < steps_per_frame; i++)
        {
            chip8_step(&chip);
            if(chip.screen_changed)
                break;
        }
        chip.screen_changed = 0;
        chip8_tick_delay_timer(&chip);
        chip8_tick_sound_timer(&chip);
        BeginDrawing();

            ClearBackground(DARKBROWN);
            
            for(int y = 0; y < SCREEN_HEIGHT; y++)
            {
                for(int x = 0; x < SCREEN_WIDTH; x++)
                {
                    if(chip.screen[x][y] == 1)
                    {
                        int x_offset = x * (screenWidth / SCREEN_WIDTH);
                        int y_offset = y * (screenHeight / SCREEN_HEIGHT);
                        int rect_width = (screenWidth / SCREEN_WIDTH);
                        int rect_height = (screenHeight / SCREEN_HEIGHT);
                        DrawRectangle(x_offset, y_offset, rect_width, rect_height, BEIGE);
                    }
                }
            } 
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}