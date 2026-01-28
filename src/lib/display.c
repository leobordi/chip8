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

    dsp->renderer = SDL_CreateRenderer(dsp->window, -1, SDL_RENDERER_ACCELERATED);
    
    dsp->texture = SDL_CreateTexture(
        dsp->renderer, 
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        D_WIDTH,
        D_HEIGHT
    );

    display_draw(dsp);

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

void display_draw(display *dsp) {
    uint32_t pixels[D_WIDTH * D_HEIGHT];

    for (int i = 0; i < D_WIDTH * D_HEIGHT; i++) {
        pixels[i] = (dsp->video_buffer[i] == 1) ? 0xFFFFFFFF : 0x000000FF;
    }

    SDL_UpdateTexture(dsp->texture, NULL, pixels, 64 * sizeof(uint32_t));
    SDL_RenderClear(dsp->renderer);
    SDL_RenderCopy(dsp->renderer, dsp->texture, NULL, NULL);
    SDL_RenderPresent(dsp->renderer);
}

void display_close(display *dsp) {
    if (dsp->window != NULL) {
        SDL_DestroyWindow(dsp->window);
    }
    SDL_Quit();
}