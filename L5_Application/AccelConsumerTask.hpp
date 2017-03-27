#ifndef _CONSUMER_TASK_HPP
#define _CONSUMER_TASK_HPP

#include "scheduler_task.hpp"
#include "FreeRTOS.h"

class AccelConsumerTask : public scheduler_task {
private:
    QueueHandle_t xDirQueue;
public:
    AccelConsumerTask(uint8_t priority);
    virtual ~AccelConsumerTask();
    virtual bool init(void);
    virtual bool run(void *param);
};

#endif