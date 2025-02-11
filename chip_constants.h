#ifndef __CHIP8_CONSTANTS__
#define __CHIP8_CONSTANTS__

#define MEMORY_SIZE 4096
#define REG_COUNT 16 
#define INT_MEMROY 512
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define STACK_SIZE 17
#define REG_VF 0x0F

typedef unsigned char byte;
typedef unsigned short word;
typedef word addr;

typedef enum opcode{
    SYS_A, CLS, RET, JP_A, CALL_A, SE_RC,
    SNE_RC, SE_RR, LD_RC, ADD_RC, LD_RR,
    OR_RR, AND_RR, XOR_RR, ADD_RR, SUB_RR,
    SHR_RR, SUBN_RR, SHL_RR, SNE_RR, LD_IA,
    JP_V0A, RND_RC, DRW_RRC, SKP_R, SKNP_R,
    LD_RD, LD_RK, LD_DR, LD_SR, ADD_IR, LD_FR,
    LD_BR, LD_IAR, LD_RAI, EXIT
}opcode;

#endif