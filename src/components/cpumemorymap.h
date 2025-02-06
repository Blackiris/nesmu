#ifndef CPUMEMORYMAP_H
#define CPUMEMORYMAP_H

#include "../ROM/rom.h"
#include "imemory.h"
#include "ram.h"
#include "controller.h"

#define CPU_SPRDMA 0x4014
#define CPU_JOYPAD1 0x4016
#define CPU_JOYPAD2 0x4017

class CPUMemoryMap : public IMemory
{
public:
    CPUMemoryMap(ROM& rom, RAM& ram, RAM& io_registers, RAM& papu_io_registers,
                 Controller* controller1, Controller* controller2);

    unsigned char get_value_at(const uint16_t& address);
    void set_value_at(const uint16_t& address, const unsigned char& value);

private:
    RAM m_rom_banks;
    RAM& m_ram;
    RAM& m_io_registers;
    RAM& m_papu_io_registers;

    Controller* m_controller1;
    Controller* m_controller2;
};

#endif // CPUMEMORYMAP_H
