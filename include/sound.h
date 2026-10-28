#pragma once

#include <SDL2/SDL.h>
#include <math.h>

#define AMPLITUDE 28000
#define FREQUENCY 300 // A4 note (440Hz) or 300Hz for classic CHIP-8 buzz
#define SAMPLE_RATE 44100


void audio_callback(void *userdata, Uint8 *stream, int len);
int init_audio();
void sound_set_enabled(int enabled);

