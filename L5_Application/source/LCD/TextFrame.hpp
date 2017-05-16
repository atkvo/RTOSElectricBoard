
#ifndef TEXTFRAME_HPP
#define TEXTFRAME_HPP

#include "draw.hpp"
#include "ViewFrame.hpp"


class TextFrame : public ViewFrame {
    char text[32];
    uint8_t textSize;
public:
    TextFrame();
    TextFrame(Screen &screen);
    TextFrame(Screen &screen, Point pos, Point sz);
    TextFrame(Point _size, Point _position);
    TextFrame(Screen &screen, Point pos, Point sz, const char *_text);
    TextFrame(Point _size, Point _position,const char *_text, uint8_t txSize = 1);

    void setText(const char *_text);
    void draw();
    ~TextFrame();
};


#endif 
