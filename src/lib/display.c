#include <display.h>

void display_init(display *dsp) {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        printf("Errore di inizializzazione: %s\n", SDL_GetError());
        exit(-1);
    }

    dsp->window = SDL_CreateWindow(
        "Chip 8",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_SHOWN
    );

    if (dsp->window != NULL) dsp->running = true;
    else dsp->running = false;
}

void display_update(display *dsp) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            dsp->running = false;
        }
    }
}

void display_close(display *dsp) {
    if (dsp->window != NULL) {
        SDL_DestroyWindow(dsp->window);
    }
    SDL_Quit();
}