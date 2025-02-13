#ifndef ROMLOADER_H
#define ROMLOADER_H

#include <string>
#include <stdint.h>
#include <map>
#include "rom.h"

class RomLoader
{
public:
    RomLoader();

    ROM read_rom_from_disk(std::string filePath);
private:
    static const std::map<uint8_t, std::string> mappers;
};

#endif // ROMLOADER_H
