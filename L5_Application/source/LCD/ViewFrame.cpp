
#include "ViewFrame.hpp"
    // Screen *_screen;

ViewFrame::ViewFrame() {
    Point p;
    _position = p;
}

ViewFrame::ViewFrame(Screen &screen) {
    Point p;
    Point s;

    _position = p;
    _size = s;
    setCorners();
    _screen = &screen;

}

ViewFrame::ViewFrame(Point size, Point position) : _position(), _size(), corners(),_maskSize(),_maskPosition() {
    _size = size;
    _position = position;
    setCorners();
    setMask(_maskPosition,Point(SCREEN_WIDTH,SCREEN_HEIGHT));
}

ViewFrame::ViewFrame(Screen &screen, Point size, Point position) {
    _size = size;
    _position = position;
    setCorners();
    _screen = &screen;
}


void ViewFrame::setCorners() {
    corners[UpperLeft] = _position;
    
    Point LL(_position.x + _size.x, _position.y);
    corners[LowerLeft] = LL;

    Point UR(_position.x, _position.y + _size.y);
    corners[UpperRight] = UR;

    Point LR(_position.x + _size.x, _position.y + _size.y);
    corners[LowerRight] = LR;

}

void ViewFrame::setMask(Point pos, Point sz) {
    maskCorners[UpperLeft] = pos;

    Point LL(pos.x + sz.x, pos.y);
    maskCorners[LowerLeft] = LL;

    Point UR(pos.x, pos.y + sz.y);
    maskCorners[UpperRight] = UR;

    Point LR(pos.x + sz.x, pos.y + sz.y);
    maskCorners[LowerRight] = LR;

    _maskPosition = pos;
    _maskSize = sz;
}

Point& ViewFrame::getMaskPosition() {
    return _maskPosition;
}

Point& ViewFrame::getMaskSize() {
    return _maskSize;
}

Point * ViewFrame::getMaskCorners() {
    return maskCorners;
}
    
Point * ViewFrame::getCorners() {
    return corners;
}


void ViewFrame::setPosition(Point p) {
    _position = p;
    setCorners();
}

Point &ViewFrame::getPosition() {
    return _position;
}

void ViewFrame::setSize(Point s) {
    _size = s;
    setCorners();
}

void ViewFrame::setScreen(Screen *screen) {
    _screen = screen;
}
