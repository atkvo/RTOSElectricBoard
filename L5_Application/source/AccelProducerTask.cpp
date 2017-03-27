#include "AccelProducerTask.hpp"
#include "acceleration_sensor.hpp"
#include <stdio.h>

AccelProducerTask::AccelProducerTask(uint8_t priority) :
    scheduler_task("accel-producer", 1024, priority, NULL)
{
}

AccelProducerTask::~AccelProducerTask()
{
    if (xDirQueue)
    {
        vQueueDelete(xDirQueue);
    }
}

bool AccelProducerTask::init(void)
{
    /* Initialize queue here and accelerometer */
    xDirQueue = xQueueCreate(10, sizeof(orientation_t));
    if (xDirQueue == NULL)
    {
        return false;
    }

    addSharedObject(shared_SensorAccel, xDirQueue);

    bool initOK = Acceleration_Sensor::getInstance().init();

    return initOK;
}

bool AccelProducerTask::run(void *param)
{
    if (xDirQueue == NULL)
    {
        return false;
    }

    orientation_t orientation = invalid;

    int16_t xValue = Acceleration_Sensor::getInstance().getX();
    // int16_t yValue = Acceleration_Sensor::getInstance().getY();
    int16_t zValue = Acceleration_Sensor::getInstance().getZ();

    // printf("%d %d %d\n", xValue, yValue, zValue);

    if (xValue < -500)
    {
        orientation = right;
    }
    else if (xValue > 500)
    {
        orientation = left;
    }
    else if (zValue > 500) 
    {
        orientation = up;
    }
    else if (zValue < -500)
    {
        orientation = down;
    }

    /* Get accelerometer data and push it to the queue every 1 second */
    xQueueSend(getSharedObject(shared_SensorAccel), &orientation, portMAX_DELAY);

    vTaskDelay(500);
    return true;
}