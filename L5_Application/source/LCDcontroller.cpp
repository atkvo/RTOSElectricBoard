/*
 * LCDcontroller.cpp
 *
 *  Created on: Apr 10, 2017
 *      Author: James
 */
#include "LCDcontroller.hpp"

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

// TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// does not matter since ports are not defined this way on SJONE board


LCDcontroller::LCDcontroller(uint8_t priority) : scheduler_task("LCD", 2048, priority, NULL), tft(0, 0, 0, 0, 0) {
    setRunDuration(33); // 30 frames per second refresh

}

bool LCDcontroller::init(void) {


    tft.reset();
    tft.initDisplay();

    testtext(RED);

    return true;
}

bool LCDcontroller::run(void * p) {

    testFillRoundRect();
    testRoundRect();
    testtriangles();
    testfilltriangles();
    testfillcircles(10, BLUE);
    testdrawcircles(10, WHITE);
    testfillrects(BLUE, RED);
    testdrawrects(BLUE);
    testfastlines(BLUE, RED);
    testlines(RED);
    testBars();


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
