#include "ElectricBoardSender.hpp"
#include "shared_handles.h"
#include <stdio.h>
#include "wireless.h"
#include "printf_lib.h"
#include "io.hpp"
#include "OnScreenData.hpp"

ElectricBoardSender::ElectricBoardSender(uint8_t priority) :
    scheduler_task("txbrd", 3000, priority, NULL)
{
}

ElectricBoardSender::~ElectricBoardSender()
{
}

bool ElectricBoardSender::init(void)
{
	LPC_GPIO0->FIODIR &= ~(1 << 1); //SET P0.1 AS INPUT FOR FORCE SENSOR
    return true;
}

bool ElectricBoardSender::run(void *param)
{
    int pressure = !(LPC_GPIO0->FIOPIN & (1 << 1));
    int accelerometerData = AS.getX();
    uint32_t sendData;
    sendData = (accelerometerData << 8);
    sendData = sendData + !pressure;
    wireless_send(106, mesh_pkt_nack, (uint32_t*)&sendData, 4, 0);
    //u0_dbg_printf("Wireless packet sent. Accelerometer = %i. eBreak = %i \n", accelerometerData, !pressure);
    vTaskDelay(500);
    return true;
}
