#include "ppumemorymap.h"

#define BASE_PALETTE_ADDR 0x3f00

PPUMemoryMap::PPUMemoryMap(ROM& rom, RAM& vram): m_vram(vram) {
    m_vram.set_memory_range(0x0000, rom.chr_rom);
}

unsigned char PPUMemoryMap::get_value_at(const uint16_t& address) {
    uint16_t real_address = address & 0x3fff;
    if (real_address >= 0x3f00) {
        real_address = 0x3f00 + (real_address - 0x3f00) % 0xf20;
    }

    return m_vram.get_value_at(real_address);
}
void PPUMemoryMap::set_value_at(const uint16_t& address, const unsigned char& value) {
    uint16_t real_address = address & 0x3fff;
    if (real_address >= BASE_PALETTE_ADDR) {
        uint16_t offset_addr = real_address - BASE_PALETTE_ADDR;

        if (offset_addr % 4 == 0) {
            m_vram.set_value_at(BASE_PALETTE_ADDR+ offset_addr, value);
            m_vram.set_value_at(BASE_PALETTE_ADDR+(offset_addr + 0x10) % 0x20, value);
        } else {
            real_address = BASE_PALETTE_ADDR + offset_addr % 0xf20;
            m_vram.set_value_at(real_address, value);
        }
    } else {
        m_vram.set_value_at(real_address, value);
    }
}
