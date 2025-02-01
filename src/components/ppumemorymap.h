#ifndef PPUMEMORYMAP_H
#define PPUMEMORYMAP_H

#include "../ROM/rom.h"
#include "imemory.h"
#include "ram.h"


class PPUMemoryMap : public IMemory
{
public:
    PPUMemoryMap(ROM& rom);

    unsigned char get_value_at(const uint16_t& address);
    void set_value_at(const uint16_t& address, const unsigned char& value);

private:
    RAM m_vram;
};

#endif // PPUMEMORYMAP_H
