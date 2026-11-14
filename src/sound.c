#include "../include/sound.h"
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_stdinc.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

static SDL_AudioDeviceID device;
static int beep_enabled = 0;
static double phase = 0.0;
static uint8_t audio_buffer[16] = {0};
static int pattern_loaded = 0;
static double audio_position = 0.0;
static uint8_t audio_pitch = 64;

void audio_callback(void *userdata, Uint8 *stream, int len)
{
    (void)userdata;

    float *samples = (float *)stream;
    int count = len / sizeof(float);
    double frequency = 4000.0 * pow(2.0, (audio_pitch - 64) / 48.0);

    for (int i = 0; i < count; i++) {
        if (pattern_loaded) {
            int byte_index = (int)audio_position / 8;
            int bit_position = 7 - ((int)audio_position % 8);

            int bit = (audio_buffer[byte_index] >> bit_position) & 1;

            samples[i] = bit ? 0.2f : -0.2f;

            audio_position += frequency * 128.0 / 4000.0 / 44100.0 * 4000.0;
            if (audio_position >= 128.0)
                audio_position -= 128.0;
        }else {
            samples[i] = beep_enabled
            ? (phase < 0.5 ? 0.2f : -0.2f)
            : 0.0f;

            phase += 300.0 / 44100.0;

            if (phase >= 1.0)
                phase -= 1.0;
        }
    }
}


int sound_init(void)
{
    SDL_AudioSpec want = {0};

    want.freq = 44100;
    want.format = AUDIO_F32SYS;
    want.channels = 1;
    want.samples = 1024;
    want.callback = audio_callback;

    device = SDL_OpenAudioDevice(NULL, 0, &want, NULL, 0);

    if (device == 0) {
        SDL_Log("Audio error: %s", SDL_GetError());
        return -1;
    }

    SDL_PauseAudioDevice(device, 0);

    return 0;
}

void sound_set_enabled(int enabled)
{
    SDL_LockAudioDevice(device);
    beep_enabled = enabled;
    SDL_UnlockAudioDevice(device);
}

void sound_load_buffer(const uint8_t *buffer)
{
    SDL_LockAudioDevice(device);

    memcpy(audio_buffer, buffer, 16);
    pattern_loaded = 1;
    audio_position = 0.0;

    SDL_UnlockAudioDevice(device);
}

void sound_set_pitch(uint8_t pitch)
{
    SDL_LockAudioDevice(device);

    audio_pitch = pitch;

    SDL_UnlockAudioDevice(device);
}