#ifndef PPU_H
#define PPU_H

#include "ram.h"
#include "cpu.h"
#include "../ROM/rom.h"

class PPU
{
public:
    PPU(RAM& io_registers, CPU& cpu, IMemory& vram);
    void set_vblank(bool enable);
    void maybe_send_nmi();
    void load_chr_rom(const ROM& rom);

private:
    RAM& m_io_registers;
    CPU& m_cpu;
    IMemory& m_ppu_mem_map;
    unsigned char vram_addr;

    int get_vram_addr_incr();
};

#endif // PPU_H
