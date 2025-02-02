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

int PPU::get_vram_addr_incr() {
    return m_io_registers.get_bit_at(PPUCTRL, PPUCTRL_VRAM_ADDR_INCR) ? 32 : 1;
}
bool PPU::is_background_rendering_enable() {
    return m_io_registers.get_bit_at(PPUMASK, PPUMASK_BACKGROUND_ENABLED);
}
bool PPU::is_sprite_rendering_enable() {
    return m_io_registers.get_bit_at(PPUMASK, PPUMASK_SPRITE_ENABLED);
}
