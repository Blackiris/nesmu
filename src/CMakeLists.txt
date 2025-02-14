
add_executable(${PROJECT_NAME}
    components/screen.h components/screen.cpp
    components/screen.cpp components/screen.h
    components/ppu.h components/ppu.cpp
    frame.h
    components/cpu_memory_map.h components/cpu_memory_map.cpp
    components/IMemory.h
    components/IMemory.cpp
    components/ppu_memory_map.h components/ppu_memory_map.cpp
    components/ppu_io_registers.h components/ppu_io_registers.cpp

    components/pattern_tile.h
    components/apu.h components/apu.cpp
    io_interface/icontroller.h io_interface/icontroller.cpp
    sdl_io_interface/sdl_controller.h sdl_io_interface/sdl_controller.cpp  )
target_sources(${PROJECT_NAME} PRIVATE main.cpp
    ROM/romloader.h ROM/romloader.cpp

    components/cpu.h components/cpu.cpp
    components/ram.h components/ram.cpp
    components/screen.h components/screen.cpp
    ROM/rom.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE vendor)
