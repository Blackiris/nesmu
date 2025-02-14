#include "icontroller.h"

IController::IController() {}


void IController::set_strobe(const uint8_t& value) {
    m_strobe_status = value & 0b1;
    update_strobe_buffer();
}

bool IController::get_next_state() {
    bool value = m_strobe_buffer & 0b1;
    m_strobe_buffer >>= 1;
    return value;
}
