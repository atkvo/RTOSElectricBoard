
#include "TextFrame.hpp"
#include "string.h"
#include "draw.hpp"
#include "Screen.hpp"

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
    setText(_text);
    textSize = txSize;
}

void TextFrame::setText(const char *_text) {
    static uint16_t size;
    size = strlen(_text) + 1;
    if (size < 32) {
        strncpy(text,_text,size);
    } else {
        strncpy(text,_text,31);
        text[31] = '\0';
    }
}

void TextFrame::draw() {
    print(_screen,text, _position.x,_position.y,textSize);
}

TextFrame::~TextFrame() {

}