
#include "InternalIO.hpp"
#include "LPC17xx.h"

#define LED0_BIT            0
#define LED1_BIT            1
#define LED2_BIT            4
#define LED3_BIT            8

#define SW0_BIT             9
#define SW1_BIT             10
#define SW2_BIT             14
#define SW3_BIT             15

#define INVALID_INDEX       -1
#define MAX_SWITCHES        4
#define MAX_INTERNAL_LEDS   4

InternalIO::InternalIO() {
    initGPIO();
}

InternalIO::~InternalIO() {

}

void InternalIO::initGPIO() {
    uint32_t mask = 0;
    mask |= 1 << LED0_BIT;
    mask |= 1 << LED1_BIT;
    mask |= 1 << LED2_BIT;
    mask |= 1 << LED3_BIT;
    LPC_GPIO1->FIODIR |= mask;
    LPC_GPIO1->FIOCLR = mask;

    // Switches are active high
    mask = 0;
    mask |= 1 << SW0_BIT;
    mask |= 1 << SW1_BIT;
    mask |= 1 << SW2_BIT;
    mask |= 1 << SW3_BIT;
    LPC_GPIO1->FIODIR &= ~(mask);
    //return true;
}

bool InternalIO::IsSwitchPressed(uint8_t switchIndex) {
    if (switchIndex >= 0 && switchIndex < MAX_SWITCHES) {
        return LPC_GPIO1->FIOPIN & (1 << getSwitchBit(switchIndex));
    }
    else {
        return false;
    }
}

bool InternalIO::SetInternalLED(uint8_t ledIndex, bool state) {
    if (ledIndex >= MAX_INTERNAL_LEDS) {
        return false;
    }
    // LEDS are active LOW
    int8_t ledBit = getLEDBit(ledIndex);
    if (state) {
        // turn on
        LPC_GPIO1->FIOCLR = 1 << ledBit;
    }
    else {
        LPC_GPIO1->FIOSET = 1 << ledBit;
        // turn off
    }
    return true;
}

bool InternalIO::IsLEDOn(uint8_t ledIndex) {
    if (ledIndex >= MAX_INTERNAL_LEDS) {
        return false;
    }

    int8_t ledBit = getLEDBit(ledIndex);

    // Remember LEDs are active low
    return (LPC_GPIO1->FIOPIN & (1 << ledBit)) ? false : true; 
}

int8_t InternalIO::getLEDBit(uint8_t ledIndex) {
    switch (ledIndex) {
    case 0:
        return LED0_BIT;
    case 1:
        return LED1_BIT;
    case 2:
        return LED2_BIT;
    case 3:
        return LED3_BIT;
    default:
        return INVALID_INDEX;
    }
}

int8_t InternalIO::getSwitchBit(uint8_t switchIndex) {
    switch (switchIndex) {
    case 0:
        return SW0_BIT;
    case 1:
        return SW1_BIT;
    case 2:
        return SW2_BIT;
    case 3:
        return SW3_BIT;
    default:
        return INVALID_INDEX;
    }
}
