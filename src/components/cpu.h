#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <map>
#include <string>

#include "IMemory.h"


class CPU
{
public:
    CPU(IMemory& cpu_mem_map);
    void init();
    int exec_cycle(int nb_cycles);
    void set_nmi();

private:
    static const std::map<unsigned char, std::string> opcode_to_inst;
    IMemory& m_mem_map;
    bool nmi;

    unsigned char reg_a = 0; //accumulator
    unsigned char reg_x = 0;
    unsigned char reg_y = 0;
    uint16_t reg_pc; //program counter
    unsigned char reg_sp = 0; //stack pointer

    // For debug
    long cpu_instructions_nb = 0;

    /**
     * @brief reg_p
     * NV1B DIZC
     * Carry, Zero, Interrupt disable, Decimal, B flag, 1, Overflow, negative
     */
    unsigned char reg_p=0b00100000; //status register


    void set_status_register(char status, bool enable);
    void enable_status_register(char status);
    void clear_status_register(char status);
    bool get_status_register(char status);

    short apply_op_code(const unsigned char& opcode);
    uint16_t get_address_from_memory(const uint16_t& address_1st_byte);
    uint16_t convert_2_bytes_to_16bits(const unsigned char& byte1, const unsigned char& byte2);


    void push_value_to_stack(const uint16_t& value);
    uint16_t pull_value_from_stack();
    void push_byte_to_stack(const unsigned char& value);
    unsigned char pull_byte_from_stack();

    unsigned char get_zero_page_value();
    unsigned char get_zero_page_value(const unsigned char& to_add);

    unsigned char get_absolute_value();
    unsigned char get_absolute_value(const unsigned char& to_add);

    unsigned char get_indexed_indirect_value(const unsigned char& to_add);
    unsigned char get_indirect_indexed_value(const unsigned char& to_add);


    int jump_relative(bool do_jump);
    void cmp(const unsigned char& reg_value, const unsigned char& value);
    void bit(const unsigned char& value);
    void set_zero_negative_flags(const unsigned char& value);
    void shift_left(unsigned char& val);
    void shift_right(unsigned char& val);
    void rotate_right(unsigned char& val);
    void rotate_left(unsigned char& val);
    void substract_with_carry(const unsigned char& value);
    void add_with_carry(const unsigned char& value);
};

#endif // CPU_H
