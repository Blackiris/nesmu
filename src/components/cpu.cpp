#include "cpu.h"

#include <iostream>
#include <bitset>

#define REGISTER_MASK_C 0b00000001
#define REGISTER_MASK_Z 0b00000010
#define REGISTER_MASK_I 0b00000100
#define REGISTER_MASK_D 0b00001000
#define REGISTER_MASK_V 0b01000000
#define REGISTER_MASK_N 0b10000000

const std::map<unsigned char, std::string> CPU::opcode_to_inst = {
    {0x05, "ORA Zero Page"},
    {0x06, "ASL Zero Page"},
    {0x09, "ORA Immediate"},
    {0x0a, "ASL Accumulator"},
    {0x10, "BPL"},
    {0x17, "SLO Zero Page X (non official)"},
    {0x18, "CLC"},
    {0x20, "JSR"},
    {0x24, "BIT Zero Page"},
    {0x25, "AND Zero Page"},
    {0x29, "AND Immediate"},
    {0x2a, "ROL Accumulator"},
    {0x2c, "BIT Absolute"},
    {0x30, "BMI"},
    {0x38, "SEC"},
    {0x3d, "AND Absolute X"},
    {0x40, "RTI"},
    {0x45, "EOR Zero Page"},
    {0x46, "LSR Zero Page"},
    {0x48, "PHA"},
    {0x4a, "LSR Accumulator"},
    {0x4c, "JMP Absolute"},
    {0x60, "RTS"},
    {0x65, "ADC Zero Page"},
    {0x66, "ROR Zero Page"},
    {0x68, "PLA"},
    {0x69, "ADC Immediate"},
    {0x6c, "JMP Indirect"},
    {0x6d, "ADC Absolute"},
    {0x78, "SEI"},
    {0x79, "ADC Absolute Y"},
    {0x7e, "ROR Absolute X"},
    {0x82, "DOP (non official)"},
    {0x84, "STY Zero Page"},
    {0x85, "STA Zero Page"},
    {0x86, "STX Zero Page"},
    {0x88, "DEY"},
    {0x8a, "TXA"},
    {0x8c, "STY Absolute"},
    {0x8d, "STA Absolute"},
    {0x8e, "STX Absolute"},
    {0x8f, "AAX Absolute (non official)"},
    {0x90, "BCC"},
    {0x91, "STA Indirect Indexed Y"},
    {0x95, "STA Zero Page X"},
    {0x98, "TYA"},
    {0x99, "STA Absolute Y"},
    {0x9a, "TXS"},
    {0x9d, "STA Absolute X"},
    {0xa0, "LDY Immediate"},
    {0xa1, "LDA Indexed Indirect X"},
    {0xa2, "LDX Immediate"},
    {0xa4, "LDY Zero Page"},
    {0xa5, "LDA Zero Page"},
    {0xa6, "LDX Zero Page"},
    {0xa8, "TAY"},
    {0xa9, "LDA Immediate"},
    {0xaa, "TAX"},
    {0xac, "LDY Absolute"},
    {0xad, "LDA Absolute"},
    {0xae, "LDX Absolute"},
    {0xb9, "LDA Absolute Y"},
    {0xb0, "BCS"},
    {0xb1, "LDA Indirect Indexed Y"},
    {0xb5, "LDA Zero Page X"},
    {0xbc, "LDY Absolute X"},
    {0xbd, "LDA Absolute X"},
    {0xbe, "LDX Absolute Y"},
    {0xc0, "CPY Immediate"},
    {0xc5, "CMP Zero Page"},
    {0xc6, "DEC Zero Page"},
    {0xc8, "INY"},
    {0xc9, "CMP Immediate"},
    {0xca, "DEX"},
    {0xcd, "CMP Absolute"},
    {0xce, "DEC Absolute"},
    {0xd0, "BNE"},
    {0xd8, "CLD"},
    {0xd9, "CMP Absolute Y"},
    {0xde, "DEC Absolute X"},
    {0xe0, "CPX Immediate"},
    {0xe4, "CPX Zero Page"},
    {0xe6, "INC Zero Page"},
    {0xe8, "INX"},
    {0xe9, "SBC Immediate"},
    {0xee, "INC Absolute"},
    {0xf0, "BEQ"},
    {0xf8, "SED"},
    {0xf9, "SBC Absolute Y"}
};

