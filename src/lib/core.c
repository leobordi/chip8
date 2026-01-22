#include <core.h>

chip8 ctx;

void chip8_init() {
    ctx.pc = FIRST_INST;
}

void chip8_step() {
    fetch();
    execute();
}

bool chip8_load_rom(char *path) {
    FILE *f = fopen(path, "rb");
    if (f == NULL) return false;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);
    
    fread(&ctx.memory[FIRST_INST], 1, size, f);
    
    fclose(f);
    return true;
}

static void fetch() {
    u8 hi = ctx.memory[ctx.pc];
    ctx.pc++;
    u8 lo = ctx.memory[ctx.pc];
    ctx.pc++;

    u8 opcode = (hi >> 4) & 0xF;
    ctx.cur_inst.X = hi & 0xF;
    ctx.cur_inst.Y = (lo >> 4) & 0xF;
    ctx.cur_inst.N = lo & 0xF;
    ctx.cur_inst.NN = lo;
    ctx.cur_inst.NNN = (ctx.cur_inst.X << 8) | ctx.cur_inst.NN;
    
    switch (opcode) {
        case 0x0:
            if (ctx.cur_inst.Y == 0xE && ctx.cur_inst.N == 0xE) {
                ctx.cur_inst.type = IN_00EE;
            } else if (ctx.cur_inst.Y == 0xE && ctx.cur_inst.N == 0x0) {
                ctx.cur_inst.type = IN_00E0;
            } else {
                ctx.cur_inst.type = IN_NONE;
            }

            break;

        case 0x1: ctx.cur_inst.type = IN_1NNN; break;
        case 0x2: ctx.cur_inst.type = IN_2NNN; break;
        case 0x3: ctx.cur_inst.type = IN_3XNN; break;
        case 0x4: ctx.cur_inst.type = IN_4XNN; break;
        case 0x5: ctx.cur_inst.type = IN_5XY0; break;
        case 0x6: ctx.cur_inst.type = IN_6XNN; break;
        case 0x7: ctx.cur_inst.type = IN_7XNN; break;

        case 0x8: 
            switch (ctx.cur_inst.N) {
                case 0x0: ctx.cur_inst.type = IN_8XY0; break;
                case 0x1: ctx.cur_inst.type = IN_8XY1; break;
                case 0x2: ctx.cur_inst.type = IN_8XY2; break;
                case 0x3: ctx.cur_inst.type = IN_8XY3; break;
                case 0x4: ctx.cur_inst.type = IN_8XY4; break;
                case 0x5: ctx.cur_inst.type = IN_8XY5; break;
                case 0x6: ctx.cur_inst.type = IN_8XY6; break;
                case 0x7: ctx.cur_inst.type = IN_8XY7; break;
                case 0xE: ctx.cur_inst.type = IN_8XYE; break;
                default: ctx.cur_inst.type = IN_NONE; break;
            }
            break;

        case 0x9: ctx.cur_inst.type = IN_9XY0; break;
        case 0xA: ctx.cur_inst.type = IN_ANNN; break;
        case 0xB: ctx.cur_inst.type = IN_BNNN; break;
        case 0xC: ctx.cur_inst.type = IN_CXNN; break;
        case 0xD: ctx.cur_inst.type = IN_DXYN; break;

        case 0xE: 
            switch (ctx.cur_inst.NN) {
                case 0x9E: ctx.cur_inst.type = IN_EX9E; break;
                case 0xA1: ctx.cur_inst.type = IN_EXA1; break;
                default: ctx.cur_inst.type = IN_NONE; break;
            }
            break;

        case 0xF: 
            switch (ctx.cur_inst.NN) {
                case 0x07: ctx.cur_inst.type = IN_FX07; break;
                case 0x0A: ctx.cur_inst.type = IN_FX0A; break;
                case 0x15: ctx.cur_inst.type = IN_FX15; break;
                case 0x18: ctx.cur_inst.type = IN_FX18; break;
                case 0x1E: ctx.cur_inst.type = IN_FX1E; break;
                case 0x29: ctx.cur_inst.type = IN_FX29; break;
                case 0x33: ctx.cur_inst.type = IN_FX33; break;
                case 0x55: ctx.cur_inst.type = IN_FX55; break;
                case 0x65: ctx.cur_inst.type = IN_FX65; break;
                default: ctx.cur_inst.type = IN_NONE; break;
            }
            break;
            
        default: ctx.cur_inst.type = IN_NONE; break;
    }
}

static void execute() {
    switch (ctx.cur_inst.type) {
        case IN_00E0:
            for (int i = 0; i < D_WIDTH; i++) {
                for (int k = 0; k < D_HEIGHT; k++) {
                    ctx.display[i][k] = 0;
                }
            }
            break;

        case IN_1NNN: 
            ctx.pc = ctx.cur_inst.NNN;
            break;

        case IN_6XNN: 
            ctx.regs[ctx.cur_inst.X] = ctx.cur_inst.NN;
            break;

        case IN_7XNN: 
            ctx.regs[ctx.cur_inst.X] += ctx.cur_inst.NN;
            break;
    
        case IN_ANNN: 
            ctx.ir = ctx.cur_inst.NNN;
            break;

        case IN_DXYN: {
                ctx.regs[0xF] = 0;
                u16 start = ctx.ir;

                for (u8 y = ctx.regs[ctx.cur_inst.Y] % 32; y < ctx.cur_inst.N; y++) {
                    u8 row = ctx.memory[start];
                    u8 x = ctx.regs[ctx.cur_inst.X] % 64;

                    for (int i = 7; i >= 0; i--) {
                        if (row >= D_WIDTH) break;

                        u8 pixel = (row >> i) & 0x1;
                        if (ctx.display[x][y] && pixel) {
                            ctx.display[x][y] = 0;
                            ctx.regs[0xF] = 1;
                        } else {
                            ctx.display[x][y] = pixel;
                        }
                        
                        x++;
                    }

                    start++;
                }
            }
            break;
    }
}