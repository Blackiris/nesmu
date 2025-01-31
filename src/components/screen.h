#ifndef SCREEN_H
#define SCREEN_H

#include <SDL3/SDL.h>
#include "../frame.h"

class Screen
{
public:
    Screen(SDL_Renderer* renderer);
    void render_frame(const Frame& frame);
private:
    SDL_Renderer *m_renderer = nullptr;
};

#endif // SCREEN_H
