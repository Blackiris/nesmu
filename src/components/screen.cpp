#include "screen.h"
#include <SDL3/SDL_main.h>


Screen::Screen(SDL_Renderer* renderer) : m_renderer(renderer) {

}

void Screen::render_frame(const Frame& frame) {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */
    SDL_RenderClear(m_renderer);  /* start with a blank canvas. */

    for (int i=0; i<frame.width; i++) {
        for (int j=0; j<frame.height; j++) {
            Color color = frame.colors[i][j];
            SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
            SDL_RenderPoint(m_renderer, i, j);
        }
    }
    SDL_RenderPresent(m_renderer);
}

