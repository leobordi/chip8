#include <stdio.h>
#include <core.h>

int main(int argc, char **argv) {
    if (argc < 2)  {
        printf("Usage: chip8 <rom_file>\n");
        return -1;
    }

    chip8_init();

    if (!chip8_load_rom(argv[1])) {
        printf("Failed to load rom file: %s\n", argv[1]);
        return -2;
    }
    
    while (true) {
        chip8_step();
    }

    return 0;
}