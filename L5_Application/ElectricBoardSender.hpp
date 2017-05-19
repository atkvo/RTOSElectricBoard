/*
 * ElectricBoardSender.hpp
 *
 *  Created on: May 18, 2017
 *      Author: EEz
 */

#ifndef L5_APPLICATION_ELECTRICBOARDSENDER_HPP_
#define L5_APPLICATION_ELECTRICBOARDSENDER_HPP_

#include "scheduler_task.hpp"
#include "FreeRTOS.h"

class ElectricBoardSender: public scheduler_task {
public:
    ElectricBoardSender(uint8_t priority);
    virtual ~ElectricBoardSender();
    virtual bool init(void);
    virtual bool run(void *param);
};



#endif /* L5_APPLICATION_ELECTRICBOARDSENDER_HPP_ */
