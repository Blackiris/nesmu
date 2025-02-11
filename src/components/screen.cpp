#include "screen.h"
#include <SDL3/SDL_main.h>

#include <iostream>
#include <math.h>

Screen::Screen(SDL_Renderer* renderer, unsigned int width, unsigned int height)
    : m_width(width), m_height(height), m_renderer(renderer) {

    m_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);
    m_pixels = new Uint32[width * height];
}

void Screen::prepare_render() {
    m_start_count = SDL_GetPerformanceCounter();
}

void Screen::render_frame(const Frame& frame) {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */
    SDL_RenderClear(m_renderer);  /* start with a blank canvas. */

    for (int i=0; i<frame.width; i++) {
        for (int j=0; j<frame.height; j++) {
            Color color = frame.colors[i][j];
            m_pixels[i+j*m_width] = 0xff000000 | (color.r << 16) | (color.g << 8) | color.b;
        }
    }
    SDL_UpdateTexture(m_texture, NULL, m_pixels, m_width * sizeof(Uint32));
    SDL_RenderTexture(m_renderer, m_texture, NULL, NULL);
    SDL_RenderPresent(m_renderer);

    Uint64 end = SDL_GetPerformanceCounter();
    float elapsedMS = (end - m_start_count) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
    SDL_Delay(fmax(floor(16.666f - elapsedMS), 0));
}

Screen::~Screen() {
    delete[] m_pixels;
    SDL_DestroyTexture(m_texture);
}

