#pragma once

#include <array>

#include "../ROM/rom.h"
#include "imemory.h"
#include "ram.h"
#include "pattern_tile.h"

class PPUMemoryMap : public IMemory
{
public:
    PPUMemoryMap(const ROM& rom, RAM& vram);
    ~PPUMemoryMap();

    uint8_t get_value_at(const uint16_t& address) override;
    void set_value_at(const uint16_t& address, const uint8_t& value) override;

    PatternTile get_pattern_tile(const int& tile_number);

private:
    RAM& m_vram;
    std::array<PatternTile, 256*2> m_pattern_tiles_cache;

    PatternTile load_pattern_tile(const int& tile_number);
};
