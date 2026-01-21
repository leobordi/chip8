#include <common.h>

#define D_WIDTH 64
#define D_HEIGHT 32
#define RAM_SIZE 4096
#define FIRST_INST 0x201

typedef enum {
    IN_NONE,
    IN_00E0,
    IN_00EE,
    IN_0NNN,
    IN_1NNN,
    IN_2NNN,
    IN_3XNN,
    IN_4XNN,
    IN_5XY0,
    IN_6XNN,
    IN_7XNN,
    IN_8XY0,
    IN_8XY1,
    IN_8XY2,
    IN_8XY3,
    IN_8XY4,
    IN_8XY5,
    IN_8XY6,
    IN_8XY7,
    IN_8XYE,
    IN_9XY0,
    IN_ANNN,
    IN_BNNN,
    IN_CXNN,
    IN_DXYN,
    IN_EX9E,
    IN_EXA1,
    IN_FX07,
    IN_FX0A,
    IN_FX15,
    IN_FX18,
    IN_FX1E,
    IN_FX29,
    IN_FX33,
    IN_FX55,
    IN_FX65
} in_type;

typedef struct {
    in_type type;
    u8 X;
    u8 Y;
    u8 N;
    u8 NN;
    u16 NNN;
} instruction;

typedef struct {
    u8 memory[RAM_SIZE];
    u8 display[D_WIDTH][D_HEIGHT];
    u8 regs[16];
    u16 pc;
    u16 ir;
    instruction cur_inst;
    u8 delay_timer;
    u8 sound_timer;
} chip8;

void chip8_init();
void chip8_step();
bool chip8_load_rom(char *path);
static void fetch();
static void execute();