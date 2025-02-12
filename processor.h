#ifndef __PROCESSOR_H__
#define __PROCESSOR_H__

#include "chip8.h"
#include "chip_constants.h"


void chip8_step(chip8*);
void chip8_init(chip8*, byte*, int);
void chip8_tick_delay_timer(chip8* chip);
void chip8_tick_sound_timer(chip8* chip);

// Operations for testing. Not to be used outside of processing core code.

void op_sys(chip8 *chip);
void op_cls(chip8 *chip);
void op_jmp(chip8 *chip, addr jmp_to_addr);
void op_call(chip8 *chip, addr jump_to_addr);
void op_skip_if_equal_rc(chip8 *chip, byte reg_x, byte constant);
void op_skip_if_not_equal_rc(chip8 *chip, byte reg_x, byte constant);
void op_skip_if_equal_rr(chip8 *chip, byte reg_x, byte reg_y);
void op_load_reg_from_constant(chip8 *chip, byte reg_x, byte constant);
void op_add_reg_to_constant(chip8 *chip, byte reg_x, byte constant);
void op_load_reg_from_reg(chip8 *chip, byte reg_x, byte reg_y);
void op_or_reg_from_reg(chip8 *chip, byte reg_x, byte reg_y);
void op_and_reg_from_reg(chip8 *chip, byte reg_x, byte reg_y);
void op_xor_reg_from_reg(chip8 *chip, byte reg_x, byte reg_y);
void op_add_reg_to_reg(chip8 *chip, byte reg_x, byte reg_y);
void op_sub_reg_to_reg(chip8 *chip, byte reg_x, byte reg_y);
void op_shift_right_reg(chip8 *chip, byte reg_x, byte reg_y);
void op_subn_reg_to_reg(chip8 *chip, byte reg_x, byte reg_y);
void op_shift_left_reg(chip8 *chip, byte reg_x, byte reg_y);
void op_skip_if_reg_not_equal_rr(chip8 *chip, byte reg_x, byte reg_y);
void op_load_addr_into_i(chip8 *chip, addr address);
void op_jmp_reg_0_offset(chip8 *chip, addr address);
void op_random_number(chip8 *chip, byte reg_x, byte constant);
void op_draw_to_screen(chip8 *chip, byte reg_x, byte reg_y, byte nibble);
void op_skip_key_pressed_reg(chip8 *chip, byte reg_x);
void op_skip_key_not_pressed_reg(chip8 *chip, byte reg_x);
void op_load_reg_from_delay_timer(chip8 *chip, byte reg_x);
void op_load_keypress_into_reg(chip8 *chip, byte reg_x);
void op_set_delay_timer_from_reg(chip8 *chip, byte reg_x);
void op_set_sound_timer_from_reg(chip8 *chip, byte reg_x);
void op_add_i_to_reg(chip8 *chip, byte reg_x);
void op_set_i_to_digit_reg(chip8 *chip, byte reg_x);
void op_bcd(chip8 *chip, byte reg_x);
void op_save_regs_at_i(chip8 *chip, byte reg_x);
void op_load_regs_at_i(chip8 *chip, byte reg_x);


#endif