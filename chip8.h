#ifndef __CHIP8_H__
#define __CHIP8_H__

#include "chip_constants.h"

typedef struct chip8
{
    byte memory[MEMORY_SIZE];
    word stack[STACK_SIZE];
    byte v_regs[REG_COUNT];
    addr PC;
    addr SP;
    addr I;
    byte DT;
    byte ST;
    byte screen[SCREEN_WIDTH][SCREEN_HEIGHT];
    byte keys[16];
}chip8;

#endif