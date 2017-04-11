/*
 * LCDcontroller.hpp
 *
 *  Created on: Apr 10, 2017
 *      Author: James
 */

#ifndef L5_APPLICATION_LCDCONTROLLER_HPP_
#define L5_APPLICATION_LCDCONTROLLER_HPP_

#include "tasks.hpp"

void testFillRoundRect();
void testRoundRect();
void testtriangles();
void testfilltriangles();
void testtext(uint16_t color);
void testfillcircles(uint8_t radius, uint16_t color);
void testdrawcircles(uint8_t radius, uint16_t color);
void testfillrects(uint16_t color1, uint16_t color2);
void testdrawrects(uint16_t color);
void testfastlines(uint16_t color1, uint16_t color2);
void testlines(uint16_t color);
void testBars();


class LCDcontroller : public scheduler_task {

public:
    LCDcontroller(uint8_t priority);
    bool init(void);
    bool run(void * p);
};



#endif /* L5_APPLICATION_LCDCONTROLLER_HPP_ */
