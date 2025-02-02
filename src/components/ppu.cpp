#include "ppu.h"



PPU::PPU(PPUIORegisters& io_registers, IMemory& vram, RAM& oam): m_io_registers(io_registers),
    m_ppu_mem_map(vram), m_oam(oam) {}

void PPU::set_vblank(bool enable) {
    m_io_registers.set_bit_at(PPUSTATUS, PPUSTATUS_VBLANK, enable);
}

bool PPU::maybe_send_nmi() {
    // Bit 7 of PPU Control Register 1
    return m_io_registers.get_bit_at(PPUCTRL, PPUCTRL_VBLANK_NMI);
}

void PPU::load_chr_rom(const ROM& rom) {
    m_ppu_mem_map.set_memory_range(0x0000, rom.chr_rom);
}

void PPU::set_oam_addr(unsigned char value) {
    m_io_registers.set_value_at(OAMADDR, value);
}

Frame PPU::render_frame() {
    Frame frame;

    for (int i=0; i<256; i++) {
        PatternTile pattern_tile = get_tile(i);
        display_tile_to_frame(pattern_tile, frame, (i%16)*8, (i/16)*8);
    }

    return frame;
}




int PPU::get_vram_addr_incr() {
    return m_io_registers.get_bit_at(PPUCTRL, PPUCTRL_VRAM_ADDR_INCR) ? 32 : 1;
}
bool PPU::is_background_rendering_enable() {
    return m_io_registers.get_bit_at(PPUMASK, PPUMASK_BACKGROUND_ENABLED);
}
bool PPU::is_sprite_rendering_enable() {
    return m_io_registers.get_bit_at(PPUMASK, PPUMASK_SPRITE_ENABLED);
}

PatternTile PPU::get_tile(const int& tile_number) {
    PatternTile pattern_tile;
    uint16_t first_addr = tile_number << 4;
    for (unsigned char j=0; j<8; j++) {
        for (unsigned char i=0; i<8; i++) {
            unsigned char pixel = 0;
            if (m_ppu_mem_map.get_bit_at(first_addr+j, 8-i)) {
                pixel++;
            }
            if (m_ppu_mem_map.get_bit_at(first_addr+j+8, 8-i)) {
                pixel += 2;
            }
            pattern_tile.pixels[i][j] = pixel;
        }
    }
    return pattern_tile;
}

void PPU::display_tile_to_frame(const PatternTile& pattern_tile, Frame& frame, int x, int y) {
    for (unsigned char j=0; j<8; j++) {
        for (unsigned char i=0; i<8; i++) {
            Color color;
            unsigned char pixel_indice = pattern_tile.pixels[i][j];
            color.r = pixel_indice * 0x70;
            color.g = pixel_indice * 0x70;
            color.b = pixel_indice * 0x70;
            frame.colors[x+i][y+j] = color;
        }
    }
}
