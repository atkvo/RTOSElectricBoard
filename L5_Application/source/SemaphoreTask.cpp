#include "SemaphoreTask.hpp"
#include "eint.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "timers.h"
#include <stdio.h>

SemaphoreHandle_t btnPressSem; 
TimerHandle_t debounceTimer;
bool debounceActive = false;

void debounceCallback(TimerHandle_t timer)
{
    debounceActive = false;
}

void buttonPressed(void)
{
    if (btnPressSem == NULL)
    {
        return;
    }

    if (!debounceActive)
    {
        xSemaphoreGiveFromISR(btnPressSem, NULL);
        debounceActive = true;
    }
}

SemaphoreTask::SemaphoreTask(uint8_t priority) :
    scheduler_task("sematask", 1024, priority, NULL)
{

}

SemaphoreTask::~SemaphoreTask()
{
    vSemaphoreDelete(btnPressSem);
    xTimerDelete(debounceTimer, 100);
}

bool SemaphoreTask::init(void)
{
    btnPressSem = xSemaphoreCreateBinary();
    if (btnPressSem == NULL)
    {
        return false;
    }

    const int DEBOUNCE_MS = 150;
    debounceTimer = xTimerCreate("dbounce-timer", DEBOUNCE_MS, pdFALSE, NULL, debounceCallback);
    if (debounceTimer == NULL)
    {
        printf("Error creating timer\n");
        return false;
    }

    xTimerStop(debounceTimer, 100);
    eint3_enable_port2(1, eint_rising_edge, buttonPressed);
    return true;
}

bool SemaphoreTask::run(void *param)
{
    if (btnPressSem)
    {
        xSemaphoreTake(btnPressSem, portMAX_DELAY);
        xTimerStart(debounceTimer, 100);
        printf("Button pressed\n");
    }

    return true;
}

