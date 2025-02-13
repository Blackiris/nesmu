#ifndef FRAME_H
#define FRAME_H

#include <stdint.h>

struct Color {
    uint8_t r=0, g=0, b=0;
};

struct Frame {
    Color colors[256][240];
    int width = 256;
    int height = 240;
};

#endif // FRAME_H
