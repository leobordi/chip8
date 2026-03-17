# CHIP 8 EMULATOR 🚀

A minimal CHIP-8 emulator written in C, using SDL2 for display, input, and audio.

## Features

- Classic 64x32 monochrome display
- 16-key hex keypad input
- Simple audio beep via SDL2
- ROM loading at 0x200

## Build

### Dependencies

- CMake 3.23+
- SDL2 development libraries
- A C compiler (MSVC, clang, or GCC)

## Run

```
./build/chip8 path/to/rom.ch8
```

## Key Mapping

The emulator maps the CHIP-8 keypad to a typical QWERTY layout:

```
CHIP-8:  1 2 3 C      Keyboard: 1 2 3 4
         4 5 6 D                Q W E R
         7 8 9 E                A S D F
         A 0 B F                Z X C V
```

## Notes

- This is a minimal implementation intended for learning and experimentation.
- Some CHIP-8 variants and edge cases may not be fully supported yet.
