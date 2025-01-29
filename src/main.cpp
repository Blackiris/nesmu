#include <iostream>
#include "ROM/romloader.h"
#include "components/cpu.h"
#include "SDL3/SDL.h"


int main()
{
    RomLoader romLoader;
    ROM rom = romLoader.read_rom_from_disk("SMB.nes");
    CPU cpu;
    cpu.init();
    cpu.load_rom(rom);

    while(cpu.exec_cycle()) {

    }

    return 0;
}
