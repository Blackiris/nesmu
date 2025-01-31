#ifndef CPUMEMORYMAP_H
#define CPUMEMORYMAP_H

#include "../ROM/rom.h"
#include "imemory.h"
#include "ram.h"

class CPUMemoryMap : public IMemory
{
public:
    CPUMemoryMap(ROM& rom, RAM& ram, RAM& io_registers);

    unsigned char get_value_at(const uint16_t& address);
    void set_value_at(const uint16_t& address, const unsigned char& value);

private:
    RAM m_rom_banks;
    RAM& m_ram;
    RAM& m_io_registers;
};

#endif // CPUMEMORYMAP_H
