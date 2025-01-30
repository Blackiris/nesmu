
add_executable(${PROJECT_NAME}
    components/screen.h components/screen.cpp
    components/screen.cpp components/screen.h)
target_sources(${PROJECT_NAME} PRIVATE main.cpp
    ROM/romloader.h ROM/romloader.cpp

    components/cpu.h components/cpu.cpp
    components/ram.h components/ram.cpp
    components/screen.h components/screen.cpp
    ROM/rom.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE vendor)
