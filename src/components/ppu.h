#ifndef PPU_H
#define PPU_H

#include "ram.h"
#include "ppuioregisters.h"
#include "../ROM/rom.h"
#include "../frame.h"


struct PatternTile {
    unsigned char pixels[8][8];
};

class PPU
{
public:
    PPU(PPUIORegisters& io_registers, IMemory& vram, RAM& oam);
    void set_vblank(bool enable);
    bool maybe_send_nmi();
    void load_chr_rom(const ROM& rom);
    void set_oam_addr(unsigned char value);
    Frame render_frame();

private:
    PPUIORegisters& m_io_registers;
    IMemory& m_ppu_mem_map;
    unsigned char vram_addr;
    RAM& m_oam;


    bool is_background_rendering_enable();
    bool is_sprite_rendering_enable();

    PatternTile get_tile(const int& tile_number);
    void display_tile_to_frame(const PatternTile& tile, Frame& frame, int x, int y);

    void render_background(Frame& frame);
    void render_sprites(Frame& frame);
};

#endif // PPU_H
