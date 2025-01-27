#ifndef RAM_H
#define RAM_H

#include <stddef.h>
#include <vector>

class RAM
{
public:
    RAM(size_t size);

private:
    std::vector<char> mem_array;
};

#endif // RAM_H
