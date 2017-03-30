#ifndef _ELECTRIC_BOARD_CONTROL_HPP
#define _ELECTRIC_BOARD_CONTROL_HPP

#include "scheduler_task.hpp"
#include "FreeRTOS.h"
#include "lpc_pwm.hpp"

class ElectricBoardControl: public scheduler_task {
private:
    bool suspendFirstRun;
    QueueHandle_t xDirQueue;
    PWM *pwmChannel;
    float currentPWM;
public:
    ElectricBoardControl(uint8_t priority);
    virtual ~ElectricBoardControl();
    virtual bool init(void);
    virtual bool run(void *param);
};

#endif