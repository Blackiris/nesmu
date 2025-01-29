
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE main.cpp
    ROM/romloader.h ROM/romloader.cpp

    components/cpu.h components/cpu.cpp
    components/ram.h components/ram.cpp
    ROM/rom.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE vendor)
