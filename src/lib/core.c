#include <core.h>
#include <display.h>

chip8 ctx;

void chip8_init() {
    load_font();
    display_init(&ctx.disp);
    ctx.pc = FIRST_INST;
    ctx.sp = 0x0;
}

void chip8_run() {
    while (true) {
        display_update(&ctx.disp);
        if (!ctx.disp.running) {
            display_close(&ctx.disp);
            break;
        }

        fetch();
        execute();
    }
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
            for (int i = 0; i < D_WIDTH * D_HEIGHT; i++) {
                ctx.disp.video_buffer[i] = 0;
            }
            break;

        case IN_00EE:
            ctx.sp--;
            ctx.pc = ctx.stack[ctx.sp];
            break;

        case IN_1NNN: 
            ctx.pc = ctx.cur_inst.NNN;
            break;

        case IN_2NNN:
            ctx.stack[ctx.sp] = ctx.pc;
            ctx.sp++;
            ctx.pc = ctx.cur_inst.NNN;
            break;

        case IN_3XNN:
            if (ctx.regs[ctx.cur_inst.X] == ctx.cur_inst.NN) ctx.pc += 2;
            break;

        case IN_4XNN:
            if (ctx.regs[ctx.cur_inst.X] != ctx.cur_inst.NN) ctx.pc += 2;
            break;

        case IN_5XY0:
            if (ctx.regs[ctx.cur_inst.X] == ctx.regs[ctx.cur_inst.Y]) ctx.pc += 2;
            break;

        case IN_6XNN: 
            ctx.regs[ctx.cur_inst.X] = ctx.cur_inst.NN;
            break;

        case IN_7XNN: 
            ctx.regs[ctx.cur_inst.X] += ctx.cur_inst.NN;
            break;

        case IN_8XY0:
            ctx.regs[ctx.cur_inst.X] = ctx.regs[ctx.cur_inst.Y];
            break;

        case IN_8XY1:
            ctx.regs[ctx.cur_inst.X] |= ctx.regs[ctx.cur_inst.Y];
            break;

        case IN_8XY2:
            ctx.regs[ctx.cur_inst.X] &= ctx.regs[ctx.cur_inst.Y];
            break;

        case IN_8XY3:
            ctx.regs[ctx.cur_inst.X] ^= ctx.regs[ctx.cur_inst.Y];
            break;

        case IN_8XY4: {
                u16 result = ctx.regs[ctx.cur_inst.X] + ctx.regs[ctx.cur_inst.Y];
                if (result > 0xFF) {
                    ctx.regs[0xF] = 1;
                    ctx.regs[ctx.cur_inst.X] = 0xFF;
                } else {
                    ctx.regs[0xF] = 0;
                    ctx.regs[ctx.cur_inst.X] =  result & 0xFF;
                }
            }
            break;

        case IN_8XY5: {
                u8 x = ctx.cur_inst.X;
                u8 y = ctx.cur_inst.Y;
                u8 flag = ctx.regs[x] >= ctx.regs[y] ? 1 : 0;

                ctx.regs[x] -= ctx.regs[y];
                ctx.regs[0xF] = flag;
            }
            break;

        case IN_8XY6:
            ctx.regs[ctx.cur_inst.X] = ctx.regs[ctx.cur_inst.Y] >> 1;
            ctx.regs[0xF] = ctx.regs[ctx.cur_inst.Y] & 0x1;
            break;

        case IN_8XY7: {
                u8 x = ctx.cur_inst.X;
                u8 y = ctx.cur_inst.Y;
                u8 flag = ctx.regs[y] >= ctx.regs[x] ? 1 : 0;

                ctx.regs[x] = ctx.regs[y] - ctx.regs[x];
                ctx.regs[0xF] = flag;
            }
            break;

        case IN_8XYE:
            ctx.regs[ctx.cur_inst.X] = ctx.regs[ctx.cur_inst.Y] << 1;
            ctx.regs[0xF] = ctx.regs[ctx.cur_inst.Y] & 0x80;
            break;

        case IN_9XY0:
            if (ctx.regs[ctx.cur_inst.X] != ctx.regs[ctx.cur_inst.Y]) ctx.pc += 2;
            break;
    
        case IN_ANNN: 
            ctx.ir = ctx.cur_inst.NNN;
            break;

        case IN_DXYN: {
                ctx.regs[0xF] = 0;
                u8 start_x = ctx.regs[ctx.cur_inst.X] % D_WIDTH;
                u8 start_y = ctx.regs[ctx.cur_inst.Y] % D_HEIGHT;

                for (u8 row_offset = 0; row_offset < ctx.cur_inst.N; row_offset++) {
                    int y = start_y + row_offset;
                    if (y >= D_HEIGHT) break; 

                    u8 sprite_byte = ctx.memory[ctx.ir + row_offset];

                    for (u8 col_offset = 0; col_offset < 8; col_offset++) {
                        int x = start_x + col_offset;
                        if (x >= D_WIDTH) break; 

                        u8 pixel = (sprite_byte >> (7 - col_offset)) & 0x1;
                        if (!pixel) continue;

                        int index = x + (y * D_WIDTH);

                        if (ctx.disp.video_buffer[index]) ctx.regs[0xF] = 1;
                        ctx.disp.video_buffer[index] ^= 1;
                    }
                }

                display_draw(&ctx.disp);
            }
            break;
        
        case IN_EX9E:
            printf("Istruzione IN_EX9E non implementata\n");
            break;
        
        case IN_EXA1:
            printf("Istruzione IN_EXA1 non implementata\n");
            break;
        
        case IN_FX07:
            ctx.regs[ctx.cur_inst.X] = ctx.delay_timer;
            break;
        
        case IN_FX0A:
            printf("Istruzione IN_FX0A non implementata\n");
            break;
        
        case IN_FX15:
            ctx.delay_timer = ctx.regs[ctx.cur_inst.X];
            break;
        
        case IN_FX18:
            ctx.sound_timer = ctx.regs[ctx.cur_inst.X];
            break;
        
        case IN_FX1E:
            ctx.ir += ctx.regs[ctx.cur_inst.X];
            break;
        
        case IN_FX29:
            printf("Istruzione IN_FX29 non implementata\n");
            break;
        
        case IN_FX33:
            printf("Istruzione IN_FX33 non implementata\n");
            break;
        
        case IN_FX55:
            printf("Istruzione IN_FX55 non implementata\n");
            break;
        
        case IN_FX65:
            printf("Istruzione IN_FX65 non implementata\n");
            break;

        default:
            printf("Istruzione  non implementata\n");
            break;
    }
}

static void load_font() {
    u8 font[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,
        0x20, 0x60, 0x20, 0x20, 0x70,
        0xF0, 0x10, 0xF0, 0x80, 0xF0,
        0xF0, 0x10, 0xF0, 0x10, 0xF0,
        0x90, 0x90, 0xF0, 0x10, 0x10,
        0xF0, 0x80, 0xF0, 0x10, 0xF0,
        0xF0, 0x80, 0xF0, 0x90, 0xF0,
        0xF0, 0x10, 0x20, 0x40, 0x40,
        0xF0, 0x90, 0xF0, 0x90, 0xF0,
        0xF0, 0x90, 0xF0, 0x10, 0xF0,
        0xF0, 0x90, 0xF0, 0x90, 0x90,
        0xE0, 0x90, 0xE0, 0x90, 0xE0,
        0xF0, 0x80, 0x80, 0x80, 0xF0,
        0xE0, 0x90, 0x90, 0x90, 0xE0,
        0xF0, 0x80, 0xF0, 0x80, 0xF0,
        0xF0, 0x80, 0xF0, 0x80, 0x80
    };

    for (int i = 0x0; i < sizeof(font); i++) {
        ctx.memory[i + 0x50] = font[i];
    }
}