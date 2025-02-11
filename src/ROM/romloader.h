#ifndef ROMLOADER_H
#define ROMLOADER_H

#include <string>
#include <map>
#include "rom.h"

class RomLoader
{
public:
    RomLoader();

    ROM read_rom_from_disk(std::string filePath);
private:
    static const std::map<unsigned char, std::string> mappers;
};

#endif // ROMLOADER_H
