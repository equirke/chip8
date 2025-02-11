#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "chip8.h"
#include "parse_instruction.h"
#include "processor.h"



void op_sys(chip8* chip)
{
    return;
}

void op_cls(chip8* chip)
{
    for(int j = 0; j < SCREEN_HEIGHT; j++)
        for(int i = 0; i < SCREEN_WIDTH; i++)
        {
            chip->screen[i][j] == 0;
        }
}

void op_ret(chip8* chip)
{
    chip->PC = chip->stack[chip->SP];
    chip->SP--;
}

void op_jmp(chip8* chip, addr jmp_to_addr)
{
    chip->PC = jmp_to_addr;
}

void op_call(chip8* chip, addr jump_to_addr)
{
    chip->SP++;
    chip->stack[chip->SP] = chip->PC;
    op_jmp(chip, jump_to_addr);
}

void op_skip_if_equal_rc(chip8* chip, byte reg_x, byte constant)
{
    if(chip->v_regs[reg_x] == constant)
        chip->PC += 2;
}

void op_skip_if_not_equal_rc(chip8* chip, byte reg_x, byte constant)
{
    if(chip->v_regs[reg_x] != constant)
        chip->PC += 2;
}

void op_skip_if_equal_rr(chip8* chip, byte reg_x, byte reg_y)
{
    if(chip->v_regs[reg_x] == chip->v_regs[reg_y])
        chip->PC += 2;
}

void op_load_reg_from_constant(chip8* chip, byte reg_x, byte constant)
{
    chip->v_regs[reg_x] = constant;
}

void op_add_reg_to_constant(chip8* chip, byte reg_x, byte constant)
{
    chip->v_regs[reg_x] += constant;
}

void op_load_reg_from_reg(chip8* chip, byte reg_x, byte reg_y)
{
    chip->v_regs[reg_x] = chip->v_regs[reg_y];
}

void op_or_reg_from_reg(chip8* chip, byte reg_x, byte reg_y)
{
    chip->v_regs[reg_x] = chip->v_regs[reg_x] | chip->v_regs[reg_y];
}

void op_and_reg_from_reg(chip8* chip, byte reg_x, byte reg_y)
{
    chip->v_regs[reg_x] = chip->v_regs[reg_x] & chip->v_regs[reg_y];
}

void op_xor_reg_from_reg(chip8* chip, byte reg_x, byte reg_y)
{
    chip->v_regs[reg_x] = chip->v_regs[reg_x] ^ chip->v_regs[reg_y];
}

void op_add_reg_to_reg(chip8* chip, byte reg_x, byte reg_y)
{
    word result = (word)(chip->v_regs[reg_x] + chip->v_regs[reg_y]);
    if (result > 255)
        chip->v_regs[REG_VF] = 1;
    else
        chip->v_regs[REG_VF] = 0;
    chip->v_regs[reg_x] = (byte)(result & 0x00FF);
}

void op_sub_reg_to_reg(chip8* chip, byte reg_x, byte reg_y)
{

    if (chip->v_regs[reg_x] < chip->v_regs[reg_y])
        chip->v_regs[REG_VF]  = 0;
    else
        chip->v_regs[REG_VF]  = 1;
    chip->v_regs[reg_x] = chip->v_regs[reg_x] - chip->v_regs[reg_y];
}

void op_shift_right_reg(chip8* chip, byte reg_x)
{
    if(chip->v_regs[reg_x] & 0x01)
        chip->v_regs[REG_VF]  = 1;
    else
        chip->v_regs[REG_VF]  = 0;
    chip->v_regs[reg_x] = chip->v_regs[reg_x] >> 2;
}

void op_subn_reg_to_reg(chip8* chip, byte reg_x, byte reg_y)
{

    if (chip->v_regs[reg_x] > chip->v_regs[reg_y])
        chip->v_regs[REG_VF]  = 0;
    else
        chip->v_regs[REG_VF]  = 1;
    chip->v_regs[reg_x] = chip->v_regs[reg_y] - chip->v_regs[reg_x];
}

void op_shift_left_reg(chip8* chip, byte reg_x)
{
    if(chip->v_regs[reg_x] & 0x80)
        chip->v_regs[REG_VF]  = 1;
    else
        chip->v_regs[REG_VF]  = 0;
    chip->v_regs[reg_x] = chip->v_regs[reg_x] << 1;
}

