#include "cpumemorymap.h"

CPUMemoryMap::CPUMemoryMap(ROM& rom, RAM& ram, RAM& io_registers, RAM& papu_io_registers, Controller* controller1, Controller* controller2)
    : m_rom_banks(32768), m_ram(ram), m_io_registers(io_registers), m_papu_io_registers(papu_io_registers),
    m_controller1(controller1), m_controller2(controller2) {

    m_rom_banks.set_memory_range(0x0000, rom.prg_rom);
    if (rom.prg_rom.size() == 16384) {
        m_rom_banks.set_memory_range(0x4000, rom.prg_rom);
    }


    // Init ram 0x0 to 0x800 excluded
    for (int16_t i=0x0; i<0x800; i++) {
        unsigned char val = (i/4)%2 ? 0xff : 0x00;
        m_ram.set_value_at(i, val);
    }
}

unsigned char CPUMemoryMap::get_value_at(const uint16_t& address) {
    unsigned char value;
    if (address >= 0x8000) {
        value = m_rom_banks.get_value_at(address - 0x8000);
    } else if (address == CPU_JOYPAD1) {
        value = m_controller1->get_next_state() * 0b1;
    } else if (address == CPU_JOYPAD2) {
    } else if (address >= 0x2000 && address < 0x4000) {
        value = m_io_registers.get_value_at((address - 0x2000) % 0x8);
    } else {
        value = m_ram.get_value_at(address % 0x0800);
    }
    return value;
}
void CPUMemoryMap::set_value_at(const uint16_t& address, const unsigned char& value) {
    if (address >= 0x8000) {
        // No write on ROM banks
    } else if (address == CPU_JOYPAD1) {
        m_controller1->set_strobe(value);
    } else if (address == CPU_JOYPAD2) {
    } else if (address == CPU_SPRDMA) {
        m_io_registers.set_value_at(0x8, value);
    } else if (address >= 0x4000 && address < 0x4016) {
        m_papu_io_registers.set_value_at(address - 0x4000, value);
    } else if (address >= 0x2000 && address < 0x4000) {
        m_io_registers.set_value_at((address - 0x2000) % 0x8, value);
    } else {
        m_ram.set_value_at(address % 0x0800, value);
    }
}
