#include "ppu.h"

#include <iostream>

const std::map<unsigned char, Color> PPU::color_palette = {
    {0x00, Color(0x75, 0x75, 0x75)},
    {0x01, Color(0x27, 0x1b, 0x8f)},
    {0x02, Color(0x00, 0x00, 0xab)},
    {0x03, Color(0x47, 0x00, 0x9f)},
    {0x04, Color(0x8f, 0x00, 0x77)},
    {0x05, Color(0xab, 0x00, 0x13)},
    {0x06, Color(0xa7, 0x00, 0x00)},
    {0x07, Color(0x7f, 0x0b, 0x00)},
    {0x08, Color(0x43, 0x2f, 0x00)},
    {0x09, Color(0x00, 0x47, 0x00)},
    {0x0a, Color(0x00, 0x51, 0x00)},
    {0x0b, Color(0x00, 0x3f, 0x17)},
    {0x0c, Color(0x1b, 0x3f, 0x5f)},
    {0x0d, Color(0x00, 0x00, 0x00)},
    {0x0e, Color(0x00, 0x00, 0x00)},
    {0x0f, Color(0x00, 0x00, 0x00)},
    {0x10, Color(0xbc, 0xbc, 0xbc)},
    {0x11, Color(0x00, 0x73, 0xef)},
    {0x12, Color(0x23, 0x3b, 0xef)},
    {0x13, Color(0x83, 0x00, 0xf3)},
    {0x14, Color(0xbf, 0x00, 0xbf)},
    {0x15, Color(0xe7, 0x00, 0x5b)},
    {0x16, Color(0xbd, 0x2b, 0x00)},
    {0x17, Color(0xcb, 0x4f, 0x0f)},
    {0x18, Color(0x8b, 0x73, 0x00)},
    {0x19, Color(0x00, 0x97, 0x00)},
    {0x1a, Color(0x00, 0xab, 0x00)},
    {0x1b, Color(0x00, 0x93, 0x3b)},
    {0x1c, Color(0x00, 0x83, 0x8b)},
    {0x1d, Color(0x00, 0x00, 0x00)},
    {0x1e, Color(0x00, 0x00, 0x00)},
    {0x1f, Color(0x00, 0x00, 0x00)},

    {0x20, Color(0xff, 0xff, 0xff)},
    {0x21, Color(0x3f, 0xbf, 0xff)},
    {0x22, Color(0x5f, 0x97, 0xff)},
    {0x23, Color(0xa7, 0x8b, 0xfd)},
    {0x24, Color(0xf7, 0x7b, 0xff)},
    {0x25, Color(0xff, 0x77, 0xb7)},
    {0x26, Color(0xff, 0x77, 0x63)},
    {0x27, Color(0xff, 0x9b, 0x3b)},
    {0x28, Color(0xf3, 0xbf, 0x3f)},
    {0x29, Color(0x83, 0xd3, 0x13)},
    {0x2a, Color(0x4f, 0xdf, 0x4b)},
    {0x2b, Color(0x58, 0xf8, 0x98)},
    {0x2c, Color(0x00, 0xeb, 0xdb)},
    {0x2d, Color(0x00, 0x00, 0x00)},
    {0x2e, Color(0x00, 0x00, 0x00)},
    {0x2f, Color(0x00, 0x00, 0x00)},
    {0x30, Color(0xff, 0xff, 0xff)},
    {0x31, Color(0xab, 0xe7, 0xff)},
    {0x32, Color(0xc7, 0xd7, 0xff)},
    {0x33, Color(0xd7, 0xcb, 0xff)},
    {0x34, Color(0xff, 0xc7, 0xff)},
    {0x35, Color(0xff, 0xc7, 0xdb)},
    {0x36, Color(0xff, 0xbf, 0xb3)},
    {0x37, Color(0xff, 0xdb, 0xab)},
    {0x38, Color(0xff, 0xe7, 0xa3)},
    {0x39, Color(0xe3, 0xff, 0xa3)},
    {0x3a, Color(0xab, 0xf3, 0xbf)},
    {0x3b, Color(0xb3, 0xff, 0xcf)},
    {0x3c, Color(0x9f, 0xff, 0xf3)},
    {0x3d, Color(0x00, 0x00, 0x00)},
    {0x3e, Color(0x00, 0x00, 0x00)},
    {0x3f, Color(0x00, 0x00, 0x00)},
};

PPU::PPU(PPUIORegisters& io_registers, IMemory& ppu_mem_map, RAM& oam): m_io_registers(io_registers),
    m_ppu_mem_map(ppu_mem_map), m_oam(oam) {}

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

    render_background(frame);
    render_sprites(frame);

    return frame;
}

void PPU::render_background(Frame& frame) {
    if (is_background_rendering_enable()) {

        uint16_t name_table_addr = 0x2000;
        uint16_t attr_table_addr = name_table_addr + 0x3c0;
        uint16_t bg_pattern_table = get_background_pattern_table_addr();
        for (int j=0; j<30; j++) {
            for (int i=0; i<32; i++) {
                unsigned char tile_id = m_ppu_mem_map.get_value_at(name_table_addr+i+j*32);
                PatternTile pattern_tile = get_pattern_tile(bg_pattern_table, tile_id);
                unsigned char palette_byte = m_ppu_mem_map.get_value_at(attr_table_addr + (i/4) + (j/4)*8);
                unsigned char palette = palette_byte;
                if (j%2 == 0) {
                    palette &= 0b00001111;
                } else {
                    palette >>= 4;
                }
                if (i%2 == 0) {
                    palette &= 0b0011;
                } else {
                    palette >>= 2;
                }
                display_tile_to_frame(pattern_tile, frame, palette, true, i*8, j*8, false, false);
            }
        }
        /*for (int i=0; i<256; i++) {
            PatternTile pattern_tile = get_pattern_tile(0x0, i);
            display_tile_to_frame(pattern_tile, frame, 0, true, (i%16)*8, (i/16)*8, false, false);
        }*/
    }
}

void PPU::render_sprites(Frame& frame) {
    if (is_sprite_rendering_enable()) {
        uint16_t spr_pattern_table = get_sprite_pattern_table_addr();
        for (int i = 63; i>=0; i--) {
            unsigned char x = m_oam.get_value_at(i*4+3);
            unsigned char y = m_oam.get_value_at(i*4);
            unsigned char tile_id = m_oam.get_value_at(i*4+1);
            unsigned char attributes = m_oam.get_value_at(i*4+2);
            bool flip_h = (attributes & 0b01000000) > 0;
            bool flip_v = (attributes & 0b10000000) > 0;
            unsigned char palette = attributes & 0b11;

            PatternTile pattern_tile = get_pattern_tile(spr_pattern_table, tile_id);
            display_tile_to_frame(pattern_tile, frame, palette, false, x, y-1, flip_h, flip_v);
        }

    }
}




bool PPU::is_background_rendering_enable() {
    return m_io_registers.get_bit_at(PPUMASK, PPUMASK_BACKGROUND_ENABLED);
}
bool PPU::is_sprite_rendering_enable() {
    return m_io_registers.get_bit_at(PPUMASK, PPUMASK_SPRITE_ENABLED);
}

PatternTile PPU::get_pattern_tile(const uint16_t& pattern_table_addr, const int& tile_number) {
    PatternTile pattern_tile;
    uint16_t first_addr = pattern_table_addr + (tile_number << 4);
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

void PPU::display_tile_to_frame(const PatternTile& pattern_tile, Frame& frame, const unsigned char& palette, bool is_background, int x, int y, bool flip_h, bool flip_v) {
    uint16_t base_palette_addr = (is_background ? 0x3f00 : 0x3f10) + palette * 0x4;
    for (unsigned char j=0; j<8; j++) {
        for (unsigned char i=0; i<8; i++) {

            unsigned char final_i = flip_h ? 8-i : i;
            unsigned char final_j = flip_v ? 8-j : j;
            unsigned char pixel_indice = pattern_tile.pixels[final_i][final_j];

            if (pixel_indice > 0) {
                unsigned char color_indice = m_ppu_mem_map.get_value_at(base_palette_addr + pixel_indice);
                Color color = color_palette.at(color_indice);

                if (x+i < frame.width && y+j < frame.height) {
                    frame.colors[x+i][y+j] = color;
                }
            }

            /*Color color;
            color.r = pixel_indice * 0x70;
            color.g = pixel_indice * 0x70;
            color.b = pixel_indice * 0x70;
            if (x+i < frame.width && y+j < frame.height) {
                frame.colors[x+i][y+j] = color;
            }*/
        }
    }
}

uint16_t PPU::get_background_pattern_table_addr() {
    return m_io_registers.get_bit_at(PPUCTRL, PPUCTRL_BACKGROUND_PATTERN_TABLE) ? 0x1000 : 0x0;
}

uint16_t PPU::get_sprite_pattern_table_addr() {
    return m_io_registers.get_bit_at(PPUCTRL, PPUCTRL_SPRITE_PATTERN_TABLE) ? 0x1000 : 0x0;
}

