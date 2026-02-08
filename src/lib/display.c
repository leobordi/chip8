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
                    //printf( "Key pressed: %s - 0x%02X\n", SDL_GetKeyName(event.key.keysym.sym), event.key.keysym.scancode);
                    u8 key_pressed = get_key(event.key.keysym.scancode);
                    if (key_pressed <= 0xF) keyboard[key_pressed] = 1;
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
        SDL_DestroyWindow(dsp->window);
    }
    SDL_Quit();
}

static u8 get_key(SDL_Scancode scancode) {
    switch (scancode) {
        case 0x1E: return 0x1;
        case 0x1F: return 0x2;
        case 0x20: return 0x3;
        case 0x21: return 0xC;
        case 0x14: return 0x4;
        case 0x1A: return 0x5;
        case 0x08: return 0x6;
        case 0x15: return 0xD;
        case 0x04: return 0x7;
        case 0x16: return 0x8;
        case 0x07: return 0x9;
        case 0x09: return 0xE;
        case 0x1D: return 0xA;
        case 0x1B: return 0x0;
        case 0x06: return 0xB;
        case 0x19: return 0xF;
        default: return 0x1F;
    }
}