#ifndef RAM_H
#define RAM_H

#include <stddef.h>
#include <stdint.h>
#include <vector>

struct ram_range {
    uint16_t start;
    uint16_t end; // end address, but not included in range
    uint16_t mirror_size;
};


class RAM
{
public:
    RAM(size_t size);
    RAM(size_t size, std::vector<ram_range> ranges);
    void set_memory_range(size_t from_address, std::vector<char> memory_to_set);

    unsigned char get_value_at(const uint16_t& address);
    void set_value_at(const uint16_t& address, const unsigned char& value);
    void set_value_at_zero_page(const unsigned char& address, const unsigned char& value);
private:
    std::vector<unsigned char> mem_array;
    std::vector<ram_range> m_ranges;

    uint16_t get_physical_address(const uint16_t& address);
};

#endif // RAM_H
