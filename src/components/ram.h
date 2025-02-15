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
    explicit RAM(size_t size);

    uint8_t get_value_at(const uint16_t& address) override;
    void set_value_at(const uint16_t& address, const uint8_t& value) override;
private:
    std::vector<uint8_t> mem_array;
    size_t m_size;
};

#endif // RAM_H
