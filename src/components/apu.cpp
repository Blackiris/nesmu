#include "apu.h"

#include <iostream>
#include <array>
#include <unordered_map>
#include "math.h"
#include "cpu_memory_map.h"

#define SAMPLES 16000


APU::APU(const float &cpu_freq) : m_cpu_freq(cpu_freq) {}

const std::unordered_map<uint8_t, std::array<bool, 8>> APU::duty_sequences = {
    {0, {0, 1, 0, 0, 0, 0, 0, 0}},
    {1, {0, 1, 1, 0, 0, 0, 0, 0}},
    {2, {0, 1, 1, 1, 1, 0, 0, 0}},
    {3, {1, 0, 0, 1, 1, 1, 1, 1}}
};

const std::array<uint8_t, 32> APU::length_counter_table = {
    10,254, 20,  2, 40,  4, 80,  6, 160,  8, 60, 10, 14, 12, 26, 14,
    12, 16, 24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30
};

static int current_sample_pulse1 = 0;
static int current_sample_pulse2 = 0;
static int current_sample_triangle = 0;
static int current_sample_noise = 0;

/* this function will be called (usually in a background thread) when the audio stream is consuming data. */
void SDLCALL APU::Pulse1CallBack(void *userdata, SDL_AudioStream *astream, int additional_amount, int) {
    additional_amount /= sizeof(float);  /* convert from bytes to samples */
    APU* apu = static_cast<APU*>(userdata);
    const float freq = apu->m_cpu_freq / (16 * (apu->pulse1_t + 1));
    std::array<bool, 8> duty_sequence = duty_sequences.at(apu->pulse1_duty);
    bool silence = apu->pulse1_t < 8 || !(apu->m_channel_status & 0b1) || apu->pulse1_length_counter == 0;
    float amplitude = 1.f;

    while (additional_amount > 0) {
        float samples[1280];
        const unsigned total = SDL_min((unsigned)additional_amount, SDL_arraysize(samples));

        for (unsigned i = 0; i < total; i++) {
            float sample = 0;
            const int phase = (int)(current_sample_pulse1 * freq*8.f/ SAMPLES) % 8;
            if (!silence) {
                sample = (duty_sequence[phase] ? 1 : -1);
            }

            if (sample != 0 && apu->pulse1_const_volume) {
                amplitude = apu->pulse1_volume;
                sample *= amplitude;
            }

            samples[i] = sample;

            if (i % 4 == 0 && !apu->pulse1_length_counter_halt && apu->pulse1_length_counter > 0) {
                apu->pulse1_length_counter--;
            }

            current_sample_pulse1++;
        }
        current_sample_pulse1 %= SAMPLES;

        SDL_PutAudioStreamData(astream, samples, total * sizeof(float));
        additional_amount -= total;
    }
}

/* this function will be called (usually in a background thread) when the audio stream is consuming data. */
void SDLCALL APU::Pulse2CallBack(void *userdata, SDL_AudioStream *astream, int additional_amount, int) {
    additional_amount /= sizeof(float);  /* convert from bytes to samples */
    APU* apu = static_cast<APU*>(userdata);
    const float freq = apu->m_cpu_freq / (16 * (apu->pulse2_t + 1));
    std::array<bool, 8> duty_sequence = duty_sequences.at(apu->pulse2_duty);
    bool silence = apu->pulse2_t < 8 || !(apu->m_channel_status & 0b10) || apu->pulse2_length_counter == 0;
    float amplitude = 1.f;

    while (additional_amount > 0) {
        float samples[1280];
        const unsigned total = SDL_min((unsigned)additional_amount, SDL_arraysize(samples));

        for (unsigned i = 0; i < total; i++) {
            float sample = 0;
            const int phase = (int)(current_sample_pulse2 * freq*8.f/ SAMPLES) % 8;
            if (!silence) {
                sample = (duty_sequence[phase] ? 1 : -1);
            }

            if (sample != 0 && apu->pulse2_const_volume) {
                amplitude = apu->pulse2_volume;
                sample *= amplitude;
            }

            samples[i] = sample;

            if (i % 4 == 0 && !apu->pulse2_length_counter_halt && apu->pulse2_length_counter > 0) {
                apu->pulse2_length_counter--;
            }

            current_sample_pulse2++;
        }
        current_sample_pulse2 %= SAMPLES;

        SDL_PutAudioStreamData(astream, samples, total * sizeof(float));
        additional_amount -= total;
    }
}

void SDLCALL APU::TriangleCallBack(void *userdata, SDL_AudioStream *astream, int additional_amount, int) {


    additional_amount /= sizeof(float);  /* convert from bytes to samples */
    APU* apu = static_cast<APU*>(userdata);

    const float freq = apu->m_cpu_freq / (32 * (apu->tri_t + 1));
    bool silence =!(apu->m_channel_status & 0b100) || apu->tri_length_counter == 0;

    while (additional_amount > 0) {
        float samples[1280];
        const unsigned total = SDL_min((unsigned)additional_amount, SDL_arraysize(samples));

        for (unsigned i = 0; i < total; i++) {
            float sample = 0;

            if (!silence) {
                const float phase = fmod(current_sample_triangle * freq*2/ SAMPLES, 2.f);

                if (phase < 1.f) {
                    sample = phase;
                } else {
                    sample = 2.f - phase;
                }

                sample = sample * 2 - 1;
            }

            samples[i] = sample;
            current_sample_triangle++;

            if (i % 4 == 0 && !apu->tri_length_counter_halt && apu->tri_length_counter > 0) {
                apu->tri_length_counter--;
            }
        }
        current_sample_triangle %= SAMPLES;

        SDL_PutAudioStreamData(astream, samples, total * sizeof(float));
        additional_amount -= total;
    }
}

