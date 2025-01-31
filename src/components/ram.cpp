#include "ram.h"

#include <iostream>

RAM::RAM(size_t size) : mem_array(size), m_ranges({}) {
}

RAM::RAM(size_t size, std::vector<ram_range> ranges) : mem_array(size), m_ranges(ranges) {
}

unsigned char RAM::get_value_at(const uint16_t& address) {
    return mem_array[get_physical_address(address)];
}

uint16_t RAM::get_physical_address(const uint16_t& address) {
    uint16_t real_address = address;

    for (const auto& range : m_ranges) {
        if (address >= range.start && address < range.end) {
            real_address = range.start + (real_address - range.start) % range.mirror_size;
            break;
        }
    }

    //std::cout << std::endl << "Addr: " << std::hex << static_cast<int>(real_address & 0xffff) << std::endl;
    return real_address;
}

void RAM::set_value_at(const uint16_t& address, const unsigned char& value) {
    mem_array[get_physical_address(address)] = value;
}

