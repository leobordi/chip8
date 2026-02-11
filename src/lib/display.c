#include <display.h>

void display_init(display *dsp) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        printf("Errore di inizializzazione: %s\n", SDL_GetError());
        exit(-1);
    }

    dsp->window = SDL_CreateWindow(
        "Chip 8",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1200,
        720,
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

void display_update(display *dsp, u8 *keyboard) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                dsp->running = false;
                break;
            
            case SDL_KEYDOWN: {
                    u8 current_key = get_key(event.key.keysym.scancode);
                    if (current_key <= 0xF) {
                        keyboard[current_key] = 1;
                    }
                }
                break;
            
            case SDL_KEYUP: {
                    u8 current_key = get_key(event.key.keysym.scancode);
                    if (current_key <= 0xF) {
                        keyboard[current_key] = 0;
                    }
                }
                break;
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
        SDL_DestroyRenderer(dsp->renderer);
        SDL_DestroyWindow(dsp->window);
    }
    SDL_Quit();
}