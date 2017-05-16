#include "NumberFrame.hpp"


NumberFrame::NumberFrame(Point sz, Point pos, int32_t val, uint8_t size) : ViewFrame(sz,pos) {
    static int32_t i;
    for (i = 0 ; i < MAX_SUBFRAMES; i++) {
            txframes[i] =  new TextFrame(*_screen, sz,pos,"");
    }
    Animation::getInstance();
}

void NumberFrame::setVal(int32_t val) {
    if(val != _val) {
        prev_val = _val;
        _val = val;
        resetNumChangeAnimation();
    }
}

void NumberFrame::draw() {
    setDraw();
    static int32_t i;
    for (i = 0 ; i < MAX_SUBFRAMES; i++) {
        if (txframes[i]->active) {
            txframes[i]->draw();
        }
    }
}

void NumberFrame::erase() {
    setErase();
    static int32_t i;
    for (i = 0 ; i < MAX_SUBFRAMES; i++) {
        if (txframes[i]->active) {
            txframes[i]->draw();
        }
    }
}

void NumberFrame::setMask(Point pos, Point size) {
    maskCorners[UpperLeft] = pos;

    Point LL(pos.x + size.x, pos.y);
    maskCorners[LowerLeft] = LL;

    Point UR(pos.x, pos.y + size.y);
    maskCorners[UpperRight] = UR;

    Point LR(pos.x + size.x, pos.y + size.y);
    maskCorners[LowerRight] = LR;

    _maskPosition = pos;
    _maskSize = size;

    static int32_t i;
    for (i = 0 ; i < MAX_SUBFRAMES; i++) {
        txframes[i]->setMask(pos, size);
    }
}

// Cheack whether the individual text frames are fully out of the 
// bounds of the NumberFrame based on its size and corners
bool NumberFrame::outOfView(TextFrame *frame) {
    static Point *fcorners;
    fcorners = frame->getCorners();
    if (fcorners == NULL || (fcorners[LowerRight].x < corners[UpperLeft].x || fcorners[LowerRight].y < corners[UpperLeft].y || fcorners[UpperLeft].x <= corners[LowerRight].x || fcorners[UpperLeft].y <= corners[LowerRight].y  )) {
        return true;
    }
    return false;
}

void NumberFrame::resetNumChangeAnimation() {
    
}