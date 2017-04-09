#include "ElectricBoardControl.hpp"
#include "lpc_pwm.hpp"
#include "shared_handles.h"
#include <stdio.h>
#include "queue.h"

ElectricBoardControl::ElectricBoardControl(uint8_t priority) :
    scheduler_task("ctrlbrd", 1024, priority, NULL),
    driveTimeout(500)
{
}

ElectricBoardControl::~ElectricBoardControl()
{
}

bool ElectricBoardControl::init(void)
{
    bool commandQueueShared = false;
    commandQueue = xQueueCreate(20, sizeof(float));
    if (commandQueue)
    {
        commandQueueShared = addSharedObject(shared_ElectricBoardQueue, commandQueue);
    }

    motorChannel1 = new PWM(PWM::pwm1, 0);
    motorChannel2 = new PWM(PWM::pwm2, 0);

    return commandQueue && commandQueueShared;
}

bool ElectricBoardControl::run(void *param)
{
    if (commandQueue == NULL)
    {
        return false;
    }

    // If no command is received in driveTimeout milliseconds, then the drive
    // will be set to 0. The trigger needs to be held to keep driving the motors.
    float driveLevel = 0;
    xQueueReceive(commandQueue, &driveLevel, driveTimeout);
    driveMotors(driveLevel);
    return true;
}

void ElectricBoardControl::driveMotors(float powerLevel)
{
    if (powerLevel > 100) { powerLevel = 100; }
    else if (powerLevel < 0) { powerLevel = 0; }

    motorChannel1->set(powerLevel);
    motorChannel2->set(powerLevel);
}