#ifndef ROMLOADER_H
#define ROMLOADER_H

#include <string>
#include "rom.h"

class RomLoader
{
public:
    RomLoader();

    ROM loadRom(std::string filePath);
};

#endif // ROMLOADER_H
