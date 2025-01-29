#include "cpu.h"

#include <iostream>
#include <bitset>


const std::map<unsigned char, std::string> CPU::opcode_to_inst = {
    {0x09, "ORA Immediate"},
    {0x10, "BPL"},
    {0x20, "JSR"},
    {0x24, "BIT Zero Page"},
    {0x29, "AND Immediate"},
    {0x2c, "BIT Absolute"},
    {0x4c, "JMP Absolute"},
    {0x60, "RTS"},
    {0x6c, "JMP Indirect"},
    {0x78, "SEI"},
    {0x84, "STY Zero Page"},
    {0x85, "STA Zero Page"},
    {0x86, "STX Zero Page"},
    {0x88, "DEY"},
    {0x8a, "TXA"},
    {0x8d, "STA Absolute"},
    {0x91, "STA Indirect Y"},
    {0x99, "STA Absolute Y"},
    {0x9a, "TXS"},
    {0xa0, "LDY Immediate"},
    {0xa2, "LDX Immediate"},
    {0xa9, "LDA immediate"},
    {0xad, "LDA Absolute"},
    {0xb0, "BCS"},
    {0xbd, "LDA Absolute X"},
    {0xc0, "CPY Immediate"},
    {0xc8, "INY"},
    {0xc9, "CMP Immediate"},
    {0xca, "DEX"},
    {0xd0, "BNE"},
    {0xd8, "CLD"},
    {0xe0, "CPX Immediate"},
    {0xe6, "INC Zero Page"},
    {0xee, "INC Absolute"},


};

CPU::CPU(): ram(65535) {}

void CPU::init(){
    reg_x = 0;
    reg_a = 0;
}

void CPU::load_rom(const ROM& rom) {
    ram.set_memory_range(0x8000, rom.prg_rom);

    init_pc_from_reset_vector();
}

bool CPU::exec_cycle() {
    unsigned char opcode = ram.get_value_at(reg_pc);
    bool success = apply_op_code(opcode);
    return success;
}

void CPU::init_pc_from_reset_vector() {
    reg_pc = get_address_from_memory(0xfffc);
}

uint16_t CPU::get_address_from_memory(const uint16_t& address_1st_byte) {
    unsigned char byte1 = ram.get_value_at(address_1st_byte);
    unsigned char byte2 = ram.get_value_at(address_1st_byte + 1);

    //std::bitset<8> byte1_bs(byte1);
    //std::bitset<8> byte2_bs(byte2);
    //std::cout << byte1_bs << " " << byte2_bs;

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
        reg_p |= 1;
        break;
    case 'Z':
        reg_p |= 2;
        break;
    case 'I':
        reg_p |= 4;
        break;
    case 'D':
        reg_p |= 8;
        break;
    case 'V':
        reg_p |= 64;
        break;
    case 'N':
        reg_p |= 128;
        break;
    }
}

void CPU::clear_status_register(char status) {
    switch(status) {
    case 'C':
        reg_p &= 0xfe;
        break;
    case 'Z':
        reg_p &= 0xfd;
        break;
    case 'I':
        reg_p &= 0xfb;
        break;
    case 'D':
        reg_p &= 0xf7;
        break;
    case 'V':
        reg_p &= 0xbf;
        break;
    case 'N':
        reg_p &= 0x7f;
        break;
    }
}

bool CPU::get_status_register(char status) {
    unsigned char res;
    switch(status) {
    case 'C':
        res = reg_p & 1;
        break;
    case 'Z':
        res = reg_p & 2;
        break;
    case 'I':
        res = reg_p & 4;
        break;
    case 'D':
        res = reg_p & 8;
        break;
    case 'V':
        res = reg_p & 64;
        break;
    case 'N':
        res = reg_p & 128;
        break;
    }

    return res > 0;
}

