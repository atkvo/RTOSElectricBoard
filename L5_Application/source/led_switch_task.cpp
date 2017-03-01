/*
 * led_switch_task.cpp
 *
 *  Created on: Feb 10, 2017
 *      Author: andrew
 */

#include <led_switch_task.hpp>
#include "InternalIO.hpp"
#include "LPC17xx.h"

led_switch_task::led_switch_task(uint8_t priority, bool useInternals=false)
    : scheduler_task("LED Switch", 1024, priority, (void*)useInternals)
{
}


bool led_switch_task::taskEntry() {
    // set P1.0 as output
    uint32_t mask = 0;
    mask |= 1 << LED0_BIT;
    mask |= 1 << LED1_BIT;
    mask |= 1 << LED2_BIT;
    mask |= 1 << LED3_BIT;
    LPC_GPIO1->FIODIR |= mask;
    LPC_GPIO1->FIOCLR = mask;

    // switch is active high
    mask = 0;
    mask |= 1 << SW0_BIT;
    mask |= 1 << SW1_BIT;
    mask |= 1 << SW2_BIT;
    mask |= 1 << SW3_BIT;
    LPC_GPIO1->FIODIR &= ~(mask);

    // setup external peripherals
#ifdef LEARN_EXTRA
    // Dynamically selects the PINMODE register based on the port/bit selection
     uint32_t * ledPinModeReg = getPinModeReg(EXT_LED_PORT_NUM, EXT_LED_BIT);
    if (ledPinModeReg != NULL) {
        *ledPinModeReg |= (2 << EXT_LED_MODE_BIT);   // disable pull-up/pull-down
    }
#else
     LPC_PINCON->PINMODE0 |= (2 << EXT_LED_MODE_BIT);
#endif
    EXT_LED_PORT->FIODIR |= (1 << EXT_LED_BIT);
    EXT_LED_PORT->FIOCLR = (1 << EXT_LED_BIT);

    EXT_SW_PORT->FIODIR &= ~(1 << EXT_SW_BIT);
    LPC_PINCON->PINMODE0 |= (2 << EXT_SW_MODE_BIT);  // disable pull-up/pull-down
    return true;
}

bool led_switch_task::run(void *p) {
    if ((bool)p) {
        // bool is_pressed = false;
        // for (int i = 0; i < MAX_SWITCHES; i++) {
        //     is_pressed = is_switch_pressed(i);
        //     set_internal_led(i, is_pressed);
        // }
    }
    set_external_led(is_external_switch_pressed());
    return true;
}

void led_switch_task::set_external_led(bool state) {
    if (state) {
        EXT_LED_PORT->FIOCLR = 1 << EXT_LED_BIT;
    }
    else {
        EXT_LED_PORT->FIOSET = 1 << EXT_LED_BIT;
    }
}

bool led_switch_task::set_internal_led(uint8_t ledIndex, bool state) {
    if (ledIndex >= 0 && ledIndex < MAX_INTERNAL_LEDS) {
        // LEDS are active LOW
        int8_t ledBit = get_led_bit(ledIndex);
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
    else {
        return false;
    }
}

bool led_switch_task::is_external_switch_pressed() {
    if (EXT_SW_PORT->FIOPIN & (1 << EXT_SW_BIT)) {
        return true;
    }
    else {
        return false;
    }
}
bool led_switch_task::is_switch_pressed(uint8_t switchIndex) {
    if (switchIndex >= 0 && switchIndex < MAX_SWITCHES) {
        return LPC_GPIO1->FIOPIN & (1 << get_switch_bit(switchIndex));
    }
    else {
        return false;
    }
}

int8_t led_switch_task::get_switch_bit(uint8_t switchIndex) {
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

int8_t led_switch_task::get_led_bit(uint8_t ledIndex) {
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

#ifdef LEARN_EXTRA
uint32_t * led_switch_task::getPinModeReg(uint8_t port, uint8_t bit) {
    if (port >= 0 && port <= 9) {
        uint32_t * pinModeReg = (uint32_t*)&LPC_PINCON->PINMODE0;
        pinModeReg = pinModeReg + (sizeof(uint32_t)*port);
        if (bit > 15) {
            pinModeReg += 1;
        }
        return pinModeReg;
    }
    else {
        return NULL;
    }
}
#endif
