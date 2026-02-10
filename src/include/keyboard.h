#pragma once

#include <common.h>

typedef struct {
    u8 buffer[16];
    bool waiting_for_release;
    u8 pressed_key;
} keyboard;

void keyboard_init(keyboard *kb);
u8 get_key_pressed(u8 *kb_buffer);
u8 get_key(u16 scancode);