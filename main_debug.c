#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "chip8.h"
#include "processor.h"
#include "parse_instruction.h"

char* opcodes_to_string(opcode value) ;
void set_key_pressed(chip8 *chip);

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
    const int debugSpace = 320;
    const int framerate = 60;
    int hertz = 600;
    int steps_per_frame = hertz / framerate;

    int step_thru = 0;

    InitWindow(screenWidth, screenHeight + debugSpace, "Chip8");

    SetTargetFPS(framerate);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if(IsKeyPressed(KEY_P))
            step_thru = !step_thru;
        
        if(!step_thru)
        {
            for(int i = 0; i < steps_per_frame; i++)
            {
                set_key_pressed(&chip);
                chip8_step(&chip);
            }
        }
        else if (IsKeyPressed(KEY_N))
        {
            chip8_step(&chip);
        }
        
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

            int j = 0;
            int x = 0;
            for(int i = 0; i < REG_COUNT; i++)
            {
                if((i % 4) == 0 )
                {
                    x=0;
                    j++;
                }
                DrawText(TextFormat("V%02i: %03i", i, chip.v_regs[i]), x * 100 , 340 + (j * 20), 20, BEIGE);
                x++;
            }

            DrawText(TextFormat("PC: %04i", chip.PC), 420, 360, 20, BEIGE);
            DrawText(TextFormat("I: %04i", chip.I),  520, 360, 20, BEIGE);
            DrawText(TextFormat("ST: %03i", chip.ST), 420, 380, 20, BEIGE);
            DrawText(TextFormat("SP: %02i", chip.SP), 520, 380, 20, BEIGE);
            DrawText(TextFormat("DT: %03i", chip.DT), 420, 400, 20, BEIGE);
            DrawText(TextFormat("RET: %04i", chip.stack[chip.SP]), 520, 400, 20, BEIGE);

            for(int i = -6; i <= 6; i += 2)
            {
                opcode op = parse_opcode(chip.memory[chip.PC + i], chip.memory[chip.PC + i+1]);
                char* op_to_string = opcodes_to_string(op);
                if(i == 0)
                    DrawText(TextFormat("!%s!", op_to_string), (screenWidth / 2) + (i * 50), 500, 20, BEIGE);
                else
                    DrawText(TextFormat("%s", op_to_string), (screenWidth / 2) + (i * 50), 500, 20, BEIGE);
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

void set_key_pressed(chip8 *chip)
{   // 1 2 3 C 
    // 4 5 6 D
    // 7 8 9 E
    // A 0 B F
    int raylib_keys[] = {KEY_ONE, KEY_TWO, KEY_THREE, KEY_FOUR,
                    KEY_Q, KEY_W, KEY_E, KEY_R,
                    KEY_A, KEY_S, KEY_D, KEY_F,
                    KEY_Z, KEY_X, KEY_C, KEY_V};

    int chip8_keys[] =  {0x01, 0x02, 0x03, 0x0C,
                         0x04, 0x05, 0x06, 0x0D,
                         0x07, 0x08, 0x09, 0x0E,
                         0x0A, 0x00, 0x0B, 0x0F};

    if(chip->awaiting_input)
    {
        for(int i = 0; i < 16; i++)
            if(IsKeyDown(raylib_keys[i]))
            {
                chip->keys[chip8_keys[i]] = 1;
                chip->awaiting_input = 0;
                chip8_step(chip);
            }

        return;
    }
    
    for(int i = 0; i < 16; i++)
    {
        if(IsKeyDown(raylib_keys[i]))
            chip->keys[chip8_keys[i]] = 1;
        if(IsKeyReleased(raylib_keys[i]))
            chip->keys[chip8_keys[i]] = 0;
    }
}

char* opcodes_to_string(opcode value) 
{
    switch (value) {
        case SYS_A: return "SYS_A";
        case CLS: return "CLS";
        case RET: return "RET";
        case JP_A: return "JP_A";
        case CALL_A: return "CALL_A";
        case SE_RC: return "SE_RC";
        case SNE_RC: return "SNE_RC";
        case SE_RR: return "SE_RR";
        case LD_RC: return "LD_RC";
        case ADD_RC: return "ADD_RC";
        case LD_RR: return "LD_RR";
        case OR_RR: return "OR_RR";
        case AND_RR: return "AND_RR";
        case XOR_RR: return "XOR_RR";
        case ADD_RR: return "ADD_RR";
        case SUB_RR: return "SUB_RR";
        case SHR_RR: return "SHR_RR";
        case SUBN_RR: return "SUBN_RR";
        case SHL_RR: return "SHL_RR";
        case SNE_RR: return "SNE_RR";
        case LD_IA: return "LD_IA";
        case JP_V0A: return "JP_V0A";
        case RND_RC: return "RND_RC";
        case DRW_RRC: return "DRW_RRC";
        case SKP_R: return "SKP_R";
        case SKNP_R: return "SKNP_R";
        case LD_RD: return "LD_RD";
        case LD_RK: return "LD_RK";
        case LD_DR: return "LD_DR";
        case LD_SR: return "LD_SR";
        case ADD_IR: return "ADD_IR";
        case LD_FR: return "LD_FR";
        case LD_BR: return "LD_BR";
        case LD_IAR: return "LD_IAR";
        case LD_RAI: return "LD_RAI";
        case EXIT: return "EXIT";
    }
    return "Unknown";
}