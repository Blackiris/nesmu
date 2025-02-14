# Nesmu NES Emulator

## Description
NES emulator based on SDL 3.
NROM supported only, NTSC

Tested working games:
- Pacman
- Super Mario Bros
- Baloon Fight
- Excite bike

## How to build
git submodule update --init --recursive
cmake .
make


## Run
nesmu rom.nes


### Controller 1 (qwerty)
A: G
B: F
SELECT: Q
START: E
UP: W
DOWN: S
LEFT: A
RIGHT: D

### Controller 2 (qwerty)
A: KEYPAD PERIOD
B: KEYPAD 0
SELECT: RSHIFT
START: RETURN
UP: UP
DOWN: DOWN
LEFT: LEFT
RIGHT: RIGHT


    SDLController controller_2(SDL_SCANCODE_KP_PERIOD, SDL_SCANCODE_KP_0, SDL_SCANCODE_RSHIFT, SDL_SCANCODE_RETURN,
                               SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT