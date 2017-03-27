#include "SemaphoreTask.hpp"
#include "eint.h"
// #include "FreeRTOS.h"
#include "semphr.h"
#include <stdio.h>

SemaphoreHandle_t btnPressSem; 

void buttonPressed(void)
{
    if (btnPressSem)
    {
        xSemaphoreGiveFromISR(btnPressSem, NULL);
    }
}

SemaphoreTask::SemaphoreTask(uint8_t priority) :
    scheduler_task("sematask", 1024, priority, NULL)
{

}

SemaphoreTask::~SemaphoreTask()
{

}

bool SemaphoreTask::init(void)
{
    btnPressSem = xSemaphoreCreateBinary();
    if (btnPressSem == NULL)
    {
        return false;
    }

    eint3_enable_port2(1, eint_rising_edge, buttonPressed);
    return true;
}

bool SemaphoreTask::run(void *param)
{
    if (btnPressSem)
    {
        xSemaphoreTake(btnPressSem, portMAX_DELAY);
        printf("Button pressed\n");
    }

    return true;
}

