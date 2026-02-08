#include <audio.h>

void audio_init(audio *audio) {
    SDL_AudioSpec spec;
    SDL_memset(&spec, 0, sizeof(spec));
    spec.freq = 44100;
    spec.format = AUDIO_S16SYS;
    spec.channels = 1;
    spec.samples = 512;
    spec.callback = audio_callback;
    spec.userdata = NULL;
    
    audio->audio_device = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
    if (audio->audio_device == 0) {
        printf("Errore apertura audio: %s\n", SDL_GetError());
        exit(-1);
    }
}

// Callback audio per generare un beep a 440 Hz
void audio_callback(void *userdata, uint8_t *stream, int len) {
    static int sample_index = 0;
    int16_t *buffer = (int16_t *)stream;
    int samples = len / 2;
    const int frequency = 440;
    const int sample_rate = 44100;
    const int16_t amplitude = 3000;
    
    // Periodo in samples: quanti campioni per un ciclo completo
    int period = sample_rate / frequency;
    
    for (int i = 0; i < samples; i++) {
        // Onda quadra: metà periodo positivo, metà negativo
        if ((sample_index % period) < (period / 2)) {
            buffer[i] = amplitude;
        } else {
            buffer[i] = -amplitude;
        }
        sample_index++;
    }
}

void play_audio(audio *audio) {
    SDL_PauseAudioDevice(audio->audio_device, 0);  // Avvia audio
    audio->playing = 1;
}

void stop_audio(audio *audio) {
    SDL_PauseAudioDevice(audio->audio_device, 1);  // Ferma audio
    audio->playing = 0;
}

void audio_close(audio *audio) {
    SDL_CloseAudioDevice(audio->audio_device);
}