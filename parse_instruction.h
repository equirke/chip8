#ifndef __PARSE_INSTRUCTION__
#define __PARSE_INSTRUCTION__

#include "chip_constants.h"

opcode parse_opcode(byte, byte);
byte get_v_reg_x(byte);
byte get_v_reg_y(byte);
word get_nnn(byte,byte);
byte get_nibble(byte);

#endif