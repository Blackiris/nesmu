#ifndef SCREEN_H
#define SCREEN_H

#include <SDL3/SDL.h>

class Screen
{
public:
    Screen();
private:
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_AppResult init_window();
};

#endif // SCREEN_H
