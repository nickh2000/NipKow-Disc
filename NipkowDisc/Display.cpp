
#include "Display.h"



Display::Display(){

}


rect* Display::drawRect(float x, float y, int w, int h) {
    for(int i = 0; i < 25; i ++)
        if (sprites[i] == nullptr) {
            sprites[i] = new rect;
            this->sprites[i]->x = x;
            this->sprites[i]->y = y;
            this->sprites[i]->w = w;
            this->sprites[i]->h = h;
            this->sprites[i]->vx = 0;
            this->sprites[i]->vy = 0;
            return this->sprites[i];
        }

        
     return nullptr;
}


void Display::draw() {
    for (int x = 0; x < this->width; x ++)
        for(int y = 0; y < this->height; y ++)
            this->Pixel[y][x] = 0;

    for(int i = 0; i < 10; i ++)
        if (this->sprites[i] != nullptr) {
            for(int x = (int)sprites[i]->x; x < (int)sprites[i]->x + sprites[i]->w; x++)
                for(int y  = (int)sprites[i]->y; y < (int)sprites[i]->y + sprites[i]->h; y ++)
                    this->Pixel[y][x] = 0x01;
            this->sprites[i]->x += this->sprites[i]->vx;
            this->sprites[i]->y += this->sprites[i]->vy;
        }
}
