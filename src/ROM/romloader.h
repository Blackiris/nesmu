#pragma once

#include "rom.h"
#include <stdint.h>
#include <string>
#include <unordered_map>

class RomLoader
{
public:
    RomLoader();

    ROM read_rom_from_disk(std::string filePath);
private:
    static const std::unordered_map<uint8_t, std::string> mappers;
};
