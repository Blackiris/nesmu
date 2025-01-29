#ifndef RAM_H
#define RAM_H

#include <stddef.h>
#include <stdint.h>
#include <vector>

class RAM
{
public:
    RAM(size_t size);
    void set_memory_range(size_t from_address, std::vector<char> memory_to_set);

    unsigned char get_value_at(const uint16_t& address);
    void set_value_at(const uint16_t& address, const unsigned char& value);
    void set_value_at_zero_page(const unsigned char& address, const unsigned char& value);
private:
    std::vector<unsigned char> mem_array;
};

#endif // RAM_H
