#ifndef APU_H
#define APU_H

#include <SDL3/SDL.h>

class APU
{
public:
    APU(const float& cpu_freq);

    void play_sound();
    void update_register(const uint16_t& addr, unsigned char value);

private:
    float m_cpu_freq; //Hz

    unsigned char m_channel_status;

    unsigned char pulse1_duty = 0;
    unsigned char pulse1_length_counter = 0;
    unsigned int pulse1_t = 0;
    bool pulse1_const_volume; //if false, envelop will be used
    float pulse1_volume = 0; //if constant, set the volume, otherwise controls rate which enveloppe lowers
    bool pulse1_length_counter_halt = false;

    unsigned char pulse2_duty = 0;
    unsigned char pulse2_length_counter = 0;
    unsigned int pulse2_t = 0;
    bool pulse2_const_volume; //if false, envelop will be used
    float pulse2_volume = 0; //if constant, set the volume, otherwise controls rate which enveloppe lowers
    bool pulse2_length_counter_halt = false;

    bool tri_length_counter_halt = false;
    unsigned char tri_linear_counter_load;
    unsigned int tri_t = 0;
    unsigned char tri_length_counter;

    static void SDLCALL Pulse1CallBack(void *userdata, SDL_AudioStream *astream, int additional_amount, int total_amount);
    static void SDLCALL Pulse2CallBack(void *userdata, SDL_AudioStream *astream, int additional_amount, int total_amount);
    static void SDLCALL TriangleCallBack(void *userdata, SDL_AudioStream *astream, int additional_amount, int total_amount);
};

#endif // APU_H
