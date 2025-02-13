#ifndef CONTROLLER_H
#define CONTROLLER_H

#define BUTTON_A 0b1
#define BUTTON_B 0b10
#define BUTTON_SELECT 0b100
#define BUTTON_START 0b1000
#define BUTTON_UP 0b10000
#define BUTTON_DOWN 0b100000
#define BUTTON_LEFT 0b1000000
#define BUTTON_RIGHT 0b10000000

#include <stdint.h>

class Controller
{
public:
    Controller();

    void set_strobe(const uint8_t& value);
    void update_strobe_buffer();
    bool get_next_state();

private:
    uint8_t m_strobe_buffer;
    bool m_strobe_status = false;
};

#endif // CONTROLLER_H
