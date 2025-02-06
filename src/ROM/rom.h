#ifndef ROM_H
#define ROM_H

#include <vector>

struct ROM {
    std::vector<char> prg_rom;
    std::vector<char> chr_rom;
    bool mirror_mode; //false: vertical, true: horizontal
};

#endif // ROM_H
