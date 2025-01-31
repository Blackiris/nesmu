#include "ram.h"

#include <iostream>

RAM::RAM(size_t size) : mem_array(size) {
}



unsigned char RAM::get_value_at(const uint16_t& address) {
    return mem_array[address];
}

void RAM::set_value_at(const uint16_t& address, const unsigned char& value) {
    mem_array[address] = value;
}

