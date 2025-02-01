#include "ppu.h"

#define PPUCTRL 0x0
#define PPUCTRL_VRAM_ADDR_INCR 3
#define PPUCTRL_SPRITE_SIZE 5
#define PPUCTRL_VBLANK_NMI 7

#define PPUMASK 0x1
#define PPUMASK_BACKGROUND_ENABLED 3
#define PPUMASK_SPRITE_ENABLED 4

#define PPUSTATUS 0x2
#define PPUSTATUS_SPRINT_0 6
#define PPUSTATUS_VBLANK 7

PPU::PPU(RAM& io_registers, CPU& cpu, IMemory& vram): m_io_registers(io_registers),
    m_cpu(cpu), m_ppu_mem_map(vram) {}

void PPU::set_vblank(bool enable) {
    m_io_registers.set_bit_at(PPUSTATUS, PPUSTATUS_VBLANK, enable);
}

void PPU::maybe_send_nmi() {
    // Bit 7 of PPU Control Register 1
    if (m_io_registers.get_bit_at(PPUCTRL, PPUCTRL_VBLANK_NMI)) {
        // send NMI
        m_cpu.set_nmi();
    }
}

void PPU::load_chr_rom(const ROM& rom) {
    m_ppu_mem_map.set_memory_range(0x0000, rom.chr_rom);
}

int PPU::get_vram_addr_incr() {
    return m_io_registers.get_bit_at(PPUCTRL, PPUCTRL_VRAM_ADDR_INCR) ? 32 : 1;
}
