
#ifndef FRAMEMANAGER_HPP
#define FRAMEMANAGER_HPP

#include "ViewFrame.hpp"
#include "TextFrame.hpp"
#include "DrawFrame.hpp"
#include "ScrollFrame.hpp"
#include "Screen.hpp"
#include "Point.hpp"
#include "TFTLCD.h"

typedef struct {
    enum {View_Frame, Text_Frame, Draw_Frame, Scoll_Frame} type;
    ViewFrame *vframe;
} frame_t;

class FrameManager {
    ViewFrame *frames[32] = {0}; 
    frame_id next_frame_id;
    Screen *_screen;
    Animation *_aniManager;
public:
    TFTLCD *tft;
    FrameManager(Screen *screen);
    frame_id addFrame(ViewFrame *frame);
    bool delFrame(frame_id id);
    bool activateFrame(frame_id id);    // indicates whether frame should be drawn  
    bool deactivateFrame(frame_id id);
    void drawFrames(void);
};

#endif
