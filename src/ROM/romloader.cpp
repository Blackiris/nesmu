#include "romloader.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

const std::unordered_map<uint8_t, std::string> RomLoader::mappers = {
    {0, "NROM, no mapper"},
    {1, "Nin. MMC1"},
    {2, "UNROM switch"},
    {3, "CNROM switch"},
    {4, "Nin. MMC3"},
};

RomLoader::RomLoader() {}

ROM RomLoader::read_rom_from_disk(std::string filePath) {
    std::ifstream rom_file(filePath, std::ios_base::binary);
    std::cout << filePath << std::endl;

    std::unique_ptr<char[]> file_bytes;
    ROM rom;

    if (rom_file.is_open()) {
        // Get length of file
        rom_file.seekg(0, std::ios::end);
        size_t length = rom_file.tellg();
        rom_file.seekg(0, std::ios::beg);

        file_bytes = std::make_unique<char[]>(length);

        rom_file.read(file_bytes.get(), length);


        int prg_rom_size = file_bytes.get()[4];
        int chr_rom_size = file_bytes.get()[5];
        uint8_t flags6 = file_bytes.get()[6];


        char* prg_rom_start = file_bytes.get() + 16;
        char* prg_rom_end = prg_rom_start + 16384 * prg_rom_size;
        char* chr_rom_start = prg_rom_end;
        char* chr_rom_end = chr_rom_start + 8192 * chr_rom_size;

        rom.prg_rom.assign(prg_rom_start, prg_rom_end);
        rom.chr_rom.assign(chr_rom_start, chr_rom_end);

        char mapper = (flags6 & 0b11110000) >> 4;
        rom.mirror_mode = flags6 & 0b1;

        std::cout << "ROM " << prg_rom_size << " x 16k - CHR " << chr_rom_size << " x 8k" << std::endl;
        std::cout << mappers.at(mapper) << std::endl;
    }

    rom_file.close();
    return rom;
}
