#pragma once

#define BUTTON_A 0b1
#define BUTTON_B 0b10
#define BUTTON_SELECT 0b100
#define BUTTON_START 0b1000
#define BUTTON_UP 0b10000
#define BUTTON_DOWN 0b100000
#define BUTTON_LEFT 0b1000000
#define BUTTON_RIGHT 0b10000000

#include <stdint.h>

class IController
{
public:
    IController();

    void set_strobe(const uint8_t& value);
    bool get_next_state();
    virtual void update_strobe_buffer() = 0;

protected:
    uint8_t m_strobe_buffer = 0;
    bool m_strobe_status = false;
};
