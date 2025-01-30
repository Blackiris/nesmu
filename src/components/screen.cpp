#include "screen.h"
#include <SDL3/SDL_main.h>

Screen::Screen() {
    init_window();
}


SDL_AppResult Screen::init_window() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Nesmu", 320, 240, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
}
