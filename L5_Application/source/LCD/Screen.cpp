#include "Screen.hpp"
#include <stdio.h>
#include "TFTLCD.h"


void Screen::drawPixel(int32_t x,int32_t y, uint8_t color) {
    if(x < SCREEN_WIDTH && x >= 0 && y < SCREEN_HEIGHT && y >= 0) {
        if (maskCorners == NULL || (x >= maskCorners[UpperLeft].x && y >= maskCorners[UpperLeft].y && x <= maskCorners[LowerRight].x && y <= maskCorners[LowerRight].y  )) {
        #ifndef BITBUFFER
            screenBuffer[y][x] = color;
        #else
            static int32_t whichBit;
            static int32_t whichByte;
            static int32_t whichSubBit;

            whichBit = (y * SCREEN_WIDTH) + x;
            whichByte = whichBit / 32; // whichBit << 5 might  be faster depending on compiler optimizations
            whichSubBit = whichBit % 32;


            screenBuffer[whichByte] = color ? (screenBuffer[whichByte] | (1<<whichSubBit)) :(screenBuffer[whichByte] & ~(1<<whichSubBit));
        #endif
        }
    }
}

void Screen::drawBuffer()
{
    static int32_t pxrow;
    static int32_t pxcol;
    for (pxrow = 0; pxrow < SCREEN_HEIGHT; pxrow++)
    {
        for (pxcol = 0; pxcol < SCREEN_WIDTH; pxcol++)
        {
#ifndef BITBUFFER

            if (screenBuffer[pxrow][pxcol])
            {
                // printf("buffer: %d x: %3d y: %3d \n",sizeof(screenBuffer),pxrow,pxcol);
                tft->drawDev(pxcol, pxrow, true);
            }
            else
            {
                tft->drawDev(pxcol, pxrow, false);
            }
#else
            static int32_t whichBit=0;
            static int32_t whichByte=0;
            static int32_t whichSubBit=0;

            whichBit = (pxrow * SCREEN_WIDTH) + pxcol;
            whichByte = whichBit / 32; // whichBit << 5 might  be faster depending on compiler optimizations
            whichSubBit = whichBit % 32;
            if ((screenBuffer[whichByte] >> whichSubBit) & 0x00000001)
            {
                // printf("buffer: %lu height: %3d x: %3d y: %3d whichBit: %3d whichByte: %3d whichSubBit: %3d\n",sizeof(screenBuffer),SCREEN_HEIGHT, pxcol,pxrow,whichBit,whichByte, whichSubBit);
//                drawDev(pxcol, pxrow, true);
                tft->drawPixel(pxcol, pxrow, 0xFFFF);
            }
            else
            {
//                drawDev(pxcol, pxrow, false);
                tft->drawPixel(pxcol, pxrow, 0x0000);
            }

#endif
        }
    }
}

void Screen::setMask(Point *pos,Point *size) {
    maskPosition = pos;
    maskSize = size;
}

void Screen::setMask(Point *maskArray) { 
    maskCorners = maskArray;
}

void Screen::clear()
{
#ifndef BITBUFFER

    static int32_t pxrow;
    static int32_t pxcol;
    for (pxrow = 0; pxrow < SCREEN_HEIGHT; pxrow++)
    {
        for (pxcol = 0; pxcol < SCREEN_WIDTH; pxcol++)
        {
            screenBuffer[pxrow][pxcol] = 0;
        }
    }
#else
    static int32_t bufferSize = SCREEN_WIDTH * SCREEN_HEIGHT / 32;
    static int32_t pxrow;
    for (pxrow = 0; pxrow < bufferSize; pxrow++)
    {
        screenBuffer[pxrow] = 0;
    }

#endif
}