CPU::CPU(IMemory& cpu_mem_map): m_mem_map(cpu_mem_map), nmi(0) {

}

void CPU::init(){
    reg_x = 0;
    reg_a = 0;
    reg_pc = get_address_from_memory(0xfffc);
}

void CPU::set_nmi() {
    nmi = true;
}

bool CPU::exec_cycle(int nb_cycles) {
    if (nmi) {
        nmi = false;
        push_value_to_stack(reg_pc);
        push_byte_to_stack(reg_p);
        reg_pc = get_address_from_memory(0xfffa);
    }

    int total_cycles = 0;
    while (total_cycles < nb_cycles) {
        unsigned char opcode = m_mem_map.get_value_at(reg_pc);
        short cycle_op = apply_op_code(opcode);
        total_cycles += cycle_op;
    }

    return total_cycles >= nb_cycles;
}

uint16_t CPU::get_address_from_memory(const uint16_t& address_1st_byte) {
    unsigned char byte1 = m_mem_map.get_value_at(address_1st_byte);
    unsigned char byte2 = m_mem_map.get_value_at(address_1st_byte + 1);

    return convert_2_bytes_to_16bits(byte1, byte2);
}

uint16_t CPU::convert_2_bytes_to_16bits(const unsigned char& byte1, const unsigned char& byte2) {
    return (byte2 << 8) | (0x00FF & byte1);
}

void CPU::set_status_register(char status, bool enable) {
    if (enable) {
        enable_status_register(status);
    } else {
        clear_status_register(status);
    }
}

void CPU::enable_status_register(char status) {
    switch(status) {
    case 'C':
        reg_p |= REGISTER_MASK_C;
        break;
    case 'Z':
        reg_p |= REGISTER_MASK_Z;
        break;
    case 'I':
        reg_p |= REGISTER_MASK_I;
        break;
    case 'D':
        reg_p |= REGISTER_MASK_D;
        break;
    case 'V':
        reg_p |= REGISTER_MASK_V;
        break;
    case 'N':
        reg_p |= REGISTER_MASK_N;
        break;
    }
}

void CPU::clear_status_register(char status) {
    switch(status) {
    case 'C':
        reg_p &= ~REGISTER_MASK_C;
        break;
    case 'Z':
        reg_p &= ~REGISTER_MASK_Z;
        break;
    case 'I':
        reg_p &= ~REGISTER_MASK_I;
        break;
    case 'D':
        reg_p &= ~REGISTER_MASK_D;
        break;
    case 'V':
        reg_p &= ~REGISTER_MASK_V;
        break;
    case 'N':
        reg_p &= ~REGISTER_MASK_N;
        break;
    }
}

bool CPU::get_status_register(char status) {
    unsigned char res;
    switch(status) {
    case 'C':
        res = reg_p & REGISTER_MASK_C;
        break;
    case 'Z':
        res = reg_p & REGISTER_MASK_Z;
        break;
    case 'I':
        res = reg_p & REGISTER_MASK_I;
        break;
    case 'D':
        res = reg_p & REGISTER_MASK_D;
        break;
    case 'V':
        res = reg_p & REGISTER_MASK_V;
        break;
    case 'N':
        res = reg_p & REGISTER_MASK_N;
        break;
    }

    return res > 0;
}

