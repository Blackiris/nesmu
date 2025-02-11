#include "apu.h"

#include <iostream>
#include <map>
#include "math.h"
#include "cpumemorymap.h"


APU::APU(const float& cpu_freq) : m_cpu_freq(cpu_freq) {}

static std::map<unsigned char, std::vector<bool>> duty_sequences = {
    {0, {0, 1, 0, 0, 0, 0, 0, 0}},
    {1, {0, 1, 1, 0, 0, 0, 0, 0}},
    {2, {0, 1, 1, 1, 1, 0, 0, 0}},
    {3, {1, 0, 0, 1, 1, 1, 1, 1}}
};

static int current_sine_sample = 0;

/* this function will be called (usually in a background thread) when the audio stream is consuming data. */
void SDLCALL APU::Pulse1CallBack(void *userdata, SDL_AudioStream *astream, int additional_amount, int total_amount) {
    additional_amount /= sizeof(float);  /* convert from bytes to samples */
    APU* apu = static_cast<APU*>(userdata);
    const float freq = apu->m_cpu_freq / (16 * (apu->pulse1_t + 1));
    std::vector<bool> duty_sequence = duty_sequences.at(apu->pulse1_duty);
    bool silence = apu->pulse1_t < 8 || !(apu->m_channel_status & 0b1) || apu->pulse1_length_counter == 0;

    while (additional_amount > 0) {
        float samples[128];
        const int total = SDL_min(additional_amount, SDL_arraysize(samples));
        int i;

        for (i = 0; i < total; i++) {
            const int phase = (int)(current_sine_sample * freq*8/ 8000.f) % 8;
            if (silence) {
                samples[i] = 0;
            } else {
                samples[i] = duty_sequence[phase] ? 1 : -1;
            }

            if (i % 64 == 0 && !apu->pulse1_length_counter_halt && apu->pulse1_length_counter > 0) {
                apu->pulse1_length_counter--;
                int q =2;
            }

            current_sine_sample++;
        }
        current_sine_sample %= 8000;

        SDL_PutAudioStreamData(astream, samples, total * sizeof(float));
        additional_amount -= total;
    }
}

static void SDLCALL TriangleCallBack(void *userdata, SDL_AudioStream *astream, int additional_amount, int total_amount) {


    additional_amount /= sizeof(float);  /* convert from bytes to samples */
    while (additional_amount > 0) {
        float samples[128];
        const int total = SDL_min(additional_amount, SDL_arraysize(samples));
        int i;

        /* generate a 440Hz pure tone */
        for (i = 0; i < total; i++) {
            const int freq = 440;
            const float phase = fmod(current_sine_sample * freq*2/ 8000.f, 2.f);
            float value;
            if (phase < 1.f) {
                value = phase;
            } else {
                value = 1.f - (phase-1.f);
            }
            samples[i] = value * 2 - 1;
            current_sine_sample++;
        }
        current_sine_sample %= 8000;

        SDL_PutAudioStreamData(astream, samples, total * sizeof(float));
        additional_amount -= total;
    }
}

static void SDLCALL NoiseCallBack(void *userdata, SDL_AudioStream *astream, int additional_amount, int total_amount) {
    additional_amount /= sizeof(float);  /* convert from bytes to samples */
    while (additional_amount > 0) {
        float samples[128];
        const int total = SDL_min(additional_amount, SDL_arraysize(samples));
        int i;

        /* generate a 440Hz pure tone */
        for (i = 0; i < total; i++) {
            float value = (static_cast<float>(rand()) * 2 / static_cast<float>(RAND_MAX)) - 1;
            samples[i] = value;
            current_sine_sample++;
        }
        current_sine_sample %= 8000;

        SDL_PutAudioStreamData(astream, samples, total * sizeof(float));
        additional_amount -= total;
    }
}

void APU::play_sound() {
    SDL_AudioSpec spec;
    spec.channels = 1;
    spec.format = SDL_AUDIO_F32;
    spec.freq = 8000;

    SDL_AudioStream* stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, Pulse1CallBack, this);
    SDL_ResumeAudioStreamDevice(stream);
}
void APU::update_register(const uint16_t& addr, unsigned char value) {
    switch(addr) {
    case APU_PULSE1_CTRL:
        pulse1_duty = (value & 0b11000000) >> 6;
        pulse1_length_counter_halt = (value & 0b00100000) >> 5;
        break;
    case APU_PULSE1_FINETUNE:
        pulse1_t = (pulse1_t & 0xff00) | value;
        break;
    case APU_PULSE1_COARSETUNE:
        pulse1_length_counter = (value & 0b11111000) >> 3;
        pulse1_t = (pulse1_t & 0xff) | ((value & 0b111) << 8);
        break;
    case APU_STATUS:
        m_channel_status = value;
        break;
    }
}

