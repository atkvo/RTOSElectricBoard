#ifndef NUMBERFRAME_HPP
#define NUMBERFRAME_HPP

#include "ViewFrame.hpp"
#include "TextFrame.hpp"
#include "Point.hpp" 

#define MAX_SUBFRAMES 2
#define MAX_VAL_LENGTH 8

class NumberFrame : ViewFrame {
    int32_t prev_val=0;
    int32_t _val=0;
    char _txtval[8];
    TextFrame *txframes[MAX_VAL_LENGTH];
    public:
    NumberFrame(Point sz, Point pos, int32_t val, uint8_t size=2);
    void setMask(Point pos, Point size);
    bool outOfView(TextFrame *frame);
    void resetNumChangeAnimation(); 
    void setVal(int32_t val);
    void draw();
};

#endif