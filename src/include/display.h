#pragma once

#include <common.h>
#include <SDL2/SDL.h>

typedef struct {
    SDL_Window *window;

    bool running;
} display;

void display_init(display *dsp);
void display_update(display *dsp);
void display_close(display *dsp);