#include "UART2Task.hpp"
#include "uart2driver.h"
#include "printf_lib.h"
#include <string.h>

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
    static uint32_t txcount = 0;
    static const char txbuf[] = "ANDREW";
    static const uint8_t txlen = 6;
    uart2_tx(txbuf[txcount]);

    char c = uart2_rx_char();
    if (c)
    {
        u0_dbg_printf("%c", c);
    }

    txcount++;
    if (txcount >= txlen)
    {
        txcount = 0;
    }

    return true;
}