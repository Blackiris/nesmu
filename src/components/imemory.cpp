#include "imemory.h"

void IMemory::set_bit_at(const uint16_t& address, short bit_nb, bool enable) {
    unsigned char value = get_value_at(address);
    if (enable) {
        switch(bit_nb) {
        case 0:
            value |= 0b00000001;
            break;
        case 1:
            value |= 0b00000010;
            break;
        case 2:
            value |= 0b00000100;
            break;
        case 3:
            value |= 0b00001000;
            break;
        case 4:
            value |= 0b00010000;
            break;
        case 5:
            value |= 0b00100000;
            break;
        case 6:
            value |= 0b01000000;
            break;
        case 7:
            value |= 0b10000000;
            break;
        }

    } else {

        switch(bit_nb) {
        case 0:
            value &= 0b11111110;
            break;
        case 1:
            value &= 0b11111101;
            break;
        case 2:
            value &= 0b11111011;
            break;
        case 3:
            value &= 0b11110111;
            break;
        case 4:
            value &= 0b11101111;
            break;
        case 5:
            value &= 0b11011111;
            break;
        case 6:
            value &= 0b10111111;
            break;
        case 7:
            value &= 0b01111111;
            break;
        }
    }

    set_value_at(address, value);
}

bool IMemory::get_bit_at(const uint16_t& address, short bit_nb) {
    unsigned char value = get_value_at(address);
    return ((value >> bit_nb) & 0b1) > 0;
}

void IMemory::set_memory_range(size_t from_address, const std::vector<char>& memory_to_set) {
    for (size_t i=0; i<memory_to_set.size(); i++) {
        set_value_at(from_address + i, memory_to_set[i]);
    }
}

