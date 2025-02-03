#include "ram.h"

#include <iostream>

RAM::RAM(size_t size) : mem_array(size), m_size(size) {
}



unsigned char RAM::get_value_at(const uint16_t& address) {
    if (address >= m_size) {
        exit(1);
    }
    return mem_array[address];
}

void RAM::set_value_at(const uint16_t& address, const unsigned char& value) {
    if (address >= m_size) {
        exit(1);
    }
    mem_array[address] = value;
}

