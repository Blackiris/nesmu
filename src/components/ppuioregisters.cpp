#include "ppuioregisters.h"

#include <iostream>

PPUIORegisters::PPUIORegisters(RAM& oam)
    : RAM(9), m_oam(oam) {}

void PPUIORegisters::set_cpu_memory_map(IMemory* cpu_memory_map) {
    m_cpu_memory_map = cpu_memory_map;
}

unsigned char PPUIORegisters::get_value_at(const uint16_t& address) {

    if (address == OAMDATA) {
        return m_oam.get_value_at(get_value_at(OAMADDR));
    }

    return RAM::get_value_at(address);
}

void PPUIORegisters::set_value_at(const uint16_t& address, const unsigned char& value) {
    if (address == OAMDATA) {
        set_oam_data(value);
    } else if (address == OAMDMA) {
        const uint16_t& cpu_addr = value << 8;
        // copy 256 bytes to from CPU mem value * 0x100 to oam from oamaddr
        for (int i=0; i<256; i++) {
            set_oam_data(m_cpu_memory_map -> get_value_at(cpu_addr + i));
        }

    } else {
        RAM::set_value_at(address, value);
    }
}

void PPUIORegisters::set_oam_data(const unsigned char& value) {
    unsigned char oamaddr = get_value_at(OAMADDR);
    m_oam.set_value_at(oamaddr, value);
    set_value_at(OAMADDR, oamaddr + 1);
}
