#ifndef DISPLAY_H_
#define DISPLAY_H_
#include "arduino.h"
struct rect {
    float x;
    float y;
    int w;
    int h;
    float vx;
    float vy;
};

class Display {

public:
    Display();
    rect* drawRect(float, float, int, int);
    void draw();
    int width = 16;
    int height = 16;
    rect* sprites[10] = {nullptr};
    byte Pixel[16][16];
};

#endif
