#include <keyboard.h>

void keyboard_init(keyboard *kb) {
    for (int i = 0; i < sizeof(kb->buffer); i++) {
        kb->buffer[i] = 0;
    }

    kb->pressed_key = 0x1F;
    kb->waiting_for_release = false;
}

u8 get_key(u16 scancode) {
    switch (scancode) {
        case 0x001E: return 0x1;
        case 0x001F: return 0x2;
        case 0x0020: return 0x3;
        case 0x0021: return 0xC;
        case 0x0014: return 0x4;
        case 0x001A: return 0x5;
        case 0x0008: return 0x6;
        case 0x0015: return 0xD;
        case 0x0004: return 0x7;
        case 0x0016: return 0x8;
        case 0x0007: return 0x9;
        case 0x0009: return 0xE;
        case 0x001D: return 0xA;
        case 0x001B: return 0x0;
        case 0x0006: return 0xB;
        case 0x0019: return 0xF;
        default: return 0x1F;
    }
}

u8 get_key_pressed(u8 *kb_buffer) {
    for (u8 i = 0x0; i <= 0xF; i++) {
        if (kb_buffer[i]) return i;
    }
    return 0x1F;
}