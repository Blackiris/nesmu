#ifndef PPU_H
#define PPU_H

#include "ram.h"
#include "cpu.h"
#include "../ROM/rom.h"

class PPU
{
public:
    PPU(RAM& ram, CPU& cpu);
    void set_vblank(bool enable);
    void maybe_send_nmi();
    void load_chr_rom(const ROM& rom);

private:
    RAM& m_ram;
    CPU& m_cpu;
    RAM m_vram;
};

#endif // PPU_H
