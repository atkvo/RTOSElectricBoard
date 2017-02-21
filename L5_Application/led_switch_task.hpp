/*
 * led_switch_task.hpp
 *
 *  Created on: Feb 10, 2017
 *      Author: andrew
 */

#ifndef L5_APPLICATION_LED_SWITCH_TASK_HPP_
#define L5_APPLICATION_LED_SWITCH_TASK_HPP_

#include "scheduler_task.hpp"
#include "LPC17xx.h"

#define LED0_BIT            0
#define LED1_BIT            1
#define LED2_BIT            4
#define LED3_BIT            8

#define SW0_BIT             9
#define SW1_BIT             10
#define SW2_BIT             14
#define SW3_BIT             15

#define LEARN_EXTRA // Define this to use a special technique
#define EXT_LED_PORT_NUM    0
#define EXT_LED_PORT        LPC_GPIO0
#define EXT_LED_BIT         1
#define EXT_LED_MODE_BIT    2

#define EXT_SW_PORT 	    LPC_GPIO0
#define EXT_SW_BIT          0
#define EXT_SW_MODE_BIT     0

#define INVALID_INDEX       -1
#define MAX_SWITCHES        4
#define MAX_INTERNAL_LEDS   4

class led_switch_task : public scheduler_task {
public:
	led_switch_task(uint8_t priority, bool useInternals);
	bool taskEntry();
	bool run(void *p);
private:
	enum direction {
	    INPUT,
	    OUTPUT
	};
	bool set_internal_led(uint8_t ledIndex, bool state);
	void set_external_led(bool state);
	bool is_switch_pressed(uint8_t switchIndex);
	bool is_external_switch_pressed();
	int8_t get_switch_bit(uint8_t switchIndex);
	int8_t get_led_bit(uint8_t ledIndex);
//	void setup_led(uint8_t ledIndex, direction dir);
#ifdef LEARN_EXTRA
	uint32_t * getPinModeReg(uint8_t port, uint8_t bit);
#endif
};



#endif /* L5_APPLICATION_LED_SWITCH_TASK_HPP_ */
