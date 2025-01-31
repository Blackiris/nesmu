#ifndef FRAME_H
#define FRAME_H

struct Color {
    unsigned char r, g, b;
};

struct Frame {
    Color colors[256][224];
    int width = 256;
    int height = 224;
};

#endif // FRAME_H