void SDLCALL APU::NoiseCallBack(void *userdata, SDL_AudioStream *astream, int additional_amount, int) {
    APU* apu = static_cast<APU*>(userdata);
    bool silence = !(apu->m_channel_status & 0b1000) || apu->noise_length_counter_load == 0;

    additional_amount /= sizeof(float);  /* convert from bytes to samples */
    while (additional_amount > 0) {
        float samples[128];
        const unsigned total = SDL_min((unsigned)additional_amount, SDL_arraysize(samples));

        for (unsigned i = 0; i < total; i++) {
            float value = 0;
            if (!silence) {
                value = (static_cast<float>(rand()) * 2 / static_cast<float>(RAND_MAX)) - 1;
            }
            samples[i] = value;
            current_sample_noise++;

            if (i % 4 == 0 && !apu->noise_length_counter_halt && apu->noise_length_counter_load > 0) {
                apu->noise_length_counter_load--;
            }
        }
        current_sample_noise %= SAMPLES;

        SDL_PutAudioStreamData(astream, samples, total * sizeof(float));
        additional_amount -= total;
    }
}

void APU::play_sound() {
    SDL_AudioSpec spec;
    spec.channels = 1;
    spec.format = SDL_AUDIO_F32;
    spec.freq = SAMPLES;

    SDL_AudioStream* stream_pulse1 = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, Pulse1CallBack, this);
    SDL_ResumeAudioStreamDevice(stream_pulse1);

    SDL_AudioStream* stream_pulse2 = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, Pulse2CallBack, this);
    SDL_ResumeAudioStreamDevice(stream_pulse2);

    SDL_AudioStream* stream_tri = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, TriangleCallBack, this);
    SDL_ResumeAudioStreamDevice(stream_tri);

    SDL_AudioStream* stream_noise = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NoiseCallBack, this);
    SDL_ResumeAudioStreamDevice(stream_noise);
}
void APU::update_register(const uint16_t &addr, const uint8_t &value) {
    switch(addr) {
    case APU_PULSE1_CTRL:
        pulse1_duty = (value & 0b11000000) >> 6;
        pulse1_length_counter_halt = value & 0b00100000;
        pulse1_const_volume = (value & 0b00010000) >> 4;
        pulse1_volume = (value & 0b00001111)/15.f;
        break;
    case APU_PULSE1_FINETUNE:
        pulse1_t = (pulse1_t & 0xff00) | value;
        break;
    case APU_PULSE1_COARSETUNE:
        pulse1_t = (pulse1_t & 0xff) | ((value & 0b111) << 8);
        pulse1_length_counter = length_counter_table[(value & 0b11111000) >> 3];
        /*std::cout << std::format("D:{} Lh:{} L:{}",
                                 pulse1_duty, pulse1_length_counter_halt, pulse1_length_counter)
                  << std::endl;*/
        break;

    case APU_PULSE2_CTRL:
        pulse2_duty = (value & 0b11000000) >> 6;
        pulse2_length_counter_halt = value & 0b00100000;
        pulse2_const_volume = (value & 0b00010000) >> 4;
        pulse2_volume = (value & 0b00001111)/15.f;
        break;
    case APU_PULSE2_FINETUNE:
        pulse2_t = (pulse2_t & 0xff00) | value;
        break;
    case APU_PULSE2_COARSETUNE:
        pulse2_t = (pulse2_t & 0xff) | ((value & 0b111) << 8);
        pulse2_length_counter = length_counter_table[(value & 0b11111000) >> 3];
        /*std::cout << std::format("D:{} Lh:{} L:{}",
                                 pulse1_duty, pulse2_length_counter_halt, pulse2_length_counter)
                  << std::endl;*/
        break;

    case APU_TRI_CTRL1:
        tri_length_counter_halt = value & 0b10000000;
        tri_linear_counter_load = value & 0b01111111;

        break;
    case APU_TRI_FREQ1:
        tri_t = (tri_t % 0xff00) | value;
        break;
    case APU_TRI_FREQ2:
        tri_t = (tri_t & 0xff) | ((value & 0b111) << 8);
        tri_length_counter = length_counter_table[(value & 0b11111000) >> 3];
        break;

    case APU_NOISE_CTRL:
        noise_length_counter_halt = value & 0b00100000;
        noise_const_volume = (value & 0b00010000) >> 4;
        noise_volume = (value & 0b00001111)/15.f;
        break;
    case APU_NOISE_FREQ1:
        break;
    case APU_NOISE_FREQ2:
        noise_length_counter_load = length_counter_table[(value & 0b11111000) >> 3];
        break;


    case APU_STATUS:
        m_channel_status = value;
        break;
    }
}