void op_skip_if_reg_not_equal_rr(chip8* chip, byte reg_x, byte reg_y)
{
    if(chip->v_regs[reg_x] != chip->v_regs[reg_y])
        chip->PC += 2;
}

void op_load_addr_into_i(chip8* chip, addr address)
{
    chip->I = address;
}

void op_jmp_reg_0_offset(chip8* chip, addr address)
{
    chip->PC = address + chip->v_regs[0];
}

void op_random_number(chip8* chip, byte reg_x, byte constant)
{
    byte random = rand() % 255;
    chip->v_regs[reg_x] = random & constant;
}

void op_draw_to_screen(chip8* chip, byte reg_x, byte reg_y, byte nibble)
{
    byte x_pos = chip->v_regs[reg_x] % SCREEN_WIDTH;
    byte y_pos = chip->v_regs[reg_y] % SCREEN_HEIGHT;
    chip->v_regs[REG_VF] = 0;

    for(int i = 0; i < nibble; i++)
    {
        byte pixels = chip->memory[chip->I + i];

        for(int p = 0; p < 8; p++)
        {
            byte x_pos_offset = (x_pos + p) % SCREEN_WIDTH;
            byte y_pos_offset = (y_pos + i) % SCREEN_HEIGHT;

            byte old_pixel = chip->screen[x_pos_offset][y_pos_offset];

            if((old_pixel ^ (pixels & (0x80>>p))!= 0))
            {
                chip->screen[x_pos_offset][y_pos_offset] = 1;
            }
            else
            {
                if(old_pixel)
                    chip->v_regs[REG_VF] = 1;
                chip->screen[x_pos_offset][y_pos_offset] = 0;
            }
        }
    }

    chip->screen_changed = 1;
}

void op_skip_key_pressed_reg(chip8* chip, byte reg_x)
{
    if(chip->keys[chip->v_regs[reg_x]] == 1)
        chip->PC+=2;
}

void op_skip_key_not_pressed_reg(chip8* chip, byte reg_x)
{
    if(chip->keys[chip->v_regs[reg_x]] != 1)
        chip->PC+=2;
}

void op_load_reg_from_delay_timer(chip8* chip, byte reg_x)
{
    chip->v_regs[reg_x] = chip->DT;
}


void op_load_keypress_into_reg(chip8* chip, byte reg_x)
{
    for(int i = 0; i < 16; i++)
        if(chip->keys[i])
        {
            chip->v_regs[reg_x] = i;
            return;
        }
    chip->PC -= 2;
}

void op_set_delay_timer_from_reg(chip8* chip, byte reg_x)
{
    chip->DT = chip->v_regs[reg_x];
}

void op_set_sound_timer_from_reg(chip8* chip, byte reg_x)
{
    chip->ST = chip->v_regs[reg_x];
}

void op_add_i_to_reg(chip8* chip, byte reg_x)
{
    chip->I = chip->I + chip->v_regs[reg_x];
}

void op_set_i_to_digit_reg(chip8* chip, byte reg_x)
{
    chip->I = chip->v_regs[reg_x] * 5;
}

void op_bcd(chip8* chip, byte reg_x)
{
    chip->memory[chip->I] = chip->v_regs[reg_x] / 100;
    chip->memory[chip->I+1] = (chip->v_regs[reg_x] % 100) / 10;
    chip->memory[chip->I+2] = chip->v_regs[reg_x] % 10;
}

void op_save_regs_at_i(chip8* chip, byte reg_x)
{
    for(int i = 0; i < reg_x; i++)
    {
        chip->memory[chip->I + i] = chip->v_regs[i];
    }
}

void op_load_regs_at_i(chip8* chip, byte reg_x)
{
    for(int i = 0; i < reg_x; i++)
    {
        chip->v_regs[i] = chip->memory[chip->I + i];
    }
}

