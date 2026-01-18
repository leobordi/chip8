#include <common.h>

typedef enum {
    IN_00E0,
    IN_00EE,
    IN_0nnn,
    IN_1nnn,
    IN_2nnn,
    IN_3xkk,
    IN_4xkk,
    IN_5xy0,
    IN_6xkk,
    IN_7xkk,
    IN_8xy0,
    IN_8xy1,
    IN_8xy2,
    IN_8xy3,
    IN_8xy4,
    IN_8xy5,
    IN_8xy6,
    IN_8xy7,
    IN_8xyE,
    IN_9xy0,
    IN_Annn,
    IN_Bnnn,
    IN_Cxkk,
    IN_Dxyn,
    IN_Ex9E,
    IN_ExA1,
    IN_Fx07,
    IN_Fx0A,
    IN_Fx15,
    IN_Fx18,
    IN_Fx1E,
    IN_Fx29,
    IN_Fx33,
    IN_Fx55,
    IN_Fx65
} in_type;

typedef struct {
    u8 V0;
    u8 V1;
    u8 V2;
    u8 V3;
    u8 V4;
    u8 V5;
    u8 V6;
    u8 V7;
    u8 V8;
    u8 V9;
    u8 VA;
    u8 VB;
    u8 VC;
    u8 VD;
    u8 VE;
    u8 VF;
} reg;

typedef struct {
    in_type type;
} instruction;

typedef struct {
    u8 memory[4096];
    reg regs;
    u16 pc;
    u16 ir;
    instruction cur_instruction;
    u8 delay_timer;
    u8 sound_timer;
} chip8;

u8 memory_read(u16 address);
void fetch();
void execute();
void run();