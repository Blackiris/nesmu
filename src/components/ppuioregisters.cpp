#include "ppuioregisters.h"

#include <iostream>

PPUIORegisters::PPUIORegisters(RAM& oam, IMemory& vram)
    : RAM(8), m_oam(oam), m_vram(vram) {}

void PPUIORegisters::set_cpu_memory_map(IMemory* cpu_memory_map) {
    m_cpu_memory_map = cpu_memory_map;
}

unsigned char PPUIORegisters::get_value_at(const uint16_t& address) {

    if (address == OAMDATA) {
        return m_oam.get_value_at(get_value_at(OAMADDR));
    } else if (address == PPUDATA) {
        unsigned char val_to_return = ppu_data_buffer;
        ppu_data_buffer = m_vram.get_value_at(m_ppu_addr);
        m_ppu_addr += get_vram_addr_incr();
        return val_to_return;
    } else if (address == PPUSTATUS) {
        w = false;
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
    } else if (address == PPUADDR) {
        if (!w) {
            t = (t & 0b000000011111111) | ((value & 0b111111) << 8);
        } else {
            t = (t & 0b111111100000000) | value;
            v = t;
        }

        m_ppu_addr = m_ppu_addr << 8 | value;
        w = !w;
    } else if (address == PPUDATA) {
        //std::cout << std::hex << m_ppu_addr << " - " << (int)value << std::endl;
        m_vram.set_value_at(m_ppu_addr & 0x3FFF, value);
        m_ppu_addr += get_vram_addr_incr();
    } else if (address == PPUSCROLL) {
        if (!w) {
            t = (t & 0b11111111100000) | ((value & 0b11111000) >> 3);
            x = value & 0b111;
            ppu_scroll_x = value;
        } else {
            t = (t & 0b000110000011111) | ((value & 0b11111000) << 2) | ((value & 0b111) << 12);
            ppu_scroll_y = value;
        }
        w = !w;
    } else if (address == PPUCTRL) {
        t = (t & 0b111001111111111) | ((value & 0b11) << 10);
        RAM::set_value_at(address, value);
    } else {
        RAM::set_value_at(address, value);
    }
}

void PPUIORegisters::set_oam_data(const unsigned char& value) {
    unsigned char oamaddr = get_value_at(OAMADDR);
    m_oam.set_value_at(oamaddr, value);
    set_value_at(OAMADDR, oamaddr + 1);
}

int PPUIORegisters::get_vram_addr_incr() {
    return RAM::get_bit_at(PPUCTRL, PPUCTRL_VRAM_ADDR_INCR) ? 32 : 1;
}

unsigned int PPUIORegisters::get_scroll_x() {
    bool x_bit8 = (t >> 10) & 0b1;
    return x_bit8 * 256 + (x | ((t & 0b11111) << 3));
    //bool x_bit8 = RAM::get_bit_at(PPUCTRL, PPUCTRL_SCROLL_X_BIT8);
    //return x_bit8 * 256 + ppu_scroll_x;
}

unsigned int PPUIORegisters::get_scroll_y() {
    bool y_bit8 = RAM::get_bit_at(PPUCTRL, PPUCTRL_SCROLL_Y_BIT8);
    return y_bit8 * 240 + ppu_scroll_y;
}
