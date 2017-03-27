
#include <stdio.h>
#include "scheduler_task.hpp"
#include "command_handler.hpp"
#include "FlashMemoryDriver.hpp"
#include "InternalIO.hpp"

CMD_HANDLER_FUNC(orientationCmd) 
{
    scheduler_task *orientationProducer = scheduler_task::getTaskPtrByName("accel-producer");
    if (orientationProducer == NULL)
    {
        return false;
    }

    if (cmdParams == "on")
    {
        orientationProducer->resume();
    }
    else
    {
        orientationProducer->suspend();
    }

    return true;
}

CMD_HANDLER_FUNC(flashHandler) {
    static FlashMemoryDriver flashDriver(SSP1);
    /* handle here */

    if (cmdParams.compareToIgnoreCase("printid")) {
        flashDriver.PrintID();
    }
    else if (cmdParams.compareToIgnoreCase("printstat")) {
        flashDriver.PrintStatusRegisters();
    }
    else if (cmdParams.compareToIgnoreCase("update")) {
        flashDriver.UpdateStatusRegisters(false);
        flashDriver.UpdateIDRegisters(false);
    }
    else if (cmdParams.compareToIgnoreCase("uprintall")) {
        flashDriver.UpdateAllRegisters(true);
    }
    else {
        printf("No command specified.\n");
    }

    return true;
}

CMD_HANDLER_FUNC(ledHandler) {
    static InternalIO ioboss;
    uint8_t ledIndex = -1;
    bool allLEDs = false;
    bool newState = false;
    bool stateQuery = false;

    if (cmdParams.containsIgnoreCase("STATUS")) {
        printf("Under construction\n");
        stateQuery = true;
    }
    else if (cmdParams.containsIgnoreCase("ON")) {
        newState = true;
    }
    else if (cmdParams.containsIgnoreCase("OFF")) {
        newState = false;
    }
    else {
        return false;
    }

    switch(cmdParams[0]) {
        case '0':
            ledIndex = 0;
            ioboss.SetInternalLED(0, newState);
            break;
        case '1':
            ledIndex = 1;
            ioboss.SetInternalLED(1, newState);
            break;
        case '2':
            ledIndex = 2;
            ioboss.SetInternalLED(2, newState);
            break;
        case '3':
            ledIndex = 3;
            ioboss.SetInternalLED(3, newState);
            break;
        case 'a':
        case 'A':
            allLEDs = true;
            break;
        default:
            return false;
    }

    if (stateQuery) {
        if (allLEDs) {
            printf("\n==============\n");
            printf(" LED STATES\n");
            printf("--------------\n");
            printf("  LED 0: %s\n",   ioboss.IsLEDOn(0) ? "ON" : "OFF");
            printf("  LED 1: %s\n",   ioboss.IsLEDOn(1) ? "ON" : "OFF");
            printf("  LED 2: %s\n",   ioboss.IsLEDOn(2) ? "ON" : "OFF");
            printf("  LED 3: %s\n\n", ioboss.IsLEDOn(3) ? "ON" : "OFF");
            printf("==============\n");
        }
        else {
            printf("\n  LED %d: %s\n", ledIndex, ioboss.IsLEDOn(ledIndex) ? "ON" : "OFF");
        }
    }
    else {
        if (allLEDs) {
            ioboss.SetInternalLED(0, newState);
            ioboss.SetInternalLED(1, newState);
            ioboss.SetInternalLED(2, newState);
            ioboss.SetInternalLED(3, newState);
        }
        else {
            ioboss.SetInternalLED(ledIndex, newState);
        }
    }

    return true;
}