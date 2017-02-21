#ifndef _INTERNAL_IO_HPP_
#define _INTERNAL_IO_HPP_

#include <stdint.h>

class InternalIO
{
    void initGPIO();
    int8_t getLEDBit(uint8_t ledIndex);
    int8_t getSwitchBit(uint8_t ledIndex);
public:
    InternalIO();
    ~InternalIO();
    bool IsSwitchPressed(uint8_t switchIndex);
    bool SetInternalLED(uint8_t ledIndex, bool state);
    bool IsLEDOn(uint8_t ledIndex);
};

#endif