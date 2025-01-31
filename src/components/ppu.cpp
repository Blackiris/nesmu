#include "ppu.h"

PPU::PPU(RAM& ram, CPU& cpu): m_ram(ram),
    m_cpu(cpu), m_vram(RAM(16384)) {}

void PPU::set_vblank(bool enable) {
    m_ram.set_bit_at(0x2002, 7, enable);
}

void PPU::maybe_send_nmi() {
    if (m_ram.get_bit_at(0x2002, 7)) {
        // send NMI
        m_cpu.set_nmi();
    }
}

void PPU::load_chr_rom(const ROM& rom) {
    m_vram.set_memory_range(0x0000, rom.chr_rom);
}
