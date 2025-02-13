#include "cpu.h"

#include <iostream>
#include <format>

#define REGISTER_MASK_C 0b00000001
#define REGISTER_MASK_Z 0b00000010
#define REGISTER_MASK_I 0b00000100
#define REGISTER_MASK_D 0b00001000
#define REGISTER_MASK_V 0b01000000
#define REGISTER_MASK_N 0b10000000

const std::map<uint8_t, std::string> CPU::opcode_to_inst = {
    {0x69, "ADC Immediate"},
    {0x65, "ADC Zero Page"},
    {0x75, "ADC Zero Page X"},
    {0x6d, "ADC Absolute"},
    {0x7d, "ADC Absolute X"},
    {0x79, "ADC Absolute Y"},
    {0x61, "ADC Indexed Indirect X"},
    {0x71, "ADC Indirect Indexed Y"},

    {0x29, "AND Immediate"},
    {0x25, "AND Zero Page"},
    {0x35, "AND Zero Page X"},
    {0x2d, "AND Absolute"},
    {0x3d, "AND Absolute X"},
    {0x39, "AND Absolute Y"},
    {0x21, "AND Indexed Indirect X"},
    {0x31, "AND Indirect Indexed Y"},

    {0x0a, "ASL Accumulator"},
    {0x06, "ASL Zero Page"},
    {0x16, "ASL Zero Page X"},
    {0x0e, "ASL Absolute"},
    {0x1e, "ASL Absolute X"},

    {0x90, "BCC"},
    {0xb0, "BCS"},
    {0xf0, "BEQ"},

    {0x24, "BIT Zero Page"},
    {0x2c, "BIT Absolute"},

    {0x30, "BMI"},
    {0xd0, "BNE"},
    {0x10, "BPL"},
    {0x00, "BRK"},
    {0x50, "BVC"},
    {0x70, "BVS"},

    {0x18, "CLC"},
    {0xd8, "CLD"},
    {0x58, "CLI"},
    {0xb8, "CLV"},

    {0xc9, "CMP Immediate"},
    {0xc5, "CMP Zero Page"},
    {0xd5, "CMP Zero Page X"},
    {0xcd, "CMP Absolute"},
    {0xdd, "CMP Absolute X"},
    {0xd9, "CMP Absolute Y"},
    {0xc1, "CMP Indexed Indirect X"},
    {0xd1, "CMP Indirect Indexed Y"},

    {0xe0, "CPX Immediate"},
    {0xe4, "CPX Zero Page"},
    {0xec, "CPX Absolute"},

    {0xc0, "CPY Immediate"},
    {0xc4, "CPY Zero Page"},
    {0xcc, "CPY Absolute"},

    {0xc6, "DEC Zero Page"},
    {0xd6, "DEC Zero Page X"},
    {0xce, "DEC Absolute"},
    {0xde, "DEC Absolute X"},

    {0xca, "DEX"},
    {0x88, "DEY"},

    {0x49, "EOR Immediate"},
    {0x45, "EOR Zero Page"},
    {0x55, "EOR Zero Page X"},
    {0x4d, "EOR Absolute"},
    {0x5d, "EOR Absolute X"},
    {0x59, "EOR Absolute Y"},
    {0x41, "EOR Indexed Indirect X"},
    {0x51, "EOR Indirect Indexed Y"},

    {0xe6, "INC Zero Page"},
    {0xf6, "INC Zero Page X"},
    {0xee, "INC Absolute"},
    {0xfe, "INC Absolute X"},

    {0xe8, "INX"},
    {0xc8, "INY"},

    {0x4c, "JMP Absolute"},
    {0x6c, "JMP Indirect"},

    {0x20, "JSR"},

    {0xa9, "LDA Immediate"},
    {0xa5, "LDA Zero Page"},
    {0xb5, "LDA Zero Page X"},
    {0xad, "LDA Absolute"},
    {0xbd, "LDA Absolute X"},
    {0xb9, "LDA Absolute Y"},
    {0xa1, "LDA Indexed Indirect X"},
    {0xb1, "LDA Indirect Indexed Y"},

    {0xa2, "LDX Immediate"},
    {0xa6, "LDX Zero Page"},
    {0xb6, "LDX Zero Page Y"},
    {0xae, "LDX Absolute"},
    {0xbe, "LDX Absolute Y"},

    {0xa0, "LDY Immediate"},
    {0xa4, "LDY Zero Page"},
    {0xb4, "LDY Zero Page X"},
    {0xac, "LDY Absolute"},
    {0xbc, "LDY Absolute X"},

    {0x4a, "LSR Accumulator"},
    {0x46, "LSR Zero Page"},
    {0x56, "LSR Zero Page X"},
    {0x4e, "LSR Absolute"},
    {0x5e, "LSR Absolute X"},

    {0xea, "NOP"},

    {0x09, "ORA Immediate"},
    {0x05, "ORA Zero Page"},
    {0x15, "ORA Zero Page X"},
    {0x0d, "ORA Absolute"},
    {0x1d, "ORA Absolute X"},
    {0x19, "ORA Absolute Y"},
    {0x01, "ORA Indexed Indirect X"},
    {0x11, "ORA Indirect Indexed Y"},

    {0x48, "PHA"},
    {0x08, "PHP"},
    {0x68, "PLA"},
    {0x28, "PLP"},

    {0x2a, "ROL Accumulator"},
    {0x26, "ROL Zero Page"},
    {0x36, "ROL Zero Page X"},
    {0x2e, "ROL Absolute"},
    {0x3e, "ROL Absolute X"},

    {0x6a, "ROR Accumulator"},
    {0x66, "ROR Zero Page"},
    {0x76, "ROR Zero Page X"},
    {0x6e, "ROR Absolute"},
    {0x7e, "ROR Absolute X"},

    {0x40, "RTI"},
    {0x60, "RTS"},

    {0xe9, "SBC Immediate"},
    {0xf5, "SBC Zero Page X"},
    {0xed, "SBC Absolute"},
    {0xfd, "SBC Absolute X"},
    {0xf9, "SBC Absolute Y"},
    {0xe1, "SBC Indexed Indirect X"},
    {0xf1, "SBC Indirect Indexed Y"},

    {0x38, "SEC"},
    {0xf8, "SED"},
    {0x78, "SEI"},

    {0x17, "SLO Zero Page X (non official)"},

    {0x85, "STA Zero Page"},
    {0x95, "STA Zero Page X"},
    {0x8d, "STA Absolute"},
    {0x9d, "STA Absolute X"},
    {0x99, "STA Absolute Y"},
    {0x81, "STA Indexed Indirect X"},
    {0x91, "STA Indirect Indexed Y"},

    {0x86, "STX Zero Page"},
    {0x96, "STX Zero Page Y"},
    {0x8e, "STX Absolute"},

    {0x84, "STY Zero Page"},
    {0x94, "STY Zero Page X"},
    {0x8c, "STY Absolute"},

    {0xaa, "TAX"},
    {0xa8, "TAY"},
    {0xba, "TSX"},
    {0x8a, "TXA"},
    {0x9a, "TXS"},
    {0x98, "TYA"},

    {0x82, "DOP (non official)"},
    {0x8f, "AAX Absolute (non official)"},

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

int CPU::exec_cycle(int nb_cycles) {
    if (nmi) {
        nmi = false;
        push_value_to_stack(reg_pc);
        push_byte_to_stack(reg_p);
        reg_pc = get_address_from_memory(0xfffa);
    }

    int total_cycles = 0;
    while (total_cycles < nb_cycles) {
        uint8_t opcode = m_mem_map.get_value_at(reg_pc);
        short cycle_op = apply_op_code(opcode);
        total_cycles += cycle_op;
    }

    return total_cycles;
}

uint16_t CPU::get_address_from_memory(const uint16_t& address_1st_byte) {
    uint8_t byte1 = m_mem_map.get_value_at(address_1st_byte);
    uint8_t byte2 = m_mem_map.get_value_at(address_1st_byte + 1);

    return convert_2_bytes_to_16bits(byte1, byte2);
}

uint16_t CPU::convert_2_bytes_to_16bits(const uint8_t& byte1, const uint8_t& byte2) {
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
    uint8_t res;
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

short CPU::apply_op_code(const uint8_t& opcode) {
    short cycle = 0;
    uint16_t addr;
    uint8_t addr8;
    uint8_t value;

    /*if (opcode_to_inst.contains(opcode)) {
        std::cout << std::format("{} {:#x} {} - a={:#x} x={:#x} y={:#x} p={:#x}",
                                 cpu_instructions_nb, reg_pc, opcode_to_inst.at(opcode), reg_a, reg_x, reg_y, reg_p)
                  << std::endl;
        cpu_instructions_nb++;
    }*/

    switch(opcode) {
    case 0x69: //ADC Immediate
        add_with_carry(m_mem_map.get_value_at(reg_pc+1));
        reg_pc += 2;
        cycle = 2;
        break;
    case 0x65: //ADC Zero Page
        add_with_carry(get_zero_page_value());
        reg_pc += 2;
        cycle = 3;
        break;
    case 0x75: //ADC Zero Page X
        add_with_carry(get_zero_page_value(reg_x));
        reg_pc += 2;
        cycle = 4;
        break;
    case 0x6d: //ADC Absolute
        add_with_carry(get_absolute_value());
        reg_pc += 3;
        cycle = 4;
        break;
    case 0x7d: //ADC Absolute X
        add_with_carry(get_absolute_value(reg_x));
        reg_pc += 3;
        cycle = 4; //FIXME 5 if page crossed
        break;
    case 0x79: //ADC Absolute Y
        add_with_carry(get_absolute_value(reg_y));
        reg_pc += 3;
        cycle = 4; //FIXME 5 if page crossed
        break;
    case 0x61: //ADC Indexed Indirect X
        add_with_carry(get_indexed_indirect_value(reg_x));
        reg_pc += 2;
        cycle = 6;
        break;
    case 0x71: //ADC Indirect Indexed Y
        add_with_carry(get_indirect_indexed_value(reg_y));
        reg_pc += 2;
        cycle = 5; //6 if page crossed
        break;

    case 0x29: //AND Immediate
        reg_a &= m_mem_map.get_value_at(reg_pc+1);
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 2;
        break;
    case 0x25: //AND Zero Page
        reg_a &= get_zero_page_value();
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 3;
        break;
    case 0x35: //AND Zero Page X
        reg_a &= get_zero_page_value(reg_x);
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 4;
        break;
    case 0x2d: //AND Absolute
        reg_a &= get_absolute_value();
        set_zero_negative_flags(reg_a);
        reg_pc += 3;
        cycle = 4;
        break;
    case 0x3d: //AND Absolute X
        reg_a &= get_absolute_value(reg_x);
        set_zero_negative_flags(reg_a);
        reg_pc += 3;
        cycle = 4; //FIXME 5 if page crossed
        break;
    case 0x39: //AND Absolute Y
        reg_a &= get_absolute_value(reg_y);
        set_zero_negative_flags(reg_a);
        reg_pc += 3;
        cycle = 4; //FIXME 5 if page crossed
        break;
    case 0x21: //AND Indexed Indirect X
        reg_a &= get_indexed_indirect_value(reg_x);
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 6;
        break;
    case 0x31: //AND Indirect Indexed Y
        reg_a &= get_indirect_indexed_value(reg_y);
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 5; //FIXME 6 if page crossed
        break;

    case 0x0a: //ASL Accumulator
        shift_left(reg_a);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0x06: //ASL Zero Page
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        value = m_mem_map.get_value_at(addr8);
        shift_left(value);
        m_mem_map.set_value_at(addr8, value);
        reg_pc += 2;
        cycle = 5;
        break;
    case 0x16: //ASL Zero Page X
        addr8 = m_mem_map.get_value_at(reg_pc+1) + reg_x;
        value = m_mem_map.get_value_at(addr8);
        shift_left(value);
        m_mem_map.set_value_at(addr8, value);
        reg_pc += 2;
        cycle = 6;
        break;
    case 0x0e: //ASL Absolute
        addr = get_address_from_memory(reg_pc+1);
        value = m_mem_map.get_value_at(addr);
        shift_left(value);
        m_mem_map.set_value_at(addr, value);
        reg_pc += 3;
        cycle = 6;
        break;
    case 0x1e: //ASL Absolute X
        addr = get_address_from_memory(reg_pc+1) + reg_x;
        value = m_mem_map.get_value_at(addr);
        shift_left(value);
        m_mem_map.set_value_at(addr, value);
        reg_pc += 3;
        cycle = 7;
        break;

    case 0x90: //BCC
        cycle = jump_relative(!get_status_register('C'));
        break;
    case 0xb0: //BCS
        cycle = jump_relative(get_status_register('C'));
        break;
    case 0xf0: //BEQ
        cycle = jump_relative(get_status_register('Z'));
        break;

    case 0x24: //BIT Zero Page
        bit(get_zero_page_value());
        reg_pc += 2;
        cycle = 3;
        break;
    case 0x2c: //BIT Absolute
        bit(get_absolute_value());
        reg_pc += 3;
        cycle = 4;
        break;

    case 0x30: //BMI
        cycle = jump_relative(get_status_register('N'));
        break;
    case 0xd0: //BNE
        cycle = jump_relative(!get_status_register('Z'));
        break;
    case 0x10: //BPL
        cycle = jump_relative(!get_status_register('N'));
        break;

    case 0x00: //BRK
        push_value_to_stack(reg_pc + 2);
        push_byte_to_stack(reg_p);
        reg_pc = 0xfffe;
        cycle = 7;
        break;

    case 0x50: //BVC
        cycle = jump_relative(!get_status_register('V'));
        break;
    case 0x70: //BVS
        cycle = jump_relative(get_status_register('V'));
        break;

    case 0x18: //CLC
        set_status_register('C', false);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0xd8: //CLD
        clear_status_register('D');
        reg_pc += 1;
        cycle = 2;
        break;
    case 0x58: //CLI
        set_status_register('I', false);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0xb8: //CLV
        set_status_register('V', false);
        reg_pc += 1;
        cycle = 2;
        break;

    case 0xc9: //CMP Immediate
        cmp(reg_a, m_mem_map.get_value_at(reg_pc+1));
        reg_pc += 2;
        cycle = 2;
        break;
    case 0xc5: //CMP Zero Page
        cmp(reg_a, get_zero_page_value());
        reg_pc += 2;
        cycle = 3;
        break;
    case 0xd5: //CMP Zero Page X
        cmp(reg_a, get_zero_page_value(reg_x));
        reg_pc += 2;
        cycle = 4;
        break;
    case 0xcd: //CMP Absolute
        cmp(reg_a, get_absolute_value());
        reg_pc += 3;
        cycle = 4;
        break;
    case 0xdd: //CMP Absolute X
        cmp(reg_a, get_absolute_value(reg_x));
        reg_pc += 3;
        cycle = 4; //5 if page crossed
        break;
    case 0xd9: //CMP Absolute Y
        cmp(reg_a, get_absolute_value(reg_y));
        reg_pc += 3;
        cycle = 4; //5 if page crossed
        break;
    case 0xc1: //CMP Indexed Indirect X
        cmp(reg_a, get_indexed_indirect_value(reg_x));
        reg_pc += 2;
        cycle = 6;
        break;
    case 0xd1: //CMP Indirect Indexed Y
        cmp(reg_a, get_indirect_indexed_value(reg_y));
        reg_pc += 2;
        cycle = 5; //FIXME 6 if paged crossed
        break;

    case 0xe0: //CPX Immediate
        cmp(reg_x, m_mem_map.get_value_at(reg_pc+1));
        reg_pc += 2;
        cycle = 2;
        break;
    case 0xe4: //CPX Zero Page
        cmp(reg_x, get_zero_page_value());
        reg_pc += 2;
        cycle = 3;
        break;
    case 0xec: //CPX Absolute
        cmp(reg_x, get_absolute_value());
        reg_pc += 3;
        cycle = 4;
        break;

    case 0xc0: //CPY Immediate
        cmp(reg_y, m_mem_map.get_value_at(reg_pc+1));
        reg_pc += 2;
        cycle = 2;
        break;
    case 0xc4: //CPY Zero Page
        cmp(reg_y, get_zero_page_value());
        reg_pc += 2;
        cycle = 3;
        break;
    case 0xcc: //CPY Absolute
        cmp(reg_y, get_absolute_value());
        reg_pc += 3;
        cycle = 4;
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
    case 0xd6: //DEC Zero Page X
        addr8 = m_mem_map.get_value_at(reg_pc+1) + reg_x;
        value = m_mem_map.get_value_at(addr8);
        value--;
        m_mem_map.set_value_at(addr8, value);
        set_zero_negative_flags(value);
        reg_pc += 2;
        cycle = 6;
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
    case 0xde: //DEC Absolute X
        addr = get_address_from_memory(reg_pc+1) + reg_x;
        value = m_mem_map.get_value_at(addr);
        value--;
        m_mem_map.set_value_at(addr, value);
        set_zero_negative_flags(value);
        reg_pc += 3;
        cycle = 7;
        break;

    case 0xca: //DEX
        reg_x--;
        set_zero_negative_flags(reg_x);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0x88: //DEY
        reg_y--;
        set_zero_negative_flags(reg_y);
        reg_pc += 1;
        cycle = 2;
        break;

    case 0x49: //EOR Immediate
        reg_a ^= m_mem_map.get_value_at(reg_pc+1);
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 2;
        break;
    case 0x45: //EOR Zero Page
        reg_a ^= get_zero_page_value();
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 3;
        break;
    case 0x55: //EOR Zero Page X
        reg_a ^= get_zero_page_value(reg_x);
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 4;
        break;
    case 0x4d: //EOR Absolute
        reg_a ^= get_absolute_value();
        set_zero_negative_flags(reg_a);
        reg_pc += 3;
        cycle = 4;
        break;
    case 0x5d: //EOR Absolute X
        reg_a ^= get_absolute_value(reg_x);
        set_zero_negative_flags(reg_a);
        reg_pc += 3;
        cycle = 4; //5 if page crossed
        break;
    case 0x59: //EOR Absolute Y
        reg_a ^= get_absolute_value(reg_y);
        set_zero_negative_flags(reg_a);
        reg_pc += 3;
        cycle = 4; //5 if page crossed
        break;
    case 0x41: //EOR Indexed Indirect X
        reg_a ^= get_indexed_indirect_value(reg_x);
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 6;
        break;
    case 0x51: //EOR Indirect Indexed Y
        reg_a ^= get_indirect_indexed_value(reg_y);
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 5; //6 if page crossed
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
    case 0xf6: //INC Zero Page X
        addr8 = m_mem_map.get_value_at(reg_pc+1) + reg_x;
        value = m_mem_map.get_value_at(addr8);
        value++;
        m_mem_map.set_value_at(addr8, value);
        set_zero_negative_flags(value);
        reg_pc += 2;
        cycle = 6;
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
    case 0xfe: //INC Absolute X
        addr = get_address_from_memory(reg_pc+1) + reg_x;
        value = m_mem_map.get_value_at(addr);
        value++;
        m_mem_map.set_value_at(addr, value);
        set_zero_negative_flags(value);
        reg_pc += 3;
        cycle = 7;
        break;

    case 0xe8: //INX
        reg_x++;
        set_zero_negative_flags(reg_x);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0xc8: //INY
        reg_y++;
        set_zero_negative_flags(reg_y);
        reg_pc += 1;
        cycle = 2;
        break;

    case 0x4c: //JMP Absolute
        reg_pc = get_address_from_memory(reg_pc+1);
        cycle = 3;
        break;
    case 0x6c: //JMP Indirect
        addr = get_address_from_memory(reg_pc+1);
        reg_pc = get_address_from_memory(addr);
        cycle = 5;
        break;

    case 0x20: //JSR
        push_value_to_stack(reg_pc + 2);
        reg_pc = get_address_from_memory(reg_pc+1);
        cycle = 6;
        break;

    case 0xa9: //LDA Immediate
        reg_a = m_mem_map.get_value_at(reg_pc+1);
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 2;
        break;
    case 0xa5: //LDA Zero Page
        reg_a = get_zero_page_value();
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 3;
        break;
    case 0xb5: //LDA Zero Page X
        reg_a = get_zero_page_value(reg_x);
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 4;
        break;
    case 0xad: //LDA Absolute
        reg_a = get_absolute_value();
        set_zero_negative_flags(reg_a);
        reg_pc += 3;
        cycle = 4;
        break;
    case 0xbd: //LDA Absolute X
        reg_a = get_absolute_value(reg_x);
        set_zero_negative_flags(reg_a);
        reg_pc += 3;
        cycle = 4; //FIXME or 5 if page crossed
        break;
    case 0xb9: //LDA Absolute Y
        reg_a = get_absolute_value(reg_y);
        set_zero_negative_flags(reg_a);
        reg_pc += 3;
        cycle = 4; //FIXME 5 if paged crossed
        break;
    case 0xa1: //LDA Indexed Indirect X
        reg_a = get_indexed_indirect_value(reg_x);
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 6;
        break;
    case 0xb1: //LDA Indirect Indexed Y
        reg_a = get_indirect_indexed_value(reg_y);
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 5; //FIXME 6 if paged crossed
        break;

    case 0xa2: //LDX Immediate
        reg_x = m_mem_map.get_value_at(reg_pc+1);
        set_zero_negative_flags(reg_x);
        reg_pc += 2;
        cycle = 2;
        break;
    case 0xa6: //LDX Zero Page
        reg_x = get_zero_page_value();
        set_zero_negative_flags(reg_x);
        reg_pc += 2;
        cycle = 3;
        break;
    case 0xb6: //LDX Zero Page Y
        reg_x = get_zero_page_value(reg_y);
        set_zero_negative_flags(reg_x);
        reg_pc += 2;
        cycle = 4;
        break;
    case 0xae: //LDX Absolute
        reg_x = get_absolute_value();
        set_zero_negative_flags(reg_x);
        reg_pc += 3;
        cycle = 4;
        break;
    case 0xbe: //LDX Absolute Y
        reg_x = get_absolute_value(reg_y);
        set_zero_negative_flags(reg_x);
        reg_pc += 3;
        cycle = 4; //FIXME or 5 if page crossed
        break;

    case 0xa0: //LDY Immediate
        reg_y = m_mem_map.get_value_at(reg_pc+1);
        set_zero_negative_flags(reg_y);
        reg_pc += 2;
        cycle = 2;
        break;
    case 0xa4: //LDY Zero Page
        reg_y = get_zero_page_value();
        set_zero_negative_flags(reg_y);
        reg_pc += 2;
        cycle = 3;
        break;
    case 0xb4: //LDY Zero Page X
        reg_y = get_zero_page_value(reg_x);
        set_zero_negative_flags(reg_y);
        reg_pc += 2;
        cycle = 4;
        break;
    case 0xac: //LDY Absolute
        reg_y = get_absolute_value();
        set_zero_negative_flags(reg_y);
        reg_pc += 3;
        cycle = 4;
        break;
    case 0xbc: //LDY Absolute X
        reg_y = get_absolute_value(reg_x);
        set_zero_negative_flags(reg_y);
        reg_pc += 3;
        cycle = 4; //FIXME 5 if paged crossed
        break;

    case 0x4a: //LSR Accumulator
        shift_right(reg_a);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0x46: //LSR Zero Page
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        value = m_mem_map.get_value_at(addr8);
        shift_right(value);
        m_mem_map.set_value_at(addr8, value);
        reg_pc += 2;
        cycle = 5;
        break;
    case 0x56: //LSR Zero Page X
        addr8 = m_mem_map.get_value_at(reg_pc+1) + reg_x;
        value = m_mem_map.get_value_at(addr8);
        shift_right(value);
        m_mem_map.set_value_at(addr8, value);
        reg_pc += 2;
        cycle = 6;
        break;
    case 0x4e: //LSR Absolute
        addr = get_address_from_memory(reg_pc+1);
        value = m_mem_map.get_value_at(addr);
        shift_right(value);
        m_mem_map.set_value_at(addr, value);
        reg_pc += 3;
        cycle = 6;
        break;
    case 0x5e: //LSR Absolute X
        addr = get_address_from_memory(reg_pc+1) + reg_x;
        value = m_mem_map.get_value_at(addr);
        shift_right(value);
        m_mem_map.set_value_at(addr, value);
        reg_pc += 3;
        cycle = 7;
        break;

    case 0xea: //NOP
        reg_pc += 1;
        cycle = 2;
        break;

    case 0x09: //ORA Immediate
        reg_a |= m_mem_map.get_value_at(reg_pc+1);
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 2;
        break;
    case 0x05: //ORA Zero Page
        reg_a |= get_zero_page_value();
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 3;
        break;
    case 0x15: //ORA Zero Page X
        reg_a |= get_zero_page_value(reg_x);
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 4;
        break;
    case 0x0d: //ORA Absolute
        reg_a |= get_absolute_value();
        set_zero_negative_flags(reg_a);
        reg_pc += 3;
        cycle = 4;
        break;
    case 0x1d: //ORA Absolute X
        reg_a |= get_absolute_value(reg_x);
        set_zero_negative_flags(reg_a);
        reg_pc += 3;
        cycle = 4; //5 if page crossed
        break;
    case 0x19: //ORA Absolute Y
        reg_a |= get_absolute_value(reg_y);
        set_zero_negative_flags(reg_a);
        reg_pc += 3;
        cycle = 4; //5 if page crossed
        break;
    case 0x01: //ORA Indexed Indirect X
        reg_a |= get_indexed_indirect_value(reg_x);
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 6;
        break;
    case 0x11: //ORA Indirect Indexed Y
        reg_a |= get_indirect_indexed_value(reg_y);
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 5; //6 if page crossed
        break;

    case 0x48: //PHA
        push_byte_to_stack(reg_a);
        reg_pc += 1;
        cycle = 3;
        break;
    case 0x08: //PHP
        push_byte_to_stack(reg_p | 0b00110000);
        reg_pc += 1;
        cycle = 3;
        break;
    case 0x68: //PLA
        reg_a = pull_byte_from_stack();
        set_zero_negative_flags(reg_a);
        reg_pc += 1;
        cycle = 4;
        break;
    case 0x28: //PLP
        reg_p = (pull_byte_from_stack() & 0b11001111) | (reg_p & 0b00110000);
        reg_pc += 1;
        cycle = 4;
        //FIXME The result of changing I is delayed 1 instruction
        break;



    case 0x2a: //ROL Accumulator
        rotate_left(reg_a);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0x26: //ROL Zero Page
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        value = m_mem_map.get_value_at(addr8);
        rotate_left(value);
        m_mem_map.set_value_at(addr8, value);
        reg_pc += 2;
        cycle = 5;
        break;
    case 0x36: //ROL Zero Page X
        addr8 =  m_mem_map.get_value_at(reg_pc+1) + reg_x;
        value = m_mem_map.get_value_at(addr8);
        rotate_left(value);
        m_mem_map.set_value_at(addr8, value);
        reg_pc += 2;
        cycle = 6;
        break;
    case 0x2e: //ROL Absolute
        addr = get_address_from_memory(reg_pc+1);
        value = m_mem_map.get_value_at(addr);
        rotate_left(value);
        m_mem_map.set_value_at(addr, value);
        reg_pc += 3;
        cycle = 6;
        break;
    case 0x3e: //ROL Absolute X
        addr = get_address_from_memory(reg_pc+1) + reg_x;
        value = m_mem_map.get_value_at(addr);
        rotate_left(value);
        m_mem_map.set_value_at(addr, value);
        reg_pc += 3;
        cycle = 7;
        break;

    case 0x6a: //ROR Accumulator
        rotate_right(reg_a);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0x66: //ROR Zero Page
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        value = m_mem_map.get_value_at(addr8);
        rotate_right(value);
        m_mem_map.set_value_at(addr8, value);
        reg_pc += 2;
        cycle = 5;
        break;
    case 0x76: //ROR Zero Page X
        addr8 = m_mem_map.get_value_at(reg_pc+1) + reg_x;
        value = m_mem_map.get_value_at(addr8);
        rotate_right(value);
        m_mem_map.set_value_at(addr8, value);
        reg_pc += 2;
        cycle = 6;
        break;
    case 0x6e: //ROR Absolute
        addr = get_address_from_memory(reg_pc+1);
        value = m_mem_map.get_value_at(addr);
        rotate_right(value);
        m_mem_map.set_value_at(addr, value);
        reg_pc += 3;
        cycle = 6;
        break;
    case 0x7e: //ROR Absolute X
        addr = get_address_from_memory(reg_pc+1) + reg_x;
        value = m_mem_map.get_value_at(addr);
        rotate_right(value);
        m_mem_map.set_value_at(addr, value);
        reg_pc += 3;
        cycle = 7;
        break;

    case 0x40: //RTI
        reg_p = pull_byte_from_stack();
        reg_pc = pull_value_from_stack();
        cycle = 6;
        break;

    case 0x60: //RTS
        reg_pc = pull_value_from_stack() + 1;
        cycle = 6;
        break;

    case 0xe9: //SBC Immediate
        substract_with_carry(m_mem_map.get_value_at(reg_pc+1));
        reg_pc += 2;
        cycle = 2;
        break;
    case 0xe5: //SBC Zero Page
        substract_with_carry(get_zero_page_value());
        reg_pc += 2;
        cycle = 3;
        break;
    case 0xf5: //SBC Zero Page X
        substract_with_carry(get_zero_page_value(reg_x));
        reg_pc += 2;
        cycle = 4;
        break;
    case 0xed: //SBC Absolute
        substract_with_carry(get_absolute_value());
        reg_pc += 3;
        cycle = 4;
        break;
    case 0xfd: //SBC Absolute X
        substract_with_carry(get_absolute_value(reg_x));
        reg_pc += 3;
        cycle = 4; //FIXME 5 if page crossed
        break;
    case 0xf9: //SBC Absolute Y
        substract_with_carry(get_absolute_value(reg_y));
        reg_pc += 3;
        cycle = 4; //FIXME 5 if page crossed
        break;
    case 0xe1: //SBC Indexed Indirect X
        substract_with_carry(get_indexed_indirect_value(reg_x));
        reg_pc += 2;
        cycle = 6;
        break;
    case 0xf1: //SBC Indirect Indexed Y
        substract_with_carry(get_indirect_indexed_value(reg_y));
        reg_pc += 2;
        cycle = 5; //6 if page crossed
        break;

    case 0x38: //SEC
        set_status_register('C', true);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0xf8: //SED
        set_status_register('D', true);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0x78: //SEI
        enable_status_register('I');
        reg_pc += 1;
        cycle = 2;
        break;

    case 0x85: //STA Zero Page
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        m_mem_map.set_value_at(addr8, reg_a);
        reg_pc += 2;
        cycle = 3;
        break;
    case 0x95: //STA Zero Page X
        addr8 = m_mem_map.get_value_at(reg_pc+1) + reg_x;
        m_mem_map.set_value_at(addr8, reg_a);
        reg_pc += 2;
        cycle = 4;
        break;
    case 0x8d: //STA Absolute
        addr = get_address_from_memory(reg_pc+1);
        m_mem_map.set_value_at(addr, reg_a);
        reg_pc += 3;
        cycle = 4;
        break;
    case 0x9d: //STA Absolute X
        addr = get_address_from_memory(reg_pc+1) + reg_x;
        m_mem_map.set_value_at(addr, reg_a);
        reg_pc += 3;
        cycle = 5;
        break;
    case 0x99: //STA Absolute Y
        addr = get_address_from_memory(reg_pc+1) + reg_y;
        m_mem_map.set_value_at(addr, reg_a);
        reg_pc += 3;
        cycle = 5;
        break;
    case 0x81: //STA Indexed Indirect X
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        addr = get_address_from_memory(addr8+reg_x);
        m_mem_map.set_value_at(addr, reg_a);
        reg_pc += 2;
        cycle = 6;
        break;
    case 0x91: //STA Indirect Indexed Y
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        addr = get_address_from_memory(addr8) + reg_y;
        m_mem_map.set_value_at(addr, reg_a);
        reg_pc += 2;
        cycle = 6;
        break;

    case 0x86: //STX Zero Page
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        m_mem_map.set_value_at(addr8, reg_x);
        reg_pc += 2;
        cycle = 3;
        break;
    case 0x96: //STX Zero Page Y
        addr8 = m_mem_map.get_value_at(reg_pc+1) + reg_y;
        m_mem_map.set_value_at(addr8, reg_x);
        reg_pc += 2;
        cycle = 4;
        break;
    case 0x8e: //STX Absolute
        addr = get_address_from_memory(reg_pc+1);
        m_mem_map.set_value_at(addr, reg_x);
        reg_pc += 3;
        cycle = 4;
        break;

    case 0x84: //STY Zero Page
        addr8 = m_mem_map.get_value_at(reg_pc+1);
        m_mem_map.set_value_at(addr8, reg_y);
        reg_pc += 2;
        cycle = 3;
        break;
    case 0x94: //STY Zero Page X
        addr8 = m_mem_map.get_value_at(reg_pc+1) + reg_x;
        m_mem_map.set_value_at(addr8, reg_y);
        reg_pc += 2;
        cycle = 4;
        break;
    case 0x8c: //STY Absolute
        addr = get_address_from_memory(reg_pc+1);
        m_mem_map.set_value_at(addr, reg_y);
        reg_pc += 3;
        cycle = 4;
        break;

    case 0xaa: //TAX
        reg_x = reg_a;
        set_zero_negative_flags(reg_x);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0xa8: //TAY
        reg_y = reg_a;
        set_zero_negative_flags(reg_y);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0xba: //TSX
        reg_x = reg_sp;
        set_zero_negative_flags(reg_x);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0x8a: //TXA
        reg_a = reg_x;
        set_zero_negative_flags(reg_a);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0x9a: //TXS
        reg_sp = reg_x;
        reg_pc += 1;
        cycle = 2;
        break;

    case 0x98: //TYA
        reg_a = reg_y;
        set_zero_negative_flags(reg_a);
        reg_pc += 1;
        cycle = 2;
        break;


    case 0x8f: //AAX Absolute (unofficial)
        addr = get_address_from_memory(reg_pc+1);
        m_mem_map.set_value_at(addr, reg_a & reg_x);
        reg_pc += 3;
        cycle = 4;
        break;
    case 0x82: //DOP (unofficial)
        reg_pc += 2;
        cycle = 2;
        break;
    case 0x17: //SLO Zero Page X (unofficial)
        value = get_zero_page_value(reg_x);
        shift_left(value);
        reg_a |= value;
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 6;
        break;
    default:
        std::cout << std::format("Unimplemented opcode: {:#x} at {:#x}.", opcode, reg_pc) << std::endl;
        exit(1);
    }

    return cycle;
}

int CPU::jump_relative(bool do_jump) {
    int cycle = 2;
    if (do_jump) {
        // do the jump
        uint8_t offset = m_mem_map.get_value_at(reg_pc+1);
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

    //std::cout << "Push " << std::hex << value << std::endl;
    reg_sp -= 2;
}

void CPU::push_byte_to_stack(const uint8_t& value) {
    m_mem_map.set_value_at(0x100+reg_sp, value);
    //std::cout << "Push byte " << std::hex << (int)value << std::endl;
    reg_sp -= 1;
}

uint16_t CPU::pull_value_from_stack() {
    reg_sp += 2;
    uint16_t value = convert_2_bytes_to_16bits(m_mem_map.get_value_at(0x100+reg_sp-1), m_mem_map.get_value_at(0x100+reg_sp));
    //std::cout << "Pull " << std::hex << value << std::endl;
    return value;
}

uint8_t CPU::pull_byte_from_stack() {
    reg_sp += 1;
    uint8_t value = m_mem_map.get_value_at(0x100+reg_sp);
    //std::cout << "Pull byte " << std::hex << (int)value << std::endl;
    return value;
}



uint8_t CPU::get_zero_page_value() {
    uint8_t addr8 = m_mem_map.get_value_at(reg_pc+1);
    return m_mem_map.get_value_at(addr8);
}

uint8_t CPU::get_zero_page_value(const uint8_t& to_add) {
    uint8_t addr8 = m_mem_map.get_value_at(reg_pc+1);
    return m_mem_map.get_value_at(addr8 + to_add);
}

uint8_t CPU::get_absolute_value() {
    uint16_t addr = get_address_from_memory(reg_pc+1);
    return m_mem_map.get_value_at(addr);
}

uint8_t CPU::get_absolute_value(const uint8_t& to_add) {
    uint16_t addr = get_address_from_memory(reg_pc+1);
    return m_mem_map.get_value_at(addr + static_cast<uint16_t>(to_add));
}

uint8_t CPU::get_indexed_indirect_value(const uint8_t& to_add) {
    uint8_t addr8 = m_mem_map.get_value_at(reg_pc+1);
    uint16_t addr = get_address_from_memory(addr8+to_add);
    return m_mem_map.get_value_at(addr);
}

uint8_t CPU::get_indirect_indexed_value(const uint8_t& to_add) {
    uint8_t addr8 = m_mem_map.get_value_at(reg_pc+1);
    uint16_t addr = get_address_from_memory(addr8);
    return m_mem_map.get_value_at(addr+to_add);
}

void CPU::cmp(const uint8_t& reg_value, const uint8_t& value) {
    set_status_register('Z', reg_value == value);
    set_status_register('C', reg_value >= value);
    uint8_t res = reg_value - value;
    set_status_register('N', res & 0b10000000);
}

void CPU::bit(const uint8_t& value) {
    uint8_t res = reg_a & value;
    set_status_register('V', value & 0b01000000);
    set_status_register('N', value & 0b10000000);
    set_status_register('Z', res == 0);
}

void CPU::shift_left(uint8_t& val) {
    set_status_register('C', val & 0b10000000);
    val <<= 1;
    set_zero_negative_flags(val);
}

void CPU::shift_right(uint8_t& val) {
    set_status_register('C', val & 0b00000001);
    val >>= 1;
    set_zero_negative_flags(val);
}

void CPU::rotate_right(uint8_t& val) {
    bool ori_carry = get_status_register('C');
    set_status_register('C', val & 0b00000001);
    val >>= 1;
    val |= ori_carry * 0b10000000;
    set_zero_negative_flags(val);
}

void CPU::rotate_left(uint8_t& val) {
    bool ori_carry = get_status_register('C');
    set_status_register('C', val & 0b10000000);
    val <<= 1;
    val |= ori_carry * 0b00000001;
    set_zero_negative_flags(val);
}


void CPU::substract_with_carry(const uint8_t& value) {
    add_with_carry(~value);
}

void CPU::add_with_carry(const uint8_t& value) {
    uint8_t ori_reg_a = reg_a;
    unsigned int res = reg_a + value + (get_status_register('C') ? 1 : 0);
    set_status_register('C', res > 255);
    reg_a = res & 0xff;
    set_status_register('V', (reg_a ^ ori_reg_a) & (reg_a ^ value) & 0x80);
    set_zero_negative_flags(reg_a);
}

void CPU::set_zero_negative_flags(const uint8_t& value){
    set_status_register('Z', value == 0);
    set_status_register('N', value & 128);
}