short CPU::apply_op_code(const unsigned char& opcode) {
    short cycle = 0;
    uint16_t addr;
    unsigned char addr8;
    unsigned char value, res;
    uint16_t ori_reg_pc = reg_pc;

    switch(opcode) {
    case 0x05: //ORA Zero Page
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        value = m_mem_map.get_value_at(addr8);
        reg_a |= value;
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 3;
        break;
    case 0x06: //ASL Zero Page
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        value = m_mem_map.get_value_at(addr8);
        shift_left(value);
        m_mem_map.set_value_at(addr8, value);
        reg_pc += 2;
        cycle = 5;
        break;
    case 0x09: //ORA Immediate
        value = m_mem_map.get_value_at(reg_pc+1);
        reg_a |= value;
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 2;
        break;
    case 0x0a: //ASL Accumulator
        shift_left(reg_a);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0x10: //BPL
        cycle = jump_relative(!get_status_register('N'));
        break;
    case 0x17: //SLO Zero Page X (unofficial)
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        value = m_mem_map.get_value_at(addr8 + reg_x);
        shift_left(value);
        reg_a |= value;
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 6;
        break;
    case 0x18: //CLC
        set_status_register('C', false);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0x20: //JSR
        push_value_to_stack(reg_pc + 2);
        reg_pc = get_address_from_memory(reg_pc+1);
        cycle = 6;
        break;
    case 0x24: //BIT Zero Page
        addr = m_mem_map.get_value_at(reg_pc+1);
        value = m_mem_map.get_value_at(addr);
        res = reg_a & value;
        set_status_register('V', value & 64);
        set_status_register('N', value & 128);
        set_status_register('Z', res == 0);

        reg_pc += 2;
        cycle = 3;
        break;
    case 0x25: //AND Zero Page
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        value = m_mem_map.get_value_at(addr8);
        reg_a &= value;
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 3;
        break;
    case 0x29: //AND Immediate
        value = m_mem_map.get_value_at(reg_pc+1);
        reg_a &= value;
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 2;
        break;
    case 0x2a: //ROL Accumulator
        set_status_register('C', reg_a & 0b10000000);
        reg_a <<= 1;
        set_zero_negative_flags(reg_a);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0x2c: //BIT Absolute
        addr = get_address_from_memory(reg_pc+1);
        value = m_mem_map.get_value_at(addr);
        res = reg_a & value;
        set_status_register('V', value & 64);
        set_status_register('N', value & 128);
        set_status_register('Z', res == 0);

        reg_pc += 3;
        cycle = 4;
        break;
    case 0x30: //BMI
        cycle = jump_relative(get_status_register('N'));
        break;
    case 0x38: //SEC
        set_status_register('C', true);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0x3d: //AND Absolute X
        addr = get_address_from_memory(reg_pc+1);
        reg_a &= m_mem_map.get_value_at(addr + reg_x);
        set_zero_negative_flags(reg_a);
        reg_pc += 3;
        cycle = 4; //FIXME 5 if page crossed
        break;
    case 0x40: //RTI
        reg_p = pull_byte_from_stack();
        reg_pc = pull_value_from_stack();
        cycle = 6;
        break;
    case 0x45: //EOR Zero Page
        reg_a ^= m_mem_map.get_value_at(reg_pc);
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 3;
        break;
    case 0x46: //LSR Zero Page
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        value = m_mem_map.get_value_at(addr8);
        shift_right(value);
        m_mem_map.set_value_at(addr8, value);
        reg_pc += 2;
        cycle = 5;
        break;
    case 0x48: //PHA
        push_byte_to_stack(reg_a);
        reg_pc += 1;
        cycle = 3;
        break;
    case 0x4a: //LSR Accumulator
        shift_right(reg_a);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0x4c: //JMP Absolute
        reg_pc = get_address_from_memory(reg_pc+1);
        cycle = 3;
        break;
    case 0x60: //RTS
        reg_pc = pull_value_from_stack();
        reg_pc += 1;
        cycle = 6;
        break;
    case 0x65: //ADC Zero Page
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        value = m_mem_map.get_value_at(addr8);
        add_with_carry(value);
        reg_pc += 2;
        cycle = 3;
        break;
    case 0x66: //ROR Zero Page
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        value = m_mem_map.get_value_at(addr8);
        rotate_right(value);
        m_mem_map.set_value_at(addr8, value);
        reg_pc += 2;
        cycle = 5;
        break;
    case 0x68: //PLA
        reg_a = pull_byte_from_stack();
        set_zero_negative_flags(reg_a);
        reg_pc += 1;
        cycle = 4;
        break;
    case 0x69: //ADC Immediate
        value = m_mem_map.get_value_at(reg_pc+1);
        add_with_carry(value);
        reg_pc += 2;
        cycle = 2;
        break;
    case 0x6c: //JMP Indirect
        addr = get_address_from_memory(reg_pc+1);
        reg_pc = get_address_from_memory(addr);
        cycle = 5;
        break;
    case 0x6d: //ADC Absolute
        addr = get_address_from_memory(reg_pc+1);
        value = m_mem_map.get_value_at(addr);
        add_with_carry(value);
        reg_pc += 3;
        cycle = 4;
        break;
    case 0x78: //SEI
        enable_status_register('I');
        reg_pc += 1;
        cycle = 2;
        break;
    case 0x79: //ADC Absolute Y
        addr = get_address_from_memory(reg_pc+1);
        value = m_mem_map.get_value_at(addr+reg_y);
        add_with_carry(value);
        reg_pc += 3;
        cycle = 4; //FIXME 5 if page crossed
        break;
    case 0x7e: //ROR Absolute X
        addr = get_address_from_memory(reg_pc+1);
        value = m_mem_map.get_value_at(addr + reg_x);
        rotate_right(value);
        m_mem_map.set_value_at(addr + reg_x, value);
        reg_pc += 3;
        cycle = 7;
        break;
    case 0x82: //DOP
        reg_pc += 2;
        cycle = 2;
        break;
    case 0x84: //STY Zero Page
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        m_mem_map.set_value_at(addr8, reg_y);
        reg_pc += 2;
        cycle = 3;
        break;
    case 0x85: //STA Zero Page
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        m_mem_map.set_value_at(addr8, reg_a);
        reg_pc += 2;
        cycle = 3;
        break;
    case 0x86: //STX Zero Page
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        m_mem_map.set_value_at(addr8, reg_x);
        reg_pc += 2;
        cycle = 3;
        break;
    case 0x88: //DEY
        reg_y -= 1;
        set_zero_negative_flags(reg_y);

        reg_pc += 1;
        cycle = 2;
        break;
    case 0x8a: //TXA
        reg_a = reg_x;
        set_zero_negative_flags(reg_a);

        reg_pc += 1;
        cycle = 2;
        break;
    case 0x8c: //STY Absolute
        addr = get_address_from_memory(reg_pc+1);
        m_mem_map.set_value_at(addr, reg_y);
        reg_pc += 3;
        cycle = 4;
        break;
    case 0x8d: //STA Absolute
        addr = get_address_from_memory(reg_pc+1);
        m_mem_map.set_value_at(addr, reg_a);
        reg_pc += 3;
        cycle = 4;
        break;
    case 0x8e: //STX Absolute
        addr = get_address_from_memory(reg_pc+1);
        m_mem_map.set_value_at(addr, reg_x);
        reg_pc += 3;
        cycle = 4;
        break;
    case 0x8f: //AAX Absolute
        addr = get_address_from_memory(reg_pc+1);
        m_mem_map.set_value_at(addr, reg_a & reg_x);
        reg_pc += 3;
        cycle = 4;
        break;
    case 0x90: //BCC
        cycle = jump_relative(!get_status_register('C'));
        break;
    case 0x91: //STA Indirect Indexed Y
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        addr = get_address_from_memory(addr8);
        m_mem_map.set_value_at(addr+reg_y, reg_a);
        reg_pc += 2;
        cycle = 6;
        break;
    case 0x95: //STA Zero Page X
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        m_mem_map.set_value_at(addr8+reg_x, reg_a);
        reg_pc += 2;
        cycle = 4;
        break;
    case 0x98: //TYA
        reg_a = reg_y;
        set_zero_negative_flags(reg_a);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0x99: //STA Absolute Y
        addr = get_address_from_memory(reg_pc+1);
        m_mem_map.set_value_at(addr + reg_y, reg_a);
        reg_pc += 3;
        cycle = 5;
        break;
    case 0x9a: //TXS
        reg_sp = reg_x;
        cycle = 2;
        reg_pc += 1;
        break;
    case 0x9d: //STA Absolute X
        addr = get_address_from_memory(reg_pc+1);
        m_mem_map.set_value_at(addr + reg_x, reg_a);
        reg_pc += 3;
        cycle = 5;
        break;
    case 0xa0: //LDY Immediate
        reg_y = m_mem_map.get_value_at(reg_pc+1);
        set_zero_negative_flags(reg_y);
        reg_pc += 2;
        cycle = 2;
        break;
    case 0xa1: //LDA Indexed Indirect X
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        addr = get_address_from_memory(addr8+reg_x);
        reg_a = m_mem_map.get_value_at(addr);
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 6;
        break;
    case 0xa2: //LDX Immediate
        reg_x = m_mem_map.get_value_at(reg_pc+1);
        set_zero_negative_flags(reg_x);
        reg_pc += 2;
        cycle = 2;
        break;
    case 0xa4: //LDY Zero Page
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        reg_y = m_mem_map.get_value_at(addr8);
        set_zero_negative_flags(reg_y);
        reg_pc += 2;
        cycle = 3;
        break;
    case 0xa5: //LDA Zero Page
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        reg_a = m_mem_map.get_value_at(addr8);
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 3;
        break;
    case 0xa6: //LDX Zero Page
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        reg_x = m_mem_map.get_value_at(addr8);
        set_zero_negative_flags(reg_x);
        reg_pc += 2;
        cycle = 3;
        break;
    case 0xa8: //TAY
        reg_y = reg_a;
        set_zero_negative_flags(reg_y);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0xa9: //LDA Immediate
        reg_a = m_mem_map.get_value_at(reg_pc+1);
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 2;
        break;
    case 0xaa: //TAX
        reg_x = reg_a;
        set_zero_negative_flags(reg_x);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0xac: //LDY Absolute
        addr = get_address_from_memory(reg_pc+1);
        reg_y = m_mem_map.get_value_at(addr);
        set_zero_negative_flags(reg_y);
        reg_pc += 3;
        cycle = 4;
        break;
    case 0xad: //LDA Absolute
        addr = get_address_from_memory(reg_pc+1);
        reg_a = m_mem_map.get_value_at(addr);
        set_zero_negative_flags(reg_a);
        reg_pc += 3;
        cycle = 4;
        break;
    case 0xae: //LDX Absolute
        addr = get_address_from_memory(reg_pc+1);
        reg_x = m_mem_map.get_value_at(addr);
        set_zero_negative_flags(reg_x);
        reg_pc += 3;
        cycle = 4;
        break;
    case 0xb0: //BCS
        cycle = jump_relative(get_status_register('C'));
        break;
    case 0xb1: //LDA Indirect Indexed Y
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        addr = get_address_from_memory(addr8);
        reg_a = m_mem_map.get_value_at(addr+reg_y);
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 5; //FIXME 6 if paged crossed
        break;
    case 0xb5: //LDA Zero Page X
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        reg_a = m_mem_map.get_value_at(addr8 + reg_x);
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 4;
        break;
    case 0xb9: //LDA Absolute Y
        addr = get_address_from_memory(reg_pc+1);
        reg_a = m_mem_map.get_value_at(addr + reg_y);
        set_zero_negative_flags(reg_a);
        reg_pc += 3;
        cycle = 4; //FIXME 5 if paged crossed
        break;
    case 0xbc: //LDY Absolute X
        addr = get_address_from_memory(reg_pc+1);
        reg_y = m_mem_map.get_value_at(addr + reg_x);
        set_zero_negative_flags(reg_y);
        reg_pc += 3;
        cycle = 4; //FIXME 5 if paged crossed
        break;
    case 0xbd: //LDA Absolute X
        addr = get_address_from_memory(reg_pc+1);
        reg_a = m_mem_map.get_value_at(addr + reg_x);
        set_zero_negative_flags(reg_a);
        reg_pc += 3;
        cycle = 4; //FIXME or 5 if page crossed
        break;
    case 0xbe: //LDX Absolute Y
        addr = get_address_from_memory(reg_pc+1);
        reg_x = m_mem_map.get_value_at(addr + reg_y);
        set_zero_negative_flags(reg_x);
        reg_pc += 3;
        cycle = 4; //FIXME or 5 if page crossed
        break;
    case 0xc0: //CPY Immediate
        cycle = cmp_immediate(reg_y);
        reg_pc += 2;
        break;
    case 0xc5: //CMP Zero Page
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        value = m_mem_map.get_value_at(addr8);
        cmp(reg_a, value);
        reg_pc += 2;
        cycle = 3;
        break;
    case 0xc6: //DEC Zero Page
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        value = m_mem_map.get_value_at(addr8);
        value--;
        m_mem_map.set_value_at(addr8, value);
        set_zero_negative_flags(value);
        reg_pc += 2;
        cycle = 5;
        break;
    case 0xc8: //INY
        reg_y++;
        set_zero_negative_flags(reg_y);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0xc9: //CMP Immediate
        cycle = cmp_immediate(reg_a);
        reg_pc += 2;
        break;
    case 0xca: //DEX
        reg_x--;
        set_zero_negative_flags(reg_x);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0xcd: //CMP Absolute
        addr = get_address_from_memory(reg_pc+1);
        value = m_mem_map.get_value_at(addr);
        cmp(reg_a, value);
        reg_pc += 4;
        cycle = 4;
        break;
    case 0xce: //DEC Absolute
        addr = get_address_from_memory(reg_pc+1);
        value = m_mem_map.get_value_at(addr);
        value--;
        m_mem_map.set_value_at(addr, value);
        set_zero_negative_flags(value);
        reg_pc += 3;
        cycle = 6;
        break;
    case 0xd0: //BNE
        cycle = jump_relative(!get_status_register('Z'));
        break;
    case 0xd8: //CLD
        clear_status_register('D');
        reg_pc += 1;
        cycle = 2;
        break;
    case 0xd9: //CMP Absolute Y
        addr = get_address_from_memory(reg_pc+1);
        value = m_mem_map.get_value_at(addr + reg_y);
        cmp(reg_a, value);
        reg_pc += 3;
        cycle = 4; //5 if page crossed
        break;
    case 0xde: //DEC Absolute X
        addr = get_address_from_memory(reg_pc+1);
        value = m_mem_map.get_value_at(addr+reg_x);
        value--;
        m_mem_map.set_value_at(addr+reg_x, value);
        set_zero_negative_flags(value);
        reg_pc += 3;
        cycle = 7;
        break;
    case 0xe0: //CPX Immediate
        cycle = cmp_immediate(reg_x);
        reg_pc += 2;
        break;
    case 0xe4: //CPX Zero Page
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        value = m_mem_map.get_value_at(addr8);
        cmp(reg_x, value);
        reg_pc += 2;
        cycle = 3;
        break;
    case 0xe6: //INC Zero Page
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        value = m_mem_map.get_value_at(addr8);
        value++;
        m_mem_map.set_value_at(addr8, value);
        set_zero_negative_flags(value);

        reg_pc += 2;
        cycle = 5;
        break;
    case 0xe8: //INX
        reg_x++;
        set_zero_negative_flags(reg_x);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0xe9: //SBC Immediate
        value = m_mem_map.get_value_at(reg_pc+1);
        substract_with_carry(value);
        reg_pc += 2;
        cycle = 2;
        break;
    case 0xee: //INC Absolute
        addr = get_address_from_memory(reg_pc+1);
        value = m_mem_map.get_value_at(addr);
        value++;
        m_mem_map.set_value_at(addr, value);
        set_zero_negative_flags(value);
        reg_pc += 3;
        cycle = 6;
        break;
    case 0xf0: //BEQ
        cycle = jump_relative(get_status_register('Z'));
        break;
    case 0xf8: //SED
        set_status_register('D', true);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0xf9: //SBC Absolute Y
        addr = get_address_from_memory(reg_pc+1);
        value = m_mem_map.get_value_at(addr + reg_y);
        substract_with_carry(value);

        reg_pc += 3;
        cycle = 4; //FIXME 5 if page crossed
        break;
    default:
        std::cout << "Unimplemented opcode: 0x" << std::hex << static_cast<int>(opcode & 0xff) << "." << std::endl;
        exit(1);
    }

    if (opcode_to_inst.contains(opcode)) {
        std::cout << std::hex << ori_reg_pc << "->" << std::hex << reg_pc << " " << opcode_to_inst.at(opcode) << std::endl;
    }

    return cycle;
}

