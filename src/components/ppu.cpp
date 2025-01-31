#include "ppu.h"

PPU::PPU(RAM& io_registers, CPU& cpu): m_io_registers(io_registers),
    m_cpu(cpu), m_vram(RAM(16384)) {}

void PPU::set_vblank(bool enable) {
    m_io_registers.set_bit_at(0x2, 7, enable);
}

void PPU::maybe_send_nmi() {
    // Bit 7 of PPU Control Register 1
    if (m_io_registers.get_bit_at(0x0, 7)) {
        // send NMI
        m_cpu.set_nmi();
    }
}

void PPU::load_chr_rom(const ROM& rom) {
    m_vram.set_memory_range(0x0000, rom.chr_rom);
}
