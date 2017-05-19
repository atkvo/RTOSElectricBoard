#ifndef L5_APPLICATION_ONSCREENDATA_HPP_
#define L5_APPLICATION_ONSCREENDATA_HPP_

struct OnScreenData
{
    bool connectionStatus = false;
    int eBreakActive = 0;
    int powerLevel = 0;
    float batteryLevel = 0;
    int accelerometer = 0;
};

#endif /* L5_APPLICATION_REMOTECONTROL_HPP_ */