int CPU::jump_relative(bool do_jump) {
    int cycle = 2;
    if (do_jump) {
        // do the jump
        unsigned char offset = m_mem_map.get_value_at(reg_pc+1);
        char* real_offset = reinterpret_cast<char*>(&offset);
        reg_pc += *real_offset;
        cycle = 3; //FIXME or 4 if page crossed
    }
    reg_pc += 2;
    return cycle;
}

void CPU::push_value_to_stack(const uint16_t& value) {
    m_mem_map.set_value_at(0x100+reg_sp-1, value & 0x00FF);
    m_mem_map.set_value_at(0x100+reg_sp, (value & 0xFF00) >> 8);

    std::cout << "Push " << std::hex << value << std::endl;
    reg_sp -= 2;
}

void CPU::push_byte_to_stack(const unsigned char& value) {
    m_mem_map.set_value_at(0x100+reg_sp, value);
    std::cout << "Push byte " << std::hex << (int)value << std::endl;
    reg_sp -= 1;
}

uint16_t CPU::pull_value_from_stack() {
    reg_sp += 2;
    uint16_t value = convert_2_bytes_to_16bits(m_mem_map.get_value_at(0x100+reg_sp-1), m_mem_map.get_value_at(0x100+reg_sp));
    std::cout << "Pull " << std::hex << value << std::endl;
    return value;
}

