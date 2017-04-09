#include "ElectricBoardReceiver.hpp"
#include "shared_handles.h"
#include <stdio.h>
#include "wireless.h"

ElectricBoardReceiver::ElectricBoardReceiver(uint8_t priority) :
    scheduler_task("rcvbrd", 1024, priority, NULL)
{
}

ElectricBoardReceiver::~ElectricBoardReceiver()
{
}

bool ElectricBoardReceiver::init(void)
{
    return true;
}

bool ElectricBoardReceiver::run(void *param)
{
    mesh_packet_t packet;
    QueueHandle_t driveQueue = getSharedObject(shared_ElectricBoardQueue);
    if (wireless_get_rx_pkt(&packet, portMAX_DELAY))
    {
        if (driveQueue)
        {
            // Parse packet.data[] for command here and send it off to the driveQueue
            // float driveLevel = [parsed value]
            // xQueueSend(driveQueue, &driveLevel, 0);     // Don't wait on queue full
        }
    }

    return true;
}