cmake_minimum_required(VERSION 3.23)

project(chip8 C)

include_directories(src/include)

file(GLOB LIB_SOURCES "src/lib/*.c")

find_package(SDL2 REQUIRED)

add_executable(chip8 
    src/main.c
    ${LIB_SOURCES}
)

# Include e link SDL2
target_include_directories(chip8 PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(chip8 PRIVATE ${SDL2_LIBRARIES})
target_compile_options(chip8 PRIVATE ${SDL2_CFLAGS_OTHER})