unsigned char CPU::pull_byte_from_stack() {
    reg_sp += 1;
    unsigned char value = m_mem_map.get_value_at(0x100+reg_sp);
    std::cout << "Pull byte " << std::hex << (int)value << std::endl;
    return value;
}

void CPU::cmp(const unsigned char& reg_value, const unsigned char& value) {
    set_status_register('Z', reg_value == value);
    set_status_register('C', reg_value >= value);
    set_status_register('N', reg_value < value);
}

int CPU::cmp_immediate(const unsigned char& reg_value) {
    unsigned char value = m_mem_map.get_value_at(reg_pc+1);
    cmp(reg_value, value);
    reg_pc += 2;
    return 2;
}

void CPU::shift_left(unsigned char& val) {
    set_status_register('C', val & 0b1000000);
    val <<= 1;
    set_zero_negative_flags(val);
}

void CPU::shift_right(unsigned char& val) {
    set_status_register('C', val & 0b00000001);
    val >>= 1;
    set_zero_negative_flags(val);
}

void CPU::rotate_right(unsigned char& val) {
    bool ori_carry = get_status_register('C');
    set_status_register('C', val & 0b00000001);
    val >>= 1;
    val |= ori_carry * 0b10000000;
    set_zero_negative_flags(val);
}


void CPU::substract_with_carry(const unsigned char& value) {
    // To substract = res
    unsigned char to_sub = value + (get_status_register('C') ? 0 : 1);
    set_status_register('C', to_sub > reg_a);
    reg_a = reg_a - to_sub;

    set_status_register('V', (reg_a ^ reg_a) & (reg_a ^ ~value) & 0x80); //TODO not sure
    set_zero_negative_flags(reg_a);
}

void CPU::add_with_carry(const unsigned char& value) {
    unsigned char to_add = value + (get_status_register('C') ? 1 : 0);
    set_status_register('C', 255-to_add < reg_a);
    reg_a = reg_a + to_add;
    set_status_register('V', (reg_a ^ reg_a) & (reg_a ^ ~value) & 0x80); //TODO not sure
    set_zero_negative_flags(reg_a);
}

void CPU::set_zero_negative_flags(const unsigned char& value){
    set_status_register('Z', value == 0);
    set_status_register('N', value & 128);
}
