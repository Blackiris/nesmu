cmake_minimum_required(VERSION 3.16)

project(Nesmu LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Nesmu main.cpp
    ROM/romloader.h ROM/romloader.cpp

    components/cpu.h components/cpu.cpp
    ROM/rom.h
    components/ram.h components/ram.cpp
)

include(GNUInstallDirs)
install(TARGETS Nesmu
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
