#include "parse_instruction.h"
#include "chip_constants.h"

opcode parse_opcode(byte upper, byte lower)
{
    byte upper_opcode = upper >> 4;
    byte lower_opcode = lower;
    byte lower_nibble_opcode = lower & 0x0F;

    switch (upper_opcode)
    {
    case 0:
        if(lower_opcode == 0xE0)  
            return CLS;
        if(lower_opcode == 0xEE)
            return RET;
    case 0x01:
        return JP_A;
    case 0x02:
        return CALL_A;
    case 0x03:
        return SE_RC;
    case 0x04:
        return SNE_RC;
    case 0x05:
        if (lower_nibble_opcode == 0)
            return SE_RR;
    case 0x06:
        return LD_RC;
    case 0x07:
        return ADD_RC;
    case 0x08:
        if(lower_nibble_opcode == 0x00)
            return LD_RR;
        if(lower_nibble_opcode == 0x01)
            return OR_RR;
        if(lower_nibble_opcode == 0x02)
            return AND_RR;
        if(lower_nibble_opcode == 0x03)
            return XOR_RR;
        if(lower_nibble_opcode == 0x04)
            return ADD_RR;
        if(lower_nibble_opcode == 0x05)
            return SUB_RR;
        if(lower_nibble_opcode == 0x06)
            return SHR_RR;
        if(lower_nibble_opcode == 0x07)
            return SUBN_RR;
        if(lower_nibble_opcode == 0x0E)
            return SHL_RR;
    case 0x09:
        if(lower_nibble_opcode == 0x00)
            return SNE_RR;
    case 0x0A:
        return LD_IA;
    case 0x0B:
        return JP_V0A;
    case 0x0C:
        return RND_RC;
    case 0x0D:
        return DRW_RRC;
    case 0x0E:
        if(lower_opcode == 0x9E)
            return SKP_R;
        if(lower_opcode == 0xA1)
            return SKNP_R;
    case 0x0F:
        if(lower_opcode == 0x07)
            return LD_RD;
        if(lower_opcode == 0x0A)
            return LD_RK;
        if(lower_opcode == 0x15)
            return LD_DR;
        if(lower_opcode == 0x1E)
            return ADD_IR;
        if(lower_opcode == 0x29)
            return LD_FR;
        if(lower_opcode == 0x33)
            return LD_BR;
        if(lower_opcode == 0x55)
            return LD_IAR;
        if(lower_opcode == 0x65)
            return LD_RAI;        
    default:
        return EXIT;
    }
}

byte get_v_reg_x(byte upper)
{
    return upper & 0x0F;
}

byte get_v_reg_y(byte lower)
{
    return (lower & 0xF0) >> 4;
}

word get_nnn(byte upper, byte lower)
{   
    word nnn = (upper & 0x0F);
    nnn = nnn << 8;
    nnn += lower;
    return nnn;
}

byte get_nibble(byte lower)
{
    return lower & 0x0F;
}
