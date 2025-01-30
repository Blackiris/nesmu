#include <iostream>
#include "ROM/romloader.h"
#include "components/cpu.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

const float CPU_FREQ = 1789773.0;  // 1.789 Mhz
const float FRAME_RATE  = 60.0;
const int CYCLES_PER_FRAME = CPU_FREQ / FRAME_RATE;

bool execute_frame(CPU& cpu) {
    cpu.exec_cycle(CYCLES_PER_FRAME);

    return true;
}

int main()
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    if (result < 0) {
        SDL_Log("SDL_Init error %s", SDL_GetError());
    }

    if (!SDL_CreateWindowAndRenderer("Nesmu", 320, 240, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }




    RomLoader romLoader;
    ROM rom = romLoader.read_rom_from_disk("SMB.nes");
    RAM ram(65535, {{0x0000, 0x2000, 0x0800} /*RAM*/, {0x2000, 0x4020, 0x8} /*IO registers*/});
    CPU cpu(ram);
    cpu.init();
    cpu.load_rom(rom);

    while(execute_frame(cpu)) {
    }

    SDL_DestroyRenderer(renderer);

    return 0;
}


