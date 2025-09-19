#ifndef APU_H
#define APU_H

#include <SDL3/SDL.h>

#include <unordered_map>

class APU
{
public:
    explicit APU(const float &cpu_freq);

    void play_sound();
    void update_register(const uint16_t &addr, const uint8_t &value);

private:
    float m_cpu_freq; //Hz

    uint8_t m_channel_status = 0;

    uint8_t pulse1_duty = 0;
    uint8_t pulse1_length_counter = 0;
    unsigned int pulse1_t = 0;
    bool pulse1_const_volume = false; //if false, envelop will be used
    float pulse1_volume = 0; //if constant, set the volume, otherwise controls rate which enveloppe lowers
    bool pulse1_length_counter_halt = false;

    uint8_t pulse2_duty = 0;
    uint8_t pulse2_length_counter = 0;
    unsigned int pulse2_t = 0;
    bool pulse2_const_volume = false; //if false, envelop will be used
    float pulse2_volume = 0; //if constant, set the volume, otherwise controls rate which enveloppe lowers
    bool pulse2_length_counter_halt = false;

    bool tri_length_counter_halt = false;
    uint8_t tri_linear_counter_load = 0;
    unsigned int tri_t = 0;
    uint8_t tri_length_counter = 0;

    bool noise_length_counter_halt = false;
    uint8_t noise_length_counter_load = 0;
    bool noise_const_volume = false; //if false, envelop will be used
    float noise_volume = 0; //if constant, set the volume, otherwise controls rate which enveloppe lowers

    static void SDLCALL Pulse1CallBack(void *userdata, SDL_AudioStream *astream, int additional_amount, int total_amount);
    static void SDLCALL Pulse2CallBack(void *userdata, SDL_AudioStream *astream, int additional_amount, int total_amount);
    static void SDLCALL TriangleCallBack(void *userdata, SDL_AudioStream *astream, int additional_amount, int total_amount);
    static void SDLCALL NoiseCallBack(void *userdata, SDL_AudioStream *astream, int additional_amount, int total_amount);

    static const std::unordered_map<uint8_t, std::array<bool, 8>> duty_sequences;
    static const std::array<uint8_t, 32> length_counter_table;
};

#endif // APU_H
