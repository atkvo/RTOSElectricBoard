
#ifndef DRAWFRAME_HPP
#define DRAWFRAME_HPP

#define MAX_DRAWINGS 8

#include "ViewFrame.hpp"
#include <stdint.h>
#include <stdio.h>

typedef enum {
    d_none,
    d_line,
    d_rect
} drawing;

typedef struct {
    drawing d_shape;
    int32_t *dparams;
    int32_t dparamSize;
} func_data;



class DrawFrame : public ViewFrame {

    func_data drawCommands[MAX_DRAWINGS];
    uint16_t drawComSize = 0;
    public:
    void addDrawing(drawing d, int32_t *params, int32_t numParams = 4);
    DrawFrame();
    DrawFrame(Point sz, Point pos);
    void draw();
    void erase();

};















#endif
