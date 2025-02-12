#ifndef RAM_H
#define RAM_H

#include <stddef.h>
#include <stdint.h>
#include <vector>
#include "IMemory.h"

struct ram_range {
    uint16_t start;
    uint16_t end; // end address, but not included in range
    uint16_t mirror_size;
};


class RAM : public IMemory
{
public:
    RAM(size_t size);

    unsigned char get_value_at(const uint16_t& address);
    void set_value_at(const uint16_t& address, const unsigned char& value);
private:
    std::vector<unsigned char> mem_array;
    size_t m_size;
};

#endif // RAM_H
