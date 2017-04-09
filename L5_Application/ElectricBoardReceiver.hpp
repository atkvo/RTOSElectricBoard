#ifndef _ELECTRICBOARDRECEIVER_HPP_
#define _ELECTRICBOARDRECEIVER_HPP_

#include "scheduler_task.hpp"
#include "FreeRTOS.h"

class ElectricBoardReceiver: public scheduler_task {
public:
    ElectricBoardReceiver(uint8_t priority);
    virtual ~ElectricBoardReceiver();
    virtual bool init(void);
    virtual bool run(void *param);
};

#endif