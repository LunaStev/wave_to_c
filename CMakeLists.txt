cmake_minimum_required(VERSION 3.22.1)

if(CMAKE_VERSION VERSION_LESS "3.29")
    message(WARNING "CMake version is less than 3.29. Some features may not be available.")
endif()

project(wave_to_c C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(Wave
    src/compiler.c
    src/compiler.h
    src/main.c)
