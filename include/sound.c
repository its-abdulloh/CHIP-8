#include "sound.h"

static SDL_AudioDeviceID device;
static int beep_enabled = 0;
static double phase = 0.0;

void audio_callback(void *userdata, Uint8 *stream, int len)
{
    (void)userdata;

    float *samples = (float *)stream;
    int count = len / sizeof(float);

    for (int i = 0; i < count; i++) {
        samples[i] = beep_enabled
        ? (phase < 0.5 ? 0.2f : -0.2f)
        : 0.0f;

        phase += 300.0 / 44100.0;

        if (phase >= 1.0)
            phase -= 1.0;
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