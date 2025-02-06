#ifndef PPU_H
#define PPU_H

#include "ram.h"
#include <map>

#include "ppuioregisters.h"
#include "../ROM/rom.h"
#include "../frame.h"


struct PatternTile {
    unsigned char pixels[8][8];
};

class PPU
{
public:
    PPU(PPUIORegisters& io_registers, IMemory& ppu_mem_map, RAM& oam);
    void set_vblank(bool enable);
    bool maybe_send_nmi();
    void load_chr_rom(const ROM& rom);
    void set_oam_addr(unsigned char value);
    Frame render_frame();

private:
    static const std::map<unsigned char, Color> color_palette;
    PPUIORegisters& m_io_registers;
    IMemory& m_ppu_mem_map;
    unsigned char vram_addr;
    RAM& m_oam;

    bool is_background_rendering_enable();
    bool is_sprite_rendering_enable();

    PatternTile get_pattern_tile(const uint16_t& pattern_table_addr, const int& tile_number);
    void display_tile_to_frame(const PatternTile& tile, Frame& frame, const unsigned char& palette, bool is_background, int x, int y, bool flip_h, bool flip_v);

    void draw_backdrop_color(Frame& frame);
    void render_background(Frame& frame);
    void render_sprites(Frame& frame);

    uint16_t get_background_pattern_table_addr();
    uint16_t get_sprite_pattern_table_addr();
};

#endif // PPU_H
