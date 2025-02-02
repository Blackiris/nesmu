#ifndef PPU_H
#define PPU_H

#include "ram.h"
#include "ppuioregisters.h"
#include "../ROM/rom.h"

class PPU
{
public:
    PPU(PPUIORegisters& io_registers, IMemory& vram, RAM& oam);
    void set_vblank(bool enable);
    bool maybe_send_nmi();
    void load_chr_rom(const ROM& rom);
    void set_oam_addr(unsigned char value);

private:
    PPUIORegisters& m_io_registers;
    IMemory& m_ppu_mem_map;
    unsigned char vram_addr;
    RAM& m_oam;

    int get_vram_addr_incr();
    bool is_background_rendering_enable();
    bool is_sprite_rendering_enable();
};

#endif // PPU_H
