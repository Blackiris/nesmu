#pragma once

#include "../io_interface/icontroller.h"
#include <SDL3/SDL.h>

class SDLController : public IController
{
public:
    SDLController(const SDL_Scancode &btn_a, const SDL_Scancode &btn_b,
                  const SDL_Scancode &btn_select, const SDL_Scancode &btn_start,
                  const SDL_Scancode &btn_up, const SDL_Scancode &btn_down,
                  const SDL_Scancode &btn_left, const SDL_Scancode &btn_right);

    void update_strobe_buffer() override;

private:
    SDL_Scancode m_btn_a, m_btn_b, m_btn_select, m_btn_start, m_btn_up, m_btn_down,
        m_btn_left, m_btn_right;
};
