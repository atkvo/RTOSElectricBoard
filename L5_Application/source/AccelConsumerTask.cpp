#include "AccelConsumerTask.hpp"
#include "AccelProducerTask.hpp"
#include <stdio.h>

AccelConsumerTask::AccelConsumerTask(uint8_t priority) :
    scheduler_task("accel-consumer", 1024, priority, NULL)
{
}

AccelConsumerTask::~AccelConsumerTask()
{
}


bool AccelConsumerTask::init(void)
{
    return true;
}

bool AccelConsumerTask::run(void *param)
{
    /* Get accelerometer data and push it to the queue every 1 second */
    orientation_t orientation = invalid;
    QueueHandle_t shared_OrientationQueue = getSharedObject(shared_SensorAccel);
    if (shared_OrientationQueue == NULL) 
    {
        return false;
    }

    if (xQueueReceive(shared_OrientationQueue, &orientation, portMAX_DELAY))
    {
        switch(orientation)
        {
            case up:
                printf("UP\n");
                break;
            case down:
                printf("DOWN\n");
                break;
            case left:
                printf("LEFT\n");
                break;
            case right:
                printf("RIGHT\n");
                break;
            default:
                printf("INVALID\n");
                break;
        }
    }
    return true;
}