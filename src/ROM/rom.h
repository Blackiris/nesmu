#ifndef ROM_H
#define ROM_H

#include <vector>

struct ROM {
    std::vector<char> prg_rom;
    std::vector<char> chr_rom;
};

#endif // ROM_H
