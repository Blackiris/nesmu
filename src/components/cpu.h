#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <map>
#include <string>

#include "IMemory.h"


class CPU
{
public:
    explicit CPU(IMemory& cpu_mem_map);
    void init();
    int exec_cycle(int nb_cycles);
    void set_nmi() noexcept;

private:
    static const std::map<uint8_t, std::string> opcode_to_inst;
    IMemory& m_mem_map;
    bool nmi;

    uint8_t reg_a = 0; //accumulator
    uint8_t reg_x = 0;
    uint8_t reg_y = 0;
    uint16_t reg_pc; //program counter
    uint8_t reg_sp = 0; //stack pointer

    // For debug
    long cpu_instructions_nb = 0;

    /**
     * @brief reg_p
     * NV1B DIZC
     * Carry, Zero, Interrupt disable, Decimal, B flag, 1, Overflow, negative
     */
    uint8_t reg_p=0b00100000; //status register


    void set_status_register(char status, bool enable) noexcept;
    void enable_status_register(char status) noexcept;
    void clear_status_register(char status) noexcept;
    bool get_status_register(char status) const;

    short apply_op_code(const uint8_t& opcode);
    uint16_t get_address_from_memory(const uint16_t& address_1st_byte) const;
    uint16_t convert_2_bytes_to_16bits(const uint8_t& byte1, const uint8_t& byte2) const;


    void push_value_to_stack(const uint16_t& value);
    uint16_t pull_value_from_stack();
    void push_byte_to_stack(const uint8_t& value);
    uint8_t pull_byte_from_stack();

    uint8_t get_zero_page_value();
    uint8_t get_zero_page_value(const uint8_t& to_add);

    uint8_t get_absolute_value();
    uint8_t get_absolute_value(const uint8_t& to_add);

    uint8_t get_indexed_indirect_value(const uint8_t& to_add);
    uint8_t get_indirect_indexed_value(const uint8_t& to_add);


    int jump_relative(bool do_jump);
    void cmp(const uint8_t& reg_value, const uint8_t& value);
    void bit(const uint8_t& value);
    void set_zero_negative_flags(const uint8_t& value);
    void shift_left(uint8_t& val);
    void shift_right(uint8_t& val);
    void rotate_right(uint8_t& val);
    void rotate_left(uint8_t& val);
    void substract_with_carry(const uint8_t& value);
    void add_with_carry(const uint8_t& value);
};

#endif // CPU_H
