#include "cpumemorymap.h"

CPUMemoryMap::CPUMemoryMap(ROM& rom, RAM& ram) : m_rom_banks(32768), m_ram(ram) {
    m_rom_banks.set_memory_range(0x0000, rom.prg_rom);
    if (rom.prg_rom.size() == 16384) {
        m_rom_banks.set_memory_range(0x4000, rom.prg_rom);
    }
}

unsigned char CPUMemoryMap::get_value_at(const uint16_t& address) {
    unsigned char value;
    if (address >= 0x8000) {
        value = m_rom_banks.get_value_at(address - 0x8000);
    } else {
        value = m_ram.get_value_at(address);
    }
    return value;
}
void CPUMemoryMap::set_value_at(const uint16_t& address, const unsigned char& value) {
    if (address >= 0x8000) {
        // No write on ROM banks
    } else {
        m_ram.set_value_at(address, value);
    }
}
