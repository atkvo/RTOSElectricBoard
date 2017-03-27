#ifndef _ACCEL_PRODUCER_TASK_HPP
#define _ACCEL_PRODUCER_TASK_HPP

#include "scheduler_task.hpp"
#include "FreeRTOS.h"

typedef enum {
    shared_SensorAccel   // x value data for sensor
} sharedHandle_t;

typedef enum {
    invalid,
    up,
    down,
    left,
    right
} orientation_t;

class AccelProducerTask: public scheduler_task {
private:
    bool suspendFirstRun;
    QueueHandle_t xDirQueue;
public:
    AccelProducerTask(uint8_t priority, bool startImmediately);
    virtual ~AccelProducerTask();
    virtual bool init(void);
    virtual bool run(void *param);
};

#endif