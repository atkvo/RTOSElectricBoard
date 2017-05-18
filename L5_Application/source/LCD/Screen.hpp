#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <stdint.h>

#include "Point.hpp"

#define BITBUFFER // this is to activate a space saving buffer that only used 1 bit per a 




class Screen {

    Point *maskPosition;
    Point *maskSize;
    Point *maskCorners;

    public:

#ifndef BITBUFFER
    uint8_t screenBuffer[SCREEN_HEIGHT][SCREEN_WIDTH] = {{0}};
#else
    uint32_t screenBuffer[SCREEN_HEIGHT * SCREEN_WIDTH / 32] = {0};
#endif
    void drawPixel( int32_t x,int32_t y, uint8_t color = 1);
    void drawBuffer();
    void setMask(Point *pos,Point *size);
    void setMask(Point *maskArray);
    void clear();
};

#endif
