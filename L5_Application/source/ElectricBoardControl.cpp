#include "ElectricBoardControl.hpp"
#include "lpc_pwm.hpp"
#include <stdio.h>

ElectricBoardControl::ElectricBoardControl(uint8_t priority) :
    scheduler_task("eboardctrl", 1024, priority, NULL)
{
}

ElectricBoardControl::~ElectricBoardControl()
{
}

bool ElectricBoardControl::init(void)
{
    pwmChannel = new PWM(PWM::pwm1, 100);
    currentPWM = 0;
    return true;
}

bool ElectricBoardControl::run(void *param)
{
    currentPWM++;
    if (currentPWM > 100) {
        currentPWM = 0;
    }

    pwmChannel->set(currentPWM);

    vTaskDelay(50);
    return true;
}