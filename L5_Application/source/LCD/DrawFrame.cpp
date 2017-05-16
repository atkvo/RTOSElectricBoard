
#include "DrawFrame.hpp"

DrawFrame::DrawFrame() : ViewFrame() {

}

DrawFrame::DrawFrame(Point sz, Point pos) : ViewFrame(sz,pos) {
    
}

void DrawFrame::addDrawing(drawing d, int32_t *params, int32_t numParams) {
    if (drawComSize < MAX_DRAWINGS) {
        drawCommands[drawComSize].d_shape = d;
        drawCommands[drawComSize].dparams = params;
        drawCommands[drawComSize].dparamSize = numParams;
        drawComSize++;
    } else {
        printf("Max drawings (%d) reached for this frame\n", MAX_DRAWINGS);
    }
}


void DrawFrame::draw() {
    setDraw();
    static int32_t i = 0;
    for (i = 0; i < drawComSize; i++) {
        static func_data *d_dat;
        d_dat = &drawCommands[i];
        if          (d_dat->d_shape == d_line) {
            drawLine(_screen,   d_dat->dparams[0] + _position.x,
                                d_dat->dparams[1] + _position.y,
                                d_dat->dparams[2] + _position.x,
                                d_dat->dparams[3] + _position.y);
        } else if   (d_dat->d_shape == d_rect) {
            drawRec(_screen,   d_dat->dparams[0] + _position.x,
                                d_dat->dparams[1] + _position.y,
                                d_dat->dparams[2] + _position.x,
                                d_dat->dparams[3] + _position.y);
        } else {
            printf("invalid shape\n");
        }
    }
    changed = false; // redraw is now complete
}

void DrawFrame::erase() {
    setErase();
    static int32_t i = 0;
    for (i = 0; i < drawComSize; i++) {
        static func_data *d_dat;
        d_dat = &drawCommands[i];
        if          (d_dat->d_shape == d_line) {
            drawLine(_screen,   d_dat->dparams[0] + _position.x,
                                d_dat->dparams[1] + _position.y,
                                d_dat->dparams[2] + _position.x,
                                d_dat->dparams[3] + _position.y);
        } else if   (d_dat->d_shape == d_rect) {
            drawRec(_screen,   d_dat->dparams[0] + _position.x,
                                d_dat->dparams[1] + _position.y,
                                d_dat->dparams[2] + _position.x,
                                d_dat->dparams[3] + _position.y);
        } else {
            printf("invalid shape\n");
        }
    }
    changed = true;
}

