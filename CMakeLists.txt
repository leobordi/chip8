cmake_minimum_required(VERSION 3.23)

project(chip8 C)

include_directories(src/include)

file(GLOB LIB_SOURCES "src/lib/*.c")

add_executable(chip8 
    src/main.c
    ${LIB_SOURCES}
)