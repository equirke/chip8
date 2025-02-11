#include <stdio.h>
#include "chip8.h"
#include "processor.h"


void print_array(chip8* chip)
{
        for(int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for(int x = 0; x < SCREEN_WIDTH; x++)
        {
            printf("%02X ", chip->screen[x][y]);
        }

        printf("\n");
    }
}
//------------------------------------------------------------------------------------
// For debugging with gdb without dealing with graphics
//------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{

    if(argc < 2)
        return 1;

    FILE* fp = fopen(argv[1], "r");

    if(fp == NULL)
        return 1;

    byte program[MEMORY_SIZE];

    int program_size = fread(program, 1, MEMORY_SIZE, fp);

    fclose(fp);

    chip8 chip;

    init_chip8(&chip, program, program_size);


    while(1)    
    {
        step(&chip);
    }

    return 0;
}