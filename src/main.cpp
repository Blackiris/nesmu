#include <iostream>
#include "ROM/romloader.h"
#include "components/cpu.h"
#include "components/cpumemorymap.h"
#include "components/ppu.h"
#include "components/ppumemorymap.h"
#include "components/screen.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <math.h>


const float CPU_FREQ = 1789773.0;  // 1.789 Mhz
const float FRAME_RATE  = 60.0;
const float CYCLES_PER_FRAME = CPU_FREQ / FRAME_RATE;
const int CYCLES_PER_FRAME_ACTIVE = CYCLES_PER_FRAME * 240 / 262;
const int CYCLES_PER_FRAME_VBLANK = CYCLES_PER_FRAME - CYCLES_PER_FRAME_ACTIVE;

bool execute_frame(SDL_Renderer* renderer, CPU& cpu, PPU& ppu, Screen& screen) {
    Uint64 start = SDL_GetPerformanceCounter();

    ppu.set_vblank(false);
    cpu.exec_cycle(CYCLES_PER_FRAME_ACTIVE);
    ppu.set_vblank(true);
    ppu.maybe_send_nmi();
    cpu.exec_cycle(CYCLES_PER_FRAME_VBLANK);

    Frame frame;
    frame.colors[10][50] = {0xff, 0x22, 0x00};
    screen.render_frame(frame);

    Uint64 end = SDL_GetPerformanceCounter();
    float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

    SDL_Delay(fmax(floor(16.666f - elapsedMS), 0));

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

    if (!SDL_CreateWindowAndRenderer("Nesmu", 256, 224, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }




    RomLoader romLoader;
    ROM rom = romLoader.read_rom_from_disk("pacman.nes");
    RAM io_registers(8);
    RAM ram(0x0800);
    CPUMemoryMap cpu_mem_map(rom, ram, io_registers);
    CPU cpu(cpu_mem_map);
    PPUMemoryMap ppu_mem_map(rom);
    PPU ppu(io_registers, cpu, ppu_mem_map);
    Screen screen(renderer);
    cpu.init();
    ppu.load_chr_rom(rom);

    bool success = true;
    while(success) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {  // poll until all events are handled!
            if (event.type == SDL_EVENT_QUIT) {
                success = false;
            }
        }

        if (success) {
            success = execute_frame(renderer, cpu, ppu, screen);
        }

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


    return 0;
}


