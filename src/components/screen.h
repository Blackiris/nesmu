#ifndef SCREEN_H
#define SCREEN_H

#include <SDL3/SDL.h>
#include "../frame.h"

class Screen
{
public:
    Screen(SDL_Renderer* renderer, unsigned int width, unsigned int height);
    ~Screen();
    void prepare_render();
    void render_frame(const Frame& frame);
private:
    unsigned int m_width, m_height;
    SDL_Renderer* m_renderer = nullptr;
    SDL_Texture* m_texture = nullptr;
    Uint32* m_pixels;
    Uint64 m_start_count;
};

#endif // SCREEN_H
