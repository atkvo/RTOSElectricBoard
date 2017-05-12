#ifndef _ELECTRIC_BOARD_CONTROL_HPP
#define _ELECTRIC_BOARD_CONTROL_HPP

#include "scheduler_task.hpp"
#include "FreeRTOS.h"
#include "lpc_pwm.hpp"

class ElectricBoardControl: public scheduler_task {
public:
    ElectricBoardControl(uint8_t priority);
    virtual ~ElectricBoardControl();
    virtual bool init(void);
    virtual bool run(void *param);
private:
    const double MAX_DUTY = 8.7;         // % Duty Cycle
    const double IDLE_DUTY = 7.2;        // % Duty Cycle
    const double MIN_DUTY = 4.5;

    QueueHandle_t commandQueue;
    PWM *motorChannel1;
    PWM *motorChannel2;
    uint16_t driveTimeout = 500;

    void driveMotors(float powerLevel);
};

#endif
