#include <iostream>
#include "ROM/romloader.h"


int main()
{
    RomLoader romLoader;
    romLoader.loadRom("SMB.nes");


    return 0;
}
