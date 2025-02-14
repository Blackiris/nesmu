#include <iostream>
#include "ROM/romloader.h"
#include "components/apu.h"
#include "components/cpu.h"
#include "components/cpu_memory_map.h"
#include "components/ppu.h"
#include "components/ppu_memory_map.h"
#include "components/ppu_io_registers.h"
#include "components/screen.h"
#include "sdl_io_interface/sdl_controller.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>



const float CPU_FREQ = 1789773.0;  // 1.789 Mhz
const float FRAME_RATE  = 60.0;
const float CYCLES_PER_FRAME = CPU_FREQ / FRAME_RATE;
const int CYCLES_PER_FRAME_ACTIVE = CYCLES_PER_FRAME * 240 / 262;
const int CYCLES_PER_FRAME_ACTIVE_PER_SCANLINE = CYCLES_PER_FRAME / 262;
const int CYCLES_PER_FRAME_VBLANK = CYCLES_PER_FRAME - CYCLES_PER_FRAME_ACTIVE;

void execute_frame(CPU& cpu, PPU& ppu, Screen& screen) {
    screen.prepare_render();


    ppu.set_vblank(false);
    int total_cpu_cycles = 0;
    int line_number = 0;
    Frame frame;
    ppu.draw_backdrop_color(frame);
    while (total_cpu_cycles < CYCLES_PER_FRAME_ACTIVE) {
        total_cpu_cycles += cpu.exec_cycle(CYCLES_PER_FRAME_ACTIVE_PER_SCANLINE);
        if (line_number < frame.height) {
            ppu.render_frame_scanline(frame, line_number);
            line_number++;
        }
    }

    screen.render_frame(frame);
    ppu.set_vblank(true);
    if (ppu.maybe_send_nmi()) {
        cpu.set_nmi();
    }
    cpu.exec_cycle(CYCLES_PER_FRAME_VBLANK);
    ppu.set_oam_addr(0);


}

int start_engine(const std::string& rom_name) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
    if (result < 0) {
        SDL_Log("SDL_Init error %s", SDL_GetError());
    }

    if (!SDL_CreateWindowAndRenderer("Nesmu", 256*2, 240*2, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }


    RomLoader romLoader;
    ROM rom = romLoader.read_rom_from_disk(rom_name);


    SDLController controller_1(SDL_SCANCODE_G, SDL_SCANCODE_F, SDL_SCANCODE_Q, SDL_SCANCODE_E,
                               SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D);
    SDLController controller_2(SDL_SCANCODE_KP_PERIOD, SDL_SCANCODE_KP_0, SDL_SCANCODE_RSHIFT, SDL_SCANCODE_RETURN,
                               SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT);
    RAM oam(0xf00);
    RAM vram(0x4000);
    PPUMemoryMap ppu_mem_map(rom, vram);
    PPUIORegisters io_registers(oam, ppu_mem_map);
    RAM ram(0x0800);
    APU apu(CPU_FREQ);
    CPUMemoryMap cpu_mem_map(rom, ram, io_registers, &controller_1, &controller_2, apu);
    io_registers.set_cpu_memory_map(&cpu_mem_map);
    CPU cpu(cpu_mem_map);

    apu.play_sound();

    PPU ppu(io_registers, ppu_mem_map, oam);
    Screen screen(renderer, 256, 240);
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
            execute_frame(cpu, ppu, screen);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: command rom.nes" << std::endl;
        return 0;
    }
    return start_engine(argv[1]);
}


