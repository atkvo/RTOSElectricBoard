#include "ElectricBoardReceiver.hpp"
#include "shared_handles.h"
#include <stdio.h>
#include "wireless.h"
#include "printf_lib.h"
#include "io.hpp"
#include "OnScreenData.hpp"

ElectricBoardReceiver::ElectricBoardReceiver(uint8_t priority) :
    scheduler_task("rcvbrd", 3000, priority, NULL)
{
}

ElectricBoardReceiver::~ElectricBoardReceiver()
{
}

bool ElectricBoardReceiver::init(void)
{
	LPC_GPIO0->FIODIR &= ~(1 << 1); //SET P0.1 AS INPUT FOR BUTTON
    return true;
}

bool ElectricBoardReceiver::run(void *param)
{
	mesh_packet_t packet;
    QueueHandle_t driveQueue = getSharedObject(shared_ElectricBoardQueue);
    if (wireless_get_rx_pkt(&packet, portMAX_DELAY))
    {
    	//u0_dbg_printf("Wireless packet received. The power level read is %i \n", (int) packet.data[0]);
        if (driveQueue /*&& !(LPC_GPIO0->FIOPIN & (1 << 1))*/) //if driveQueue exists and there is something on pressure sensor
        {
            //Parse packet.data[] for command here and send it off to the driveQueue
            int driveLevel = (int) packet.data[0];
            xQueueSend(driveQueue, &driveLevel, 0);     // Don't wait on queue full
        }
    }
    return true;
}
