#include "RemoteControl.hpp"
#include "lpc_pwm.hpp"
#include "shared_handles.h"
#include <stdio.h>
#include "OnScreenData.hpp"
#include "queue.h"
#include "eint.h"
#include "semphr.h"
#include "adc0.h"
#include "printf_lib.h"
#include "wireless.h"


SemaphoreHandle_t buttonSignal;

RemoteControl::RemoteControl(uint8_t priority) : scheduler_task("remoteControl", 1024, priority, NULL)
{
}

RemoteControl::~RemoteControl()
{
}

void buttonPressInterrupt(void)
{
	xSemaphoreGive(buttonSignal);
}

bool RemoteControl::init(void)
{
	//INIT ADC PIN FOR ANALOG SLIDER
	LPC_PINCON->PINSEL1 |= (1 << 20); //SET UP ADC0.3 ON P0.26

	//INIT GPIO PIN FOR BUTTON
	LPC_GPIO0->FIODIR &= ~(1 << 0); //SET P0.0 AS INPUT FOR BUTTON

	//INIT EXTERNAL INTERRUPUT FOR BUTTON PRESS RISING EDGE
	eint3_enable_port2(6, eint_rising_edge, buttonPressInterrupt);
	vSemaphoreCreateBinary(buttonSignal);
	xSemaphoreTake(buttonSignal, 0);

	bool screenQueueShared = false;
	screenQueue = xQueueCreate(20, sizeof(OnScreenData));
	if (screenQueue)
	{
	    screenQueueShared = addSharedObject(shared_screenQueue, screenQueue);
	}

	return screenQueue && screenQueueShared;
}



bool RemoteControl::run(void *param)
{
	OnScreenData data;
	while(xSemaphoreTake(buttonSignal, portMAX_DELAY)) //WAIT FOR BUTTON PRESS TO GIVE BINARY SEMAPHORE
	{
		while(LPC_GPIO2->FIOPIN & (1 << 6)) // WHILE BUTTON IS PRESSED
		{
			data.powerLevel = calcPower(adc0_get_reading(3));
			xQueueSend(screenQueue, &data, 0);
			sendPowerLevel();
		}
		data.powerLevel = 0;
		xQueueSend(screenQueue, &data, 0);
		sendPowerLevel();
	}
    return true;
}

float RemoteControl::calcPower(int input)
{
    int power;
    power = input/MAX_READ * 100;
    return power;
}

void RemoteControl::sendPowerLevel(void)
{
	//TODO
	u0_dbg_printf("Sending power level of %i \n", powerLevel);
	wireless_send(MESH_BROADCAST_ADDR, mesh_pkt_nack, (int*)&powerLevel, 1, 0);
}
