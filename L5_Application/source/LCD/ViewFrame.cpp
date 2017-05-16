
#include "ViewFrame.hpp"
    // Screen *_screen;

ViewFrame::ViewFrame() {
    Point p;
    _position = p;
}

ViewFrame::ViewFrame(Screen &screen) : _size(), _position(), _screen(&screen) {
    setCorners();
    _screen = &screen;

}

ViewFrame::ViewFrame(Point size, Point position) :  _size(size), _position(position), corners(),_maskSize(SCREEN_WIDTH,SCREEN_HEIGHT),_maskPosition() {
    setCorners();
    setMaskCorners();
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

void ViewFrame::setMaskCorners() {
    maskCorners[UpperLeft] = _maskPosition;

    Point LL(_maskPosition.x + _maskSize.x, _maskPosition.y);
    maskCorners[LowerLeft] = LL;

    Point UR(_maskPosition.x, _maskPosition.y + _maskSize.y);
    maskCorners[UpperRight] = UR;

    Point LR(_maskPosition.x + _maskSize.x, _maskPosition.y + _maskSize.y);
    maskCorners[LowerRight] = LR;
}

void ViewFrame::setMask(Point pos, Point sz) {
    erase();
    _maskPosition = pos;
    _maskSize = sz;
    setMaskCorners();
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
    if (p.y!= _position.y || p.x != _position.x ){
        erase();
        _position = p;
        setCorners();
    }
}

Point &ViewFrame::getPosition() {
    return _position;
}

void ViewFrame::setSize(Point &size) {
    if (size.y!= _size.y || size.x != _size.x ){
        erase();
        _size = size;
        setCorners();
    }
}

void ViewFrame::setScreen(Screen *screen) {
    _screen = screen;
}
