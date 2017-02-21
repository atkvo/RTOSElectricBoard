/*
 * sspdriver.hpp
 *
 *  Created on: Feb 18, 2017
 *      Author: atkvo
 */

#ifndef L5_APPLICATION_SSPDRIVER_HPP_
#define L5_APPLICATION_SSPDRIVER_HPP_

#include "LPC17xx.h"

#define PCSPI             8
#define PCSSP1            10
#define PCSSP0            21

#define SSP1_PCLK0_BIT    20
#define SSP0_PCLK1_BIT    10

#define PINSEL0_SCK1      14
#define PINSEL0_MISO1     16
#define PINSEL0_MOSI1     18
#define SSP1_SCK_SEL      0b10
#define SSP1_MOSI_SEL     0b10
#define SSP1_MISO_SEL     0b10

#define PINSEL1_MOSI0     4
#define PINSEL1_MISO0     2
#define SSP0_MOSI_SEL_VAL 0b10
#define SSP0_MISO_SEL_VAL 0b10

#define SSP1_CS_PIN       6
#define SSP1_CS_SEL_BIT   12

#define SSP_LOOPBACK_BIT  0

typedef enum {
    SSP0 = 0,
    SSP1 = 1
} e_ssp;

typedef enum {
    CLK_BY_4 = 0b00,
    CLK_BY_1 = 0b01,
    CLK_BY_2 = 0b10,
    CLK_BY_8 = 0b11
} e_pclk_div;

typedef enum {
    SSP_DATA_SIZE_8  = 0b0111,
    SSP_DATA_SIZE_16 = 0b1111
} e_ssp_data_size;

class SSPDriver {
private:
    e_ssp mSSPIndex;
    LPC_SSP_TypeDef *mSSP;
    void setupControlRegisters(LPC_SSP_TypeDef *SSP);
    bool setupSSP(e_pclk_div divider);
    int8_t getDividerFromPCLK();
public:
    SSPDriver(e_ssp ssp);
    void DeassertCS();
    void AssertCS();
    uint8_t TransferByte(uint8_t dataOut);
    void SetClock(uint32_t frequency);
    bool SetLoopback(bool state);
};


#endif /* L5_APPLICATION_SSPDRIVER_HPP_ */
