#pragma once

#include <common.h>
#include <SDL2/SDL.h>

typedef struct {
    SDL_AudioDeviceID audio_device;
    bool playing;
} audio;

void audio_init(audio *audio);
void play_audio(audio *audio);
void stop_audio(audio *audio);
void audio_callback(void *userdata, uint8_t *stream, int len);
void audio_close(audio *audio);