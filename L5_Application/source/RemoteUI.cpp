/*
 * RemoteUI.cpp
 *
 *  Created on: May 15, 2017
 *      Author: James
 */
#include "RemoteUI.hpp"
#include "io.hpp"
#include <string.h>
#include <stdio.h>
//#include <stdlib.h>
#include "LCD/draw.hpp"
#include "queue.h"
#include "shared_handles.h"
#include "printf_lib.h"
#include "source/LCD/pixel.h"

//#include <sstream>
//#include <string>

RemoteUI::RemoteUI(uint8_t priority) : scheduler_task("LCD", 2048 , priority, NULL), tft(0, 0, 0, 0, 0) {
    //    setRunDuration(33); // 30 frames per second refresh
        setRunDuration(32); // 24 frames per second refresh


}

bool RemoteUI::init(void) {
//    bool screenQueueShared = false;
//    qh = xQueueCreate(20, sizeof(OnScreenData));
//    if (qh)
//    {
//        screenQueueShared = addSharedObject(shared_screenQueue, qh);
//    }


    printf("init screen (even though unnecessary)\n");
    mainScreen = new Screen();
    manager = new FrameManager(mainScreen);

//    mainScreen->tft = &tft;
    setTFT(&tft);

    printf("RESETING...\n");
    tft.reset();

    printf("INITIATING DISPLAY...\n");
    tft.initDisplay();

    tft.fillScreen(0x0000);

    printf("filled screen...\n");

    return true;
}

bool RemoteUI::run(void * p) {
    static bool first_time = true;
    static TextFrame *nameFrame;
    static TextFrame *pwrLabel;
    static TextFrame *pwrLvl;
    static DrawFrame *pwrLvlBorders;
    static int32_t *border1params;
    static int32_t *border2params;

    static NumberFrame *numFrame;


    static const char * bname = "<eBoard>";

    static DrawFrame *batteryBorders;
    static int32_t *batteryBorderparams;
    static DrawFrame *battery;
    static int32_t *tip;
    static int32_t *bl1;
    static int32_t *bl2;
    static int32_t *bl3;
    static int32_t *bl4;
    static int32_t *box;

    static const char * test = "PWR LVL";
    static int16_t font_width = 6;
    static int16_t font_size = 3;
    static Point *pwrLabelPos;
    static Point *pwrLvlPos;

    static int32_t margin = 15;
    static int32_t rec_height = 50;


    if (first_time) {
        printf("first run -----\n");
        setFont(mainScreen,(uint8_t*)&homespun_font);
        printf("set font -----\n");

        border1params = (int32_t*)malloc(4 * sizeof(int32_t));
        border1params[0] = margin;
        border1params[1] = 0;
        border1params[2] = SCREEN_WIDTH - margin;
        border1params[3] = 3;

        border2params = (int32_t*)malloc(4 * sizeof(int32_t));
        border2params[0] = margin;
        border2params[1] = rec_height - 3;
        border2params[2] = SCREEN_WIDTH - margin;
        border2params[3] = rec_height;
        printf("set border parameters -----\n");

        pwrLvlBorders = new DrawFrame(Point(240,3),Point(0,SCREEN_HEIGHT/2 - (rec_height/2)));
        pwrLvlBorders->setMask(Point(0,0),Point(SCREEN_WIDTH,SCREEN_HEIGHT));

        pwrLvlBorders->addDrawing(d_rect,border1params);
        pwrLvlBorders->addDrawing(d_rect,border2params);
        printf("created draw frame and added border parameters -----\n");

//        numFrame = new NumberFrame(Point(50,50),Point(SCREEN_WIDTH/2 + 70,150),6);

        printf("created draw frame for numframe -----\n");

        battery = new DrawFrame(Point(30,20),Point(200,5));
        printf("created draw frame for battery -----\n");


        batteryBorderparams = (int32_t*)malloc(4 * sizeof(int32_t));
        batteryBorderparams[0] = margin;
        batteryBorderparams[1] = 0;
        batteryBorderparams[2] = SCREEN_WIDTH - margin;
        batteryBorderparams[3] = 3;
        batteryBorders = new DrawFrame(Point(240,3),Point(0,35));
        batteryBorders->addDrawing(d_rect,border1params);



        tip = (int32_t*)malloc(4 * sizeof(int32_t));
        tip[0] = 1;
        tip[1] = 7;
        tip[2] = 3;
        tip[3] = 12;

        bl1 = (int32_t*)malloc(4 * sizeof(int32_t));
        bl1[0] = 5;
        bl1[1] = 4;
        bl1[2] = 7;
        bl1[3] = 15;


        bl2 = (int32_t*)malloc(4 * sizeof(int32_t));
        bl2[0] = 9;
        bl2[1] = 4;
        bl2[2] = 11;
        bl2[3] = 15;

        bl3 = (int32_t*)malloc(4 * sizeof(int32_t));
        bl3[0] = 13;
        bl3[1] = 4;
        bl3[2] = 15;
        bl3[3] = 15;

        bl4 = (int32_t*)malloc(4 * sizeof(int32_t));
        bl4[0] = 17;
        bl4[1] = 4;
        bl4[2] = 19;
        bl4[3] = 15;

        box = (int32_t*)malloc(4 * sizeof(int32_t));
        box[0] = 3;
        box[1] = 2;
        box[2] = 21;
        box[3] = 17;

        battery->addDrawing(d_rect,tip);
        battery->addDrawing(d_rect,bl1);
        battery->addDrawing(d_rect,bl2);
        battery->addDrawing(d_rect,bl3);
        battery->addDrawing(d_rect,bl4);
        battery->addDrawing(d_rect,box);

        printf("added drawings\n");

        // rec2 = new DrawFrame(Point(240,3),Point(margin, SCREEN_HEIGHT/2 + (rec_height/2) - 3));
        // rec2->setMask(Point(0,0),Point(SCREEN_WIDTH,SCREEN_HEIGHT));

        pwrLabelPos = new Point();
        pwrLvlPos = new Point();
        printf("made points\n");
        pwrLabel = new TextFrame(Point(150,50),*pwrLabelPos,test,3);
        printf("made pwrLabel\n");
        pwrLvl = new TextFrame(Point(50,50),*pwrLvlPos,"",3);
        printf("made pwrLevll\n");
        nameFrame = new TextFrame(Point(200,50),Point(50,280),bname,3);
        printf("made text frames\n");
        TextFrame *connectionLabel = new TextFrame(Point(200,50),Point(20,55),"CONN: ACTIVE",2);
        TextFrame *opTimeLabel = new TextFrame(Point(200,50),Point(20,80),"OP. TIME: XX:XX",2);
        TextFrame *timeRemLabel = new TextFrame(Point(200,50),Point(20,105),"TIME LEFT: XX:XX",2);

        printf("made txt frames drawings\n");

        manager->addFrame(nameFrame);
        manager->addFrame(pwrLabel);
        manager->addFrame(pwrLvl);
        manager->addFrame((ViewFrame*)pwrLvlBorders);
        manager->addFrame((ViewFrame*)batteryBorders);
        manager->addFrame((ViewFrame*)battery);
        manager->addFrame((ViewFrame*)connectionLabel);
        manager->addFrame((ViewFrame*)opTimeLabel);
        manager->addFrame((ViewFrame*)timeRemLabel);

        pwrLabel->setMask(Point(0,140),Point(240,40));
        pwrLvl->setMask(Point(0,140),Point(240, 40));
//        pwrLabelPos->x = 10 + SCREEN_WIDTH/2 - (strlen(test)* font_size/2 * font_width);
        pwrLabelPos->x = 20;
        pwrLvlPos->x = 160;

        printf("end first run\n");
        first_time = false;
    }

    //mainScreen->clear();
    static int32_t scroller = 150;
    static int16_t testVal = 0;
    static char testValStr[8];


    if (SW.getSwitch(1)){
        scroller++;
        printf("inc\n");
    }

    if (SW.getSwitch(2)){
        scroller--;
        printf("dec\n");
    }

    if (SW.getSwitch(3)) {
        testVal++;
    }
    if (SW.getSwitch(4)) {
        testVal--;
    }

	mesh_packet_t packet;
    OnScreenData dataRx;
    if (wireless_get_rx_pkt(&packet, 500))
    {
    	dataRx.eBreakActive = packet.data[0];
    	if (packet.data[2] == 0)
    	{
    		dataRx.accelerometer = packet.data[1];
    	}
    	else
    	{
    		dataRx.accelerometer = (packet.data[1] << 8) + packet.data[2];
    	}
    }

//    xQueueReceive( qh, &OSD, 0 );
//    testVal = OSD.powerLevel;
    snprintf(testValStr,8,"%d",testVal);


    pwrLvl->setText(testValStr);
    pwrLvlPos->y = scroller;
    pwrLabelPos->y = scroller;

//    printf("setting position\n");
    pwrLvl->setPosition(*pwrLvlPos);
    pwrLabel->setPosition(*pwrLabelPos);

    manager->drawFrames();

    return true;
}

//RemoteUI::~RemoteUI() {
//
//}
