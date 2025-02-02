#ifndef PPUIOREGISTERS_H
#define PPUIOREGISTERS_H

#include "ram.h"

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

#define OAMADDR 0x3

#define OAMDATA 0x4

#define OAMDMA 0x8

class PPUIORegisters : public RAM
{
public:
    PPUIORegisters(RAM& oam);

    void set_cpu_memory_map(IMemory* cpu_memory_map);

    unsigned char get_value_at(const uint16_t& address);
    void set_value_at(const uint16_t& address, const unsigned char& value);

private:
    RAM& m_oam;
    IMemory* m_cpu_memory_map;

    void set_oam_data(const unsigned char& value);
};

#endif // PPUIOREGISTERS_H
