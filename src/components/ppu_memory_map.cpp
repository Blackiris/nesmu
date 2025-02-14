#include "ppu_memory_map.h"

#define BASE_PALETTE_ADDR 0x3f00

PPUMemoryMap::PPUMemoryMap(ROM& rom, RAM& vram): m_vram(vram) {
    m_vram.set_memory_range(0x0000, rom.chr_rom);

    for (int i=0; i<512; i++) {
        m_pattern_tiles_cache[i] = load_pattern_tile(i);
    }
}

PPUMemoryMap::~PPUMemoryMap() {
}

PatternTile PPUMemoryMap::get_pattern_tile(const int& tile_number) {
    return m_pattern_tiles_cache[tile_number];
}

PatternTile PPUMemoryMap::load_pattern_tile(const int& tile_number) {
    PatternTile pattern_tile;
    uint16_t first_addr = (tile_number << 4);
    for (uint8_t j=0; j<8; j++) {
        for (uint8_t i=0; i<8; i++) {
            uint8_t pixel = 0;
            if (get_bit_at(first_addr+j, 7-i)) {
                pixel++;
            }
            if (get_bit_at(first_addr+j+8, 7-i)) {
                pixel += 2;
            }
            pattern_tile.pixels[i][j] = pixel;
        }
    }
    return pattern_tile;
}

uint8_t PPUMemoryMap::get_value_at(const uint16_t& address) {
    uint16_t real_address = address & 0x3fff;
    if (real_address >= 0x3f00) {
        real_address = 0x3f00 + (real_address - 0x3f00) % 0xf20;
    }

    return m_vram.get_value_at(real_address);
}
void PPUMemoryMap::set_value_at(const uint16_t& address, const uint8_t& value) {
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
