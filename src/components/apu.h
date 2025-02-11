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
    unsigned int pulse1_length_counter = 0;
    unsigned int pulse1_t = 0;
    bool pulse1_length_counter_halt = false;

    static void SDLCALL Pulse1CallBack(void *userdata, SDL_AudioStream *astream, int additional_amount, int total_amount);

};

#endif // APU_H
