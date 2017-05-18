
#include "TextFrame.hpp"
#include "string.h"
#include "draw.hpp"
#include "Screen.hpp"
#include "pixel.h"
#include <stdio.h>

TextFrame::TextFrame() : ViewFrame() {

}

TextFrame::TextFrame(Screen &screen) : ViewFrame(screen) {

}

TextFrame::TextFrame(Screen &screen, Point pos, Point sz) : ViewFrame(screen, pos, sz) {

}

TextFrame::TextFrame(Point _size, Point _position) : ViewFrame(_size,_position) {
    setText("?");
}

TextFrame::TextFrame(Screen &screen, Point pos, Point sz, const char *_text) : ViewFrame(screen, pos, sz) {
    setText(_text);
}

TextFrame::TextFrame(Point _size, Point _position,const char *_text,uint8_t txSize) : ViewFrame(_size,_position) {
//    printf("--constructor start--\n");
    setText(_text);
    textSize = txSize;
//    printf("--constructor end--\n");
}

void TextFrame::setText(const char *_text) {

    if (strcmp(_text, text) != 0) {
        erase();
        static uint16_t size;
        size = strlen(_text) + 1;
        if (size < 32) {
            strncpy(text,_text,size);
        } else {
            strncpy(text,_text,31);
            text[31] = '\0';
        }
    }

}

void TextFrame::draw() {
    setDraw();
    print(_screen,text, _position.x,_position.y,textSize);
    changed = false;

}

void TextFrame::erase() {

    setErase();
//    printf("erase pixel was set\n");
    print(_screen,text, _position.x,_position.y,textSize);
//    printf("printing occured\n");
    changed = true;
}
