
#include <stdio.h>
#include "command_handler.hpp"
#include "FlashMemoryDriver.hpp"

CMD_HANDLER_FUNC(flashHandler) {
    static FlashMemoryDriver flashDriver(SSP1);
    /* handle here */

    if (cmdParams.compareToIgnoreCase("status")) {
        flashDriver.PrintStatusRegisters();
    }
    else if (cmdParams.compareToIgnoreCase("update")) {
        flashDriver.UpdateStatusRegisters(false);
    }
    else if (cmdParams.compareToIgnoreCase("upprint")) {
        flashDriver.UpdateStatusRegisters(false);
        flashDriver.PrintStatusRegisters();
    }
    else {
        printf("No command specified.\n");
    }

    return true;
}