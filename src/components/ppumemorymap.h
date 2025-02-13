#ifndef PPUMEMORYMAP_H
#define PPUMEMORYMAP_H

#include "../ROM/rom.h"
#include "IMemory.h"
#include "ram.h"
#include "pattern_tile.h"

class PPUMemoryMap : public IMemory
{
public:
    PPUMemoryMap(ROM& rom, RAM& vram);
    ~PPUMemoryMap();

    uint8_t get_value_at(const uint16_t& address) override;
    void set_value_at(const uint16_t& address, const uint8_t& value) override;

    PatternTile get_pattern_tile(const int& tile_number);

private:
    RAM& m_vram;
    PatternTile* m_pattern_tiles_cache;

    PatternTile load_pattern_tile(const int& tile_number);
};

#endif // PPUMEMORYMAP_H
