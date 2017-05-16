
#ifndef VIEW_FRAME_HPP
#define VIEW_FRAME_HPP

#include "Point.hpp"
#include "draw.hpp"
#include "animate.hpp"



class ViewFrame {
    protected:
    frame_id id;
    Point _size;     
    Point _position; // absolute coordinates
    Point corners[4] = {0};
    Point _maskSize;
    Point _maskPosition;
    Point maskCorners[4] = {0};

    anim_id anim;
    Screen *_screen;
    void setCorners();

    public:
    bool active = true;
    bool changed = true; // only draw if changed
    ViewFrame();
    ViewFrame(Screen &screen);
    ViewFrame(Point size, Point position);
    ViewFrame(Screen &screen, Point size, Point position);
    void setMask(Point pos, Point size);
    void setMaskCorners();
    Point &getMaskPosition();
    Point &getMaskSize();
    Point *getMaskCorners();
    Point *getCorners();
    void setSize(Point &size);
    void setX(int32_t x);
    void setY(int32_t y);
    void setPosition(Point p);
    void setPosition(int32_t x, int32_t y);
    Point &getPosition();
    void setScreen(Screen *screen);
    virtual void draw(void) = 0; // every frame must be able to draw itself
    virtual void erase(void) = 0; // every frame must be able to draw itself

};

#endif
