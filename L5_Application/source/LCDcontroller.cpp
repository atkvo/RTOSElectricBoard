/*
 * LCDcontroller.cpp
 *
 *  Created on: Apr 10, 2017
 *      Author: James
 */
#include "LCDcontroller.hpp"
#include "utilities.h"
#include <stdio.h>

//#define LCD_CS A3
//#define LCD_CD A2
//#define LCD_WR A1
//#define LCD_RD A0
// you can also just connect RESET to the arduino RESET pin
//#define LCD_RESET A4
//Duemilanove/Diecimila/UNO/etc ('168 and '328 chips) microcontoller:
// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

#include "TFTLCD.h"
#include "gpio.hpp"

// TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// does not matter since ports are not defined this way on SJONE board


LCDcontroller::LCDcontroller(uint8_t priority) : scheduler_task("LCD", 2048, priority, NULL),
                                                                        tft(0, 0, 0, 0, 0),
                                                                        D0(P2_0),D1(P2_1),
                                                                        D2(P2_2), D3(P2_3),
                                                                        D4(P2_4), D5(P2_5),
                                                                        D6(P2_6), D7(P2_7) {
    setRunDuration(33); // 30 frames per second refresh

}

bool LCDcontroller::init(void) {


    printf("RESETING...\n");
    tft.reset();

    printf("INITIATING DISPLAY...\n");
    tft.initDisplay();

    testtext(RED);

    return true;
}

bool LCDcontroller::run(void * p) {
    printf("\n-------------------\nrestarting...\n------------------\n");
    tft.setWriteDir();
    printf("TESTING FILL ROUND RECTANGLES...\n");
    testFillRoundRect();

    printf("TESTING ROUND RECTANGLES...\n");
    testRoundRect();

    printf("TESTING TRIANGLES...\n");
    testtriangles();

    printf("TESTING FILLED TRIANGLES...\n");
    testfilltriangles();

    printf("TESTING FILLED CIRCLES...\n");
    testfillcircles(10, BLUE);

    printf("TESTING CIRCLES...\n");
    testdrawcircles(10, WHITE);

    printf("TESTING FILL RECTANGLES...\n");
    testfillrects(BLUE, RED);

    printf("TESTING DRAW RECTANGLES...\n");
    testdrawrects(BLUE);

    printf("TESTING FAST (HORIZONTAL AND VERTICAL) LINES...\n");
    testfastlines(BLUE, RED);

    printf("TESTING LINES...\n");
    testlines(RED);

    printf("TESTING BARS...\n");
    testBars();

//    delay_ms(500);
//    printf("SET LOW\n");
//    tft.testPinsLow();
//
//
//    delay_ms(500);
//    printf("SET HIGH\n");
//    tft.testPinsHigh();
//
//    delay_ms(500);
//    printf("SET LOW\n");
//    tft.testPinsLow();
//
//    delay_ms(500);
//    printf("SET DATA 10\n");
//    tft.write8(10);
//
//    delay_ms(500);
//    printf("SET DATA 255\n");
//    tft.write8(255);
//
//
//    delay_ms(500);
//    printf("SET DATA 254\n");
//    tft.write8(254);
//
//
//    delay_ms(700);
//    printf("SET DATA 127\n");
//    tft.write8(127);
//
//
//    delay_ms(500);
//    printf("SET DATA 126\n");
//    tft.write8(126);
//
//    delay_ms(500);
//    printf("CONTROL SEQUENCE\n");
//    tft.testControlSequence();
//
//    delay_ms(500);
//    printf("DATA SEQUENCE\n");
//    tft.testDataSequence();
//
//    delay_ms(200);
//    D0.setHigh();
//
//    delay_ms(200);
//    D0.setLow();
//    D1.setHigh();
//
//    delay_ms(200);
//    D1.setLow();
//    D2.setHigh();
//
//    delay_ms(200);
//    D2.setLow();
//    D3.setHigh();
//
//    delay_ms(200);
//    D3.setLow();
//    D4.setHigh();
//
//    delay_ms(200);
//    D4.setLow();
//    D5.setHigh();
//
//    delay_ms(200);
//    D5.setLow();
//    D6.setHigh();
//
//    delay_ms(200);
//    D6.setLow();
//    D7.setHigh();
//
//    delay_ms(200);
//    D7.setLow();
    return true;
}



void LCDcontroller::testFillRoundRect() {
  tft.fillScreen(BLACK);

  for (uint16_t x=tft.width(); x > 20 ; x-=6) {
    tft.fillRoundRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, x/8,  tft.Color565(0, (uint8_t)x, 0));
 }
}

void LCDcontroller::testRoundRect() {
  tft.fillScreen(BLACK);

  for (uint16_t x=0; x < tft.width(); x+=6) {
    tft.drawRoundRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, x/8, tft.Color565((uint8_t)x, 0, 0));
 }
}

