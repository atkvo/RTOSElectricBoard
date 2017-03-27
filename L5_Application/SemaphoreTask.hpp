#ifndef _SEMAPHORE_TASK_HPP_
#define _SEMAPHORE_TASK_HPP_

#include "scheduler_task.hpp"

class SemaphoreTask : public scheduler_task {
private:
public:
    SemaphoreTask(uint8_t priority);
    virtual ~SemaphoreTask();
    virtual bool init(void);
    virtual bool run(void *param);
};

#endif