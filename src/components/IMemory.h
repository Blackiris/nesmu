#ifndef IMEMORY_H
#define IMEMORY_H

#include <stddef.h>
#include <stdint.h>
#include <vector>

class IMemory
{
public:
    virtual uint8_t get_value_at(const uint16_t& address) = 0;
    virtual void set_value_at(const uint16_t& address, const uint8_t& value) = 0;
    void set_bit_at(const uint16_t& address, short bit_nb, bool enable);
    bool get_bit_at(const uint16_t& address, short bit_nb);

    void set_memory_range(size_t from_address, const std::vector<char>& memory_to_set);

};

#endif // IMEMORY_H