void LCDcontroller::testtriangles() {
  tft.fillScreen(BLACK);
  for (uint16_t i=0; i<tft.width()/2; i+=5) {
    tft.drawTriangle(tft.width()/2, tft.height()/2-i,
                     tft.width()/2-i, tft.height()/2+i,
                     tft.width()/2+i, tft.height()/2+i, tft.Color565(0, 0, (uint8_t)i));
  }
}

void LCDcontroller::testfilltriangles() {
  tft.fillScreen(BLACK);

  for (uint16_t i=tft.width()/2; i>10; i-=5) {
    tft.fillTriangle(tft.width()/2, tft.height()/2-i,
                     tft.width()/2-i, tft.height()/2+i,
                     tft.width()/2+i, tft.height()/2+i,
                     tft.Color565(0, i, i));
    tft.drawTriangle(tft.width()/2, tft.height()/2-i,
                     tft.width()/2-i, tft.height()/2+i,
                     tft.width()/2+i, tft.height()/2+i, tft.Color565(i, i, 0));
  }
}
void LCDcontroller::testtext(uint16_t color) {
  tft.fillScreen(BLACK);
  tft.setCursor(0, 20);
  tft.setTextColor(color);
  tft.setTextSize(3);
  //tft.println("Hello aitendo!");
  // tft.setTextSize(3);
  //tft.println(1234.56);
  //tft.setTextSize(3);
  //tft.println(0xDEADBEEF, HEX);
}

void LCDcontroller::testfillcircles(uint8_t radius, uint16_t color) {
  tft.fillScreen(BLACK);
  for (uint16_t x=radius; x < tft.width(); x+=radius*2) {
    for (uint16_t y=radius; y < tft.height(); y+=radius*2) {
      tft.fillCircle(x, y, radius, color);
    }
  }
}

void LCDcontroller::testdrawcircles(uint8_t radius, uint16_t color) {
  for (uint16_t x=0; x < tft.width()+radius; x+=radius*2) {
    for (uint16_t y=0; y < tft.height()+radius; y+=radius*2) {
      tft.drawCircle(x, y, radius, color);
    }
  }
}


void LCDcontroller::testfillrects(uint16_t color1, uint16_t color2) {
 tft.fillScreen(BLACK);
 for (uint16_t x=tft.width()-1; x > 6; x-=6) {
   //Serial.println(x, DEC);
   tft.fillRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color1);
   tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color2);
 }
}

void LCDcontroller::testdrawrects(uint16_t color) {
 tft.fillScreen(BLACK);
 for (uint16_t x=0; x < tft.width(); x+=6) {
   tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color);
 }
}

void LCDcontroller::testfastlines(uint16_t color1, uint16_t color2) {
   tft.fillScreen(BLACK);
   for (uint16_t y=0; y < tft.height(); y+=5) {
     tft.drawHorizontalLine(0, y, tft.width(), color1);
   }
   for (uint16_t x=0; x < tft.width(); x+=5) {
     tft.drawVerticalLine(x, 0, tft.height(), color2);
   }

}

void LCDcontroller::testlines(uint16_t color) {
   tft.fillScreen(BLACK);
   for (uint16_t x=0; x < tft.width(); x+=6) {
     tft.drawLine(0, 0, x, tft.height()-1, color);
   }
   for (uint16_t y=0; y < tft.height(); y+=6) {
     tft.drawLine(0, 0, tft.width()-1, y, color);
   }

   tft.fillScreen(BLACK);
   for (uint16_t x=0; x < tft.width(); x+=6) {
     tft.drawLine(tft.width()-1, 0, x, tft.height()-1, color);
   }
   for (uint16_t y=0; y < tft.height(); y+=6) {
     tft.drawLine(tft.width()-1, 0, 0, y, color);
   }

   tft.fillScreen(BLACK);
   for (uint16_t x=0; x < tft.width(); x+=6) {
     tft.drawLine(0, tft.height()-1, x, 0, color);
   }
   for (uint16_t y=0; y < tft.height(); y+=6) {
     tft.drawLine(0, tft.height()-1, tft.width()-1, y, color);
   }

   tft.fillScreen(BLACK);
   for (uint16_t x=0; x < tft.width(); x+=6) {
     tft.drawLine(tft.width()-1, tft.height()-1, x, 0, color);
   }
   for (uint16_t y=0; y < tft.height(); y+=6) {
     tft.drawLine(tft.width()-1, tft.height()-1, 0, y, color);
   }
}


void LCDcontroller::testBars() {
  uint16_t i,j;
  for(i=0; i < tft.height(); i++)
  {
    for(j=0; j < tft.width(); j++)
    {
      if(i>279) tft.writeData(WHITE);
      else if(i>239) tft.writeData(BLUE);
      else if(i>199) tft.writeData(GREEN);
      else if(i>159) tft.writeData(CYAN);
      else if(i>119) tft.writeData(RED);
      else if(i>79) tft.writeData(MAGENTA);
      else if(i>39) tft.writeData(YELLOW);
      else tft.writeData(BLACK);
    }
  }
}
