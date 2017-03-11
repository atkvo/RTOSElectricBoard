#ifndef _UART_2_TASK_HPP
#define _UART_2_TASK_HPP

#include <scheduler_task.hpp>
class UART2Task : public scheduler_task {
private:
public:
    UART2Task(uint8_t priority);
    virtual ~UART2Task();
    bool init(void);
    bool run(void *param);
};



#endif