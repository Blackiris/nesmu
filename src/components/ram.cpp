#include "ram.h"

RAM::RAM(size_t size) : mem_array(size) {
}

void RAM::set_memory_range(size_t from_address, std::vector<char> memory_to_set) {
    for (size_t i=0; i<memory_to_set.size(); i++) {
        mem_array[from_address + i] = memory_to_set[i];
    }
}

unsigned char RAM::get_value_at(const uint16_t& address) {
    return mem_array[address];
}

void RAM::set_value_at(const uint16_t& address, const unsigned char& value) {
    mem_array[address] = value;
}

void RAM::set_value_at_zero_page(const unsigned char& address, const unsigned char& value) {
    mem_array[address] = value;
}
