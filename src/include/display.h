#pragma once

#include <common.h>
#include <SDL2/SDL.h>

#define D_WIDTH 64
#define D_HEIGHT 32

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    u8 video_buffer[D_WIDTH * D_HEIGHT];
    bool running;
} display;

void display_init(display *dsp);
void display_update(display *dsp);
void display_draw(display *dsp);
void display_close(display *dsp);