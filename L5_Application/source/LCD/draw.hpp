///
/// draw.hpp
/// created by james grantham
/// 
/// this code contains simple implementation of several draw functions for use in a memory constrained
/// microcontroller driven LCD display

#ifndef DRAW_HPP
#define DRAW_HPP

#include <stdint.h>
#include "pixel.h"
#include "fonts.h"
#include "stdint.h"
#include "Screen.hpp"
#include "Point.hpp"
#include "TFTLCD.h"



// extern uint8_t screenBuffer[SCREEN_HEIGHT][SCREEN_WIDTH]; 
// extern uint8_t * current_font;


void drawLine( Screen *screen, int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void drawRec( Screen *screen, int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void drawChar( Screen *screen, char letter, uint16_t xpos, uint16_t ypos, uint16_t size = 1);
void print( Screen *screen, char * line, uint16_t xpos, uint16_t ypos,uint16_t size = 1);
void setFont( Screen *screen, uint8_t *font);
void clearScreen( Screen *screen);
// void drawBuffer( uint16_t pixel_size);



#endif