void chip8_step(chip8* chip)
{   
    opcode op;
    byte upper = chip->memory[chip->PC];
    byte lower = chip->memory[chip->PC+1];
    chip->PC += 2;
    op = parse_opcode(upper, lower);
    switch(op)
    {  
        case SYS_A:
            op_sys(chip);
            return;
        case CLS:
            op_cls(chip);
            return;
        case RET:
            op_ret(chip);
            return;
        case JP_A:
            op_jmp(chip, get_nnn(upper, lower));
            return;
        case CALL_A:
            op_call(chip, get_nnn(upper, lower));
            return;
        case SE_RC:
            op_skip_if_equal_rc(chip, get_v_reg_x(upper), lower);
            return;
        case SNE_RC:
            op_skip_if_not_equal_rc(chip, get_v_reg_x(upper), lower);
            return;
        case SE_RR:
            op_skip_if_equal_rr(chip, get_v_reg_x(upper), get_v_reg_y(lower));
            return;
        case LD_RC:
            op_load_reg_from_constant(chip, get_v_reg_x(upper), lower);
            return;
        case ADD_RC:
            op_add_reg_to_constant(chip, get_v_reg_x(upper), lower);
            return;
        case LD_RR:
            op_load_reg_from_reg(chip, get_v_reg_x(upper), get_v_reg_y(lower));
            return;
        case OR_RR:
            op_or_reg_from_reg(chip, get_v_reg_x(upper), get_v_reg_y(lower));
            return;
        case AND_RR:
            op_and_reg_from_reg(chip, get_v_reg_x(upper), get_v_reg_y(lower));
            return;
        case XOR_RR:
            op_xor_reg_from_reg(chip, get_v_reg_x(upper), get_v_reg_y(lower));
            return;
        case ADD_RR:
            op_add_reg_to_reg(chip, get_v_reg_x(upper), get_v_reg_y(lower));
            return;
        case SUB_RR:
            op_sub_reg_to_reg(chip, get_v_reg_x(upper), get_v_reg_y(lower));
            return;
        case SHR_RR:
            op_shift_right_reg(chip, get_v_reg_x(upper));
            return;
        case SUBN_RR:
            op_subn_reg_to_reg(chip, get_v_reg_x(upper), get_v_reg_y(lower));
            return;
        case SHL_RR:
            op_shift_right_reg(chip, get_v_reg_x(upper));
            return;
        case SNE_RR:
            op_skip_if_reg_not_equal_rr(chip, get_v_reg_x(upper), get_v_reg_y(lower));
            return;
        case LD_IA:
            op_load_addr_into_i(chip, get_nnn(upper, lower));
            return;
        case JP_V0A:
            op_jmp_reg_0_offset(chip, get_nnn(upper, lower));
            return;
        case RND_RC:
            op_random_number(chip, get_v_reg_x(upper), lower);
            return;
        case DRW_RRC:
            op_draw_to_screen(chip, get_v_reg_x(upper), get_v_reg_y(lower), get_nibble(lower));
            return;
        case SKP_R:
            op_skip_key_pressed_reg(chip, get_v_reg_x(upper));
            return;
        case SKNP_R:
            op_skip_key_not_pressed_reg(chip, get_v_reg_x(upper));
            return;
        case LD_RD:
            op_load_reg_from_delay_timer(chip, get_v_reg_x(upper));
            return;
        case LD_RK:
            op_load_keypress_into_reg(chip, get_v_reg_x(upper));
            return;
        case LD_DR:
            op_set_delay_timer_from_reg(chip, get_v_reg_x(upper));
            return;
        case LD_SR:
            op_set_sound_timer_from_reg(chip, get_v_reg_x(upper));
            return;
        case ADD_IR:
            op_add_i_to_reg(chip, get_v_reg_x(upper));
            return;
        case LD_FR:
            op_set_i_to_digit_reg(chip, get_v_reg_x(upper));
            return;
        case LD_BR:
            op_bcd(chip, get_v_reg_x(upper));
            return;
        case LD_IAR:
            op_save_regs_at_i(chip, get_v_reg_x(upper));
            return;
        case LD_RAI:
            op_load_regs_at_i(chip, get_v_reg_x(upper));
            return;
        case EXIT:
        default:
            return;
    }
}

void chip8_tick_delay_timer(chip8* chip)
{
    if(chip->DT > 0)
        chip->DT--;
}

void chip8_tick_sound_timer(chip8* chip)
{
    if(chip->ST > 0)
        chip->ST--;
}


void chip8_init(chip8* chip, byte* program, int program_size)
{
    memset(chip->memory, 0, MEMORY_SIZE);
    memset(chip->stack, 0, STACK_SIZE);
    memset(chip->v_regs, 0, STACK_SIZE);
    memset(chip->screen, 0, SCREEN_WIDTH * SCREEN_HEIGHT);
    memset(chip->keys, 0, 16);
    chip->PC = 0x200;
    chip->SP = 0;
    chip->DT = 0;
    chip->ST = 0;

    memcpy(&(chip->memory[0x200]), program, program_size);

    srand(time(NULL));
}