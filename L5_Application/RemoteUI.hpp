/*
 * RemoteUI.hpp
 *
 *  Created on: May 15, 2017
 *      Author: James
 */

#ifndef L5_APPLICATION_REMOTEUI_HPP_
#define L5_APPLICATION_REMOTEUI_HPP_

#include "tasks.hpp"
#include "TFTLCD.h"

#include "source/LCD/FrameManager.hpp"
#include "source/LCD/TextFrame.hpp"
#include "source/LCD/DrawFrame.hpp"
#include "source/LCD/NumberFrame.hpp"
#include "source/LCD/pixel.h"

//#include "source/LCD/Screen.hpp"

class RemoteUI : public scheduler_task {
    TFTLCD tft;
    Screen *mainScreen;
    FrameManager *manager;
    QueueHandle_t qh;


public:
    RemoteUI(uint8_t priority);
    bool init(void);
    bool run(void * p);
//    ~RemoteUI();
};



#endif /* L5_APPLICATION_REMOTEUI_HPP_ */
