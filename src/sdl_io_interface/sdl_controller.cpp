#include "sdl_controller.h"


SDLController::SDLController(const SDL_Scancode &btn_a, const SDL_Scancode &btn_b,
                             const SDL_Scancode &btn_select, const SDL_Scancode &btn_start,
                             const SDL_Scancode &btn_up, const SDL_Scancode &btn_down,
                             const SDL_Scancode &btn_left, const SDL_Scancode &btn_right) :
    m_btn_a(btn_a), m_btn_b(btn_b), m_btn_select(btn_select), m_btn_start(btn_start), m_btn_up(btn_up), m_btn_down(btn_down),
    m_btn_left(btn_left), m_btn_right(btn_right) {}


void SDLController::update_strobe_buffer() {
    if (m_strobe_status) {
        m_strobe_buffer = 0;
        const bool * keys = SDL_GetKeyboardState(NULL);
        if (keys[m_btn_a]) {
            m_strobe_buffer |= BUTTON_A;
        }
        if (keys[m_btn_b]) {
            m_strobe_buffer |= BUTTON_B;
        }

        if (keys[m_btn_select]) {
            m_strobe_buffer |= BUTTON_SELECT;
        }
        if (keys[m_btn_start]) {
            m_strobe_buffer |= BUTTON_START;
        }

        if (keys[m_btn_up]) {
            m_strobe_buffer |= BUTTON_UP;
        }
        if (keys[m_btn_down]) {
            m_strobe_buffer |= BUTTON_DOWN;
        }
        if (keys[m_btn_left]) {
            m_strobe_buffer |= BUTTON_LEFT;
        }
        if (keys[m_btn_right]) {
            m_strobe_buffer |= BUTTON_RIGHT;
        }
    }
}
