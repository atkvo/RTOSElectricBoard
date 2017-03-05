#include "UART2Task.hpp"
#include "uart2driver.h"
#include "printf_lib.h"

UART2Task::UART2Task(uint8_t priority) : scheduler_task("uart 2 polling", 1024, priority)
{
}

UART2Task::~UART2Task()
{

}

bool UART2Task::init(void)
{
    uart2_init();
    // setRunDuration(500);
    return true;
}

bool UART2Task::run(void *param)
{
    const char txbuf[] = "Hello";
    const uint32_t MAX_LEN = 10;
    char buf[MAX_LEN] = { 0 };
    for (int i = 0; i < 5; i++)
    {
        uart2_tx(txbuf[i]);
    }

    uint32_t bytesread = uart2_rx(buf, MAX_LEN);
    if (bytesread > 0)
    {
        u0_dbg_printf("Bytes read: %d; Message: %s\n", bytesread, buf);
    }
    return true;
}