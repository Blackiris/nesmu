
add_executable(${PROJECT_NAME}
    components/screen.h components/screen.cpp
    components/screen.cpp components/screen.h
    components/ppu.h components/ppu.cpp
    frame.h
    components/CPUMemoryMap.h components/CPUMemoryMap.cpp
    components/IMemory.h
    components/IMemory.cpp
    components/ppumemorymap.h components/ppumemorymap.cpp
    components/PPUIORegisters.h components/PPUIORegisters.cpp
    components/controller.h components/controller.cpp
    components/pattern_tile.h
    components/apu.h components/apu.cpp  )
target_sources(${PROJECT_NAME} PRIVATE main.cpp
    ROM/romloader.h ROM/romloader.cpp

    components/cpu.h components/cpu.cpp
    components/ram.h components/ram.cpp
    components/screen.h components/screen.cpp
    ROM/rom.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE vendor)