bool CPU::apply_op_code(const unsigned char& opcode) {
    short cycle = 0;
    bool success = true;
    uint16_t addr;
    unsigned char addr8;
    unsigned char value, res;

    std::cout << reg_pc << " " << opcode_to_inst.at(opcode);

    switch(opcode) {
    case 0x09: //ORA Immediate
        value = ram.get_value_at(reg_pc+1);
        reg_a |= value;
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
        cycle = 2;
        break;
    case 0x10: //BPL
        cycle = jump_relative(!get_status_register('N'));
        break;
    case 0x20: //JSR
        push_value_to_stack(reg_pc + 2);
        addr = get_address_from_memory(reg_pc+1);
        reg_pc = addr;
        cycle = 6;
        break;
    case 0x2c: //BIT Absolute
        addr = get_address_from_memory(reg_pc+1);
        value = ram.get_value_at(addr);
        res = reg_a & value;
        set_status_register('V', value & 64);
        set_status_register('N', value & 128);
        set_status_register('Z', res == 0);

        reg_pc += 3;
        cycle = 4;
        break;
    case 0x29: //AND Immediate
        value = ram.get_value_at(reg_pc+1);
        reg_a &= value;
        set_zero_negative_flags(reg_a);
        reg_pc += 2;
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
    case 0x78: //SEI
        enable_status_register('I');
        reg_pc += 1;
        cycle = 2;
        break;
    case 0x84: //STY Zero Page
        addr8 = ram.get_value_at(reg_pc+1);
        ram.set_value_at_zero_page(addr8, reg_y);
        reg_pc += 2;
        cycle = 3;
        break;
    case 0x85: //STA Zero Page
        addr8 = ram.get_value_at(reg_pc+1);
        ram.set_value_at_zero_page(addr8, reg_a);
        reg_pc += 2;
        cycle = 3;
        break;
    case 0x86: //STX Zero Page
        addr8 = ram.get_value_at(reg_pc+1);
        ram.set_value_at_zero_page(addr8, reg_x);
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
    case 0x8d: //STA Absolute
        addr = get_address_from_memory(reg_pc+1);
        ram.set_value_at(addr, reg_a);
        cycle = 4;
        reg_pc += 3;
        break;
    case 0x91: //STA Indirect Y
        addr8 = ram.get_value_at(reg_pc+1);
        addr = convert_2_bytes_to_16bits(addr8, reg_y);
        ram.set_value_at(addr, reg_a);
        cycle = 6;
        reg_pc += 2;
        break;
    case 0x99: //STA Absolute Y
        addr = get_address_from_memory(reg_pc+1);
        ram.set_value_at(addr + reg_y, reg_a);
        reg_pc += 3;
        cycle = 5;
        break;
    case 0x9a: //TXS
        reg_sp = reg_x;
        cycle = 2;
        reg_pc += 1;
        break;
    case 0xa0: //LDY Immediate
        reg_y = ram.get_value_at(reg_pc+1);
        reg_pc += 2;
        cycle = 2;
        break;
    case 0xa2: //LDX Immediate
        reg_x = ram.get_value_at(reg_pc+1);
        reg_pc += 2;
        cycle = 2;
        break;
    case 0xa9: //LDA immediate
        reg_a = ram.get_value_at(reg_pc+1);
        reg_pc += 2;
        cycle = 2;
        break;
    case 0xad: //LDA Absolute
        addr = get_address_from_memory(reg_pc+1);
        reg_a = ram.get_value_at(addr);
        reg_pc += 3;
        cycle = 4;
        break;
    case 0xb0: //BCS
        cycle = jump_relative(get_status_register('C'));
        break;
    case 0xbd: //LDA Absolute X
        addr = get_address_from_memory(reg_pc+1);
        reg_a = ram.get_value_at(addr + reg_x);
        reg_pc += 3;
        cycle = 4; //FIXME or 5 if page crossed
        break;
    case 0xc0: //CPY Immediate
        cycle = cmp_immediate(reg_y);
        reg_pc += 2;
        break;
    case 0xc8: //INY
        reg_y += 1;
        set_zero_negative_flags(reg_y);
        reg_pc += 1;
        cycle = 2;
        break;
    case 0xc9: //CMP Immediate
        cycle = cmp_immediate(reg_a);
        reg_pc += 2;
        break;
    case 0xca: //DEX
        reg_x -= 1;
        reg_pc += 1;
        cycle = 2;
        break;
    case 0xd0: //BNE
        cycle = jump_relative(!get_status_register('Z'));
        break;
    case 0xd8: //CLD
        clear_status_register('D');
        reg_pc += 1;
        cycle = 2;
        break;
    case 0xe0: //CPX Immediate
        cycle = cmp_immediate(reg_x);
        reg_pc += 2;
        break;
    case 0xee: //INC Absolute
        addr = get_address_from_memory(reg_pc+1);
        value = ram.get_value_at(addr);
        value += 1;
        ram.set_value_at(addr, value);
        set_zero_negative_flags(value);
        reg_pc += 3;
        cycle = 6;
        break;
    default:
        std::cout << "Unimplemented opcode: 0x" << std::hex << static_cast<int>(opcode & 0xff) << "." << std::endl;
        std::cout << (opcode == 0xd8);
        success = false;
    }
    std::cout << " " << get_address_from_memory(0x8058) << "-";
    std::cout << " " << reg_pc << std::endl;

    return success;
}

int CPU::jump_relative(bool do_jump) {
    int cycle = 2;
    if (do_jump) {
        // do the jump
        unsigned char offset = ram.get_value_at(reg_pc+1);
        char* real_offset = reinterpret_cast<char*>(&offset);
        reg_pc += 2 + *real_offset;
        cycle = 3; //FIXME or 4 if page crossed
    } else {
        // do not jump
        reg_pc += 2;
    }
    return cycle;
}

void CPU::push_value_to_stack(const uint16_t& value) {
    ram.set_value_at(reg_sp, value & 0x00FF);
    ram.set_value_at(reg_sp+1, (value & 0xFF00) >> 8);
    reg_sp += 2;
}

uint16_t CPU::pull_value_from_stack() {
    reg_sp -= 2;
    return convert_2_bytes_to_16bits(ram.get_value_at(reg_sp), ram.get_value_at(reg_sp+1));
}

int CPU::cmp_immediate(const unsigned char& reg_value) {
    unsigned char value = ram.get_value_at(reg_pc+1);
    if (reg_value == value) {
        enable_status_register('Z');
    } else {
        clear_status_register('Z');
    }

    if (reg_value >= value) {
        enable_status_register('C');
    } else {
        clear_status_register('C');
    }

    if (reg_value < value) {
        enable_status_register('N');
    } else {
        clear_status_register('N');
    }

    reg_pc += 2;
    return 2;
}

void CPU::set_zero_negative_flags(const unsigned char& value){
    set_status_register('Z', value == 0);
    set_status_register('N', value & 128);
}
