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

