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
#include "OnScreenData.hpp"
#include "queue.h"
//#include <sstream>
//#include <string>

RemoteUI::RemoteUI(uint8_t priority) : scheduler_task("LCD", 12000 , priority, NULL), tft(0, 0, 0, 0, 0) {
    //    setRunDuration(33); // 30 frames per second refresh
        setRunDuration(33); // 30 frames per second refresh

}

bool RemoteUI::init(void) {
    qh = xQueueCreate(1,sizeof(OnScreenData));
    printf("init screen (even though unnecessary)\n");
    //mainScreen = new Screen();
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
    static DrawFrame *rec1;
    static int32_t *border1params;
    static int32_t *border2params;

    static NumberFrame *numFrame;


    static const char * bname = "<eBoard>";


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

        rec1 = new DrawFrame(Point(240,3),Point(0,SCREEN_HEIGHT/2 - (rec_height/2)));
        rec1->setMask(Point(0,0),Point(SCREEN_WIDTH,SCREEN_HEIGHT));

        rec1->addDrawing(d_rect,border1params);
        rec1->addDrawing(d_rect,border2params);

        numFrame = new NumberFrame(Point(50,50),Point(SCREEN_WIDTH/2 + 70,150),6);

        battery = new DrawFrame(Point(40,20),Point(200,10));

        tip = (int32_t*)malloc(4 * sizeof(int32_t));
        tip[0] = 1;
        tip[1] = 17;
        tip[2] = 3;
        tip[3] = 22;

        bl1 = (int32_t*)malloc(4 * sizeof(int32_t));
        bl1[0] = 5;
        bl1[1] = 14;
        bl1[2] = 7;
        bl1[3] = 25;


        bl2 = (int32_t*)malloc(4 * sizeof(int32_t));
        bl2[0] = 9;
        bl2[1] = 14;
        bl2[2] = 11;
        bl2[3] = 25;

        bl3 = (int32_t*)malloc(4 * sizeof(int32_t));
        bl3[0] = 13;
        bl3[1] = 14;
        bl3[2] = 15;
        bl3[3] = 25;

        bl4 = (int32_t*)malloc(4 * sizeof(int32_t));
        bl4[0] = 17;
        bl4[1] = 14;
        bl4[2] = 19;
        bl4[3] = 25;

        box = (int32_t*)malloc(4 * sizeof(int32_t));
        box[0] = 3;
        box[1] = 12;
        box[2] = 21;
        box[3] = 27;

        battery->addDrawing(d_rect,tip);
        battery->addDrawing(d_rect,bl1);
        battery->addDrawing(d_rect,bl2);
        battery->addDrawing(d_rect,bl3);
        battery->addDrawing(d_rect,bl4);
        battery->addDrawing(d_rect,box);

        // rec2 = new DrawFrame(Point(240,3),Point(margin, SCREEN_HEIGHT/2 + (rec_height/2) - 3));
        // rec2->setMask(Point(0,0),Point(SCREEN_WIDTH,SCREEN_HEIGHT));

        pwrLabelPos = new Point();
        pwrLvlPos = new Point();
        pwrLabel = new TextFrame(Point(150,50),*pwrLabelPos,test,3);
        pwrLvl = new TextFrame(Point(50,50),*pwrLvlPos,"",3);
        nameFrame = new TextFrame(Point(200,50),Point(30,280),bname,4);
        TextFrame *txt = new TextFrame(Point(200,50),Point(20,20),"BOARD CONNECTION: ACTIVE",1);
        TextFrame *txt2 = new TextFrame(Point(200,50),Point(20,40),"OP. TIME ELAPSED: XX:XX",1);
        TextFrame *txt3 = new TextFrame(Point(200,50),Point(20,60),"EST. TIME REMAINING: XX:XX",1);

        manager->addFrame(nameFrame);
        manager->addFrame(pwrLabel);
        manager->addFrame(pwrLvl);
        manager->addFrame((ViewFrame*)rec1);
        manager->addFrame((ViewFrame*)battery);
        manager->addFrame((ViewFrame*)txt);
        manager->addFrame((ViewFrame*)txt2);
        manager->addFrame((ViewFrame*)txt3);

        pwrLabel->setMask(Point(0,140),Point(240,40));
        pwrLvl->setMask(Point(0,140),Point(240, 40));
        pwrLabelPos->x = 10 + SCREEN_WIDTH/2 - (strlen(test)* font_size/2 * font_width);
        pwrLvlPos->x = 15;

        first_time = false;
    }

    //mainScreen->clear();
    static int32_t scroller = 150;
    static int16_t testVal = 0;
    static char testValStr[8];


    if (SW.getSwitch(1)){
        scroller++;
//        printf("inc\n");
    }

    if (SW.getSwitch(2)){
        scroller--;
//        printf("dec\n");
    }

    if (SW.getSwitch(3)) {
        testVal++;
    }
    if (SW.getSwitch(4)) {
        testVal--;
    }



    snprintf(testValStr,8,"%d",testVal);
    //    pwrLvl->setText(itoa(testVal));
//    static stringstream ss;
//    ss.str(std::string());
//    ss << testVal;
//    printf("stream: %s\n",ss.str().c_str());
////    const char *testValStr = ss.str().c_str();
//    pwrLvl->setText(ss.str().c_str());
    pwrLvl->setText(testValStr);
    pwrLvlPos->y = scroller;
    pwrLabelPos->y = scroller;

//    printf("setting position\n");
    pwrLvl->setPosition(*pwrLvlPos);
    pwrLabel->setPosition(*pwrLabelPos);
    // Point currentPOS;
    // currentPOS = pwrLabel->getPosition();
    // printf("pwrLabel position: %d %d\n", currentPOS.x, currentPOS.y);
    // print(mainScreen,(char *)test,SCREEN_WIDTH/2 - (strlen(test)* font_size/2 * font_width),scroller,font_size);

    // drawRec(mainScreen, margin,SCREEN_HEIGHT/2 - (rec_height/2),SCREEN_WIDTH - margin,SCREEN_HEIGHT/2 - (rec_height/2)+3);
    // drawRec(mainScreen,margin, SCREEN_HEIGHT/2 + (rec_height/2) - 3,SCREEN_WIDTH - margin,SCREEN_HEIGHT/2 + (rec_height/2));

    manager->drawFrames();
    // mainScreen->drawBuffer();

    return true;
}

//RemoteUI::~RemoteUI() {
//
//}
