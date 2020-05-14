
#ifndef FALLING_H_
#define FALLING_H_
#include "Display.h"
class Falling {

public:
    Falling();
    void setup();
    void update();
    void drawDrop();
    void move(bool);
    Display disp;
    rect* bottle;
    volatile unsigned int t;
    volatile unsigned int lastPressed;
    void won(int);
    rect* score;
    int penalty;
    int state;
};

#endif
