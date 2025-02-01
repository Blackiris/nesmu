#include "ppumemorymap.h"

PPUMemoryMap::PPUMemoryMap(ROM& rom): m_vram(RAM(16384)) {
    m_vram.set_memory_range(0x0000, rom.chr_rom);
}

unsigned char PPUMemoryMap::get_value_at(const uint16_t& address) {
    uint16_t real_address = address & 0x3fff;
    return m_vram.get_value_at(real_address);
}
void PPUMemoryMap::set_value_at(const uint16_t& address, const unsigned char& value) {
    uint16_t real_address = address & 0x3fff;
    m_vram.set_value_at(real_address, value);
}
