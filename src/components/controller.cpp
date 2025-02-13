#include "controller.h"

#include <SDL3/SDL.h>

Controller::Controller() {}

void Controller::set_strobe(const uint8_t& value) {
    m_strobe_status = value & 0b1;
    update_strobe_buffer();
}

bool Controller::get_next_state() {
    bool value = m_strobe_buffer & 0b1;
    m_strobe_buffer >>= 1;
    return value;
}


void Controller::update_strobe_buffer() {
    if (m_strobe_status) {
        m_strobe_buffer = 0;
        const bool * keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_X]) {
            m_strobe_buffer |= BUTTON_A;
        }
        if (keys[SDL_SCANCODE_Z]) {
            m_strobe_buffer |= BUTTON_B;
        }

        if (keys[SDL_SCANCODE_RSHIFT]) {
            m_strobe_buffer |= BUTTON_SELECT;
        }
        if (keys[SDL_SCANCODE_RETURN]) {
            m_strobe_buffer |= BUTTON_START;
        }

        if (keys[SDL_SCANCODE_UP]) {
            m_strobe_buffer |= BUTTON_UP;
        }
        if (keys[SDL_SCANCODE_DOWN]) {
            m_strobe_buffer |= BUTTON_DOWN;
        }
        if (keys[SDL_SCANCODE_LEFT]) {
            m_strobe_buffer |= BUTTON_LEFT;
        }
        if (keys[SDL_SCANCODE_RIGHT]) {
            m_strobe_buffer |= BUTTON_RIGHT;
        }
    }
}
