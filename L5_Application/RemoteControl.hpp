/*
 * RemoteControl.hpp
 *
 *  Created on: May 3, 2017
 *      Author: EEz
 */

#ifndef L5_APPLICATION_REMOTECONTROL_HPP_
#define L5_APPLICATION_REMOTECONTROL_HPP_

#include "scheduler_task.hpp"
#include "FreeRTOS.h"
#include "lpc_pwm.hpp"
#include "semphr.h"

class RemoteControl : public scheduler_task {
public:
    RemoteControl(uint8_t priority);
    virtual ~RemoteControl();
    virtual bool init(void);
    virtual bool run(void *param);
    static SemaphoreHandle_t buttonSignal;
private:
    const float MAX_READ = 4095;
    float powerLevel = 0;

    float calcPower(float input);
    void sendPowerLevel(float powerLevel);
};



#endif /* L5_APPLICATION_REMOTECONTROL_HPP_ */
