#include <core.h>

chip8 ctx;

u8 memory_read(u16 address) {
    return ctx.memory[address];
}

void fetch() {
    // chip8 usa big endian
    u8 hi = ctx.memory[ctx.pc];
    u8 lo = ctx.memory[ctx.pc++];

    u8 opcode = (hi & 0xF0) >> 4;
    u8 X = hi & 0xF;
    u8 Y = (lo & 0x0F) >> 4;
    u8 N = lo & 0x0F;
    u8 NN = lo;
    u16 NNN = (X << 8) | NN;
    
    switch (opcode) {
        case 0x0:
            if (!X && !N) {

            } else if (!X) {

            } else {

            }

            break;
    }
}