/*
 * sspdriver.cpp
 *
 *  Created on: Feb 18, 2017
 *      Author: atkvo
 */

#include "sys_config.h"
#include "sspdriver.hpp"

SSPDriver::SSPDriver(e_ssp ssp) : mSSPIndex(ssp) {
    mSSP = (ssp == SSP0 ? LPC_SSP0 : LPC_SSP1);
    setupSSP(CLK_BY_1);
}

void SSPDriver::AssertCS() {
    if (mSSPIndex == SSP0) {
        ;
    }
    else if (mSSPIndex == SSP1) {
        LPC_GPIO0->FIOCLR = (1 << SSP1_CS_PIN);
    }
};

void SSPDriver::DeassertCS() {
    if (mSSPIndex == SSP0) {
        // nothing
        ;
    }
    else if (mSSPIndex == SSP1) {
        LPC_GPIO0->FIOSET = (1 << SSP1_CS_PIN);
    }
};

uint8_t SSPDriver::TransferByte(uint8_t dataOut) {
    mSSP->DR = dataOut;
    while (mSSP->SR & (1 << 4)) { ; }
    return mSSP->DR;
}

int8_t SSPDriver::getDividerFromPCLK() {
    uint32_t dividerBits = CLK_BY_1;
    switch (mSSPIndex) {
        case SSP0:
            dividerBits = (0x3) & (LPC_SC->PCLKSEL0 >> SSP1_PCLK0_BIT);
            break;
        case SSP1:
            dividerBits = (0x3) & (LPC_SC->PCLKSEL1 >> SSP0_PCLK1_BIT);
            break;
        default:
            return 1;
    }

    switch (dividerBits) {
        case CLK_BY_4:
            return 4;
        case CLK_BY_1:
            return 1;
        case CLK_BY_2:
            return 2;
        case CLK_BY_8:
            return 8;
        default:
            return 1;
    }
}

void SSPDriver::SetClock(uint32_t frequency) {
    unsigned int sysClk = sys_get_cpu_clock();
    uint8_t pclkDivider = getDividerFromPCLK();
    uint8_t divider = 2;    // ranges from 2 to 254

    uint8_t sspBaseClk = sysClk / pclkDivider;

    for (divider = 2; divider <= 254; divider += 2) {
        if ((sspBaseClk/divider) < frequency) {
            break;
        }
    }

    mSSP->CPSR = divider;
}

bool SSPDriver::SetLoopback(bool state) {
    if (mSSP == 0) {
        return false;
    }

    if (state) {
        mSSP->CR1 |= (1 << SSP_LOOPBACK_BIT);
    }
    else {
        mSSP->CR1 &= ~(1 << SSP_LOOPBACK_BIT);
    }

    return true;
}

bool SSPDriver::setupSSP(e_pclk_div clkDivider) {
    switch (mSSPIndex) {
        case SSP0:
            LPC_SC->PCONP |= (1 << PCSSP0);
            LPC_SC->PCLKSEL1 |= (clkDivider << SSP0_PCLK1_BIT);
            LPC_PINCON->PINSEL1 |= (SSP0_MISO_SEL_VAL << PINSEL1_MISO0);
            LPC_PINCON->PINSEL1 |= (SSP0_MOSI_SEL_VAL << PINSEL1_MOSI0);
            setupControlRegisters(LPC_SSP0);
            break;
        case SSP1:
            LPC_SC->PCONP |= (1 << PCSSP1);
            LPC_SC->PCLKSEL0 |= (clkDivider << SSP1_PCLK0_BIT);

            // LPC_PINCON->PINSEL0 |= (0b10 << 12); // SSEL1
            LPC_PINCON->PINSEL0 &= ~(0b00 << SSP1_CS_SEL_BIT); // P0.6 as GPIO 
            LPC_GPIO0->FIODIR |= (1 << SSP1_CS_PIN); // P0.6 as output
            LPC_GPIO0->FIOSET = (1 << SSP1_CS_PIN);
            LPC_PINCON->PINMODE0 |= (2 << SSP1_CS_SEL_BIT); // P0.6 No pullup/pulldown

            LPC_PINCON->PINSEL0 &= ~((3 << PINSEL0_MISO1) | (3 << PINSEL0_MOSI1) | (3  << PINSEL0_SCK1));
            LPC_PINCON->PINSEL0 |= ((SSP1_MISO_SEL << PINSEL0_MISO1) | (SSP1_MOSI_SEL << PINSEL0_MOSI1) | (SSP1_SCK_SEL  << PINSEL0_SCK1));
            setupControlRegisters(LPC_SSP1);
            break;
        default:
            return false;
            break;
    }

    return true;
}

void SSPDriver::setupControlRegisters(LPC_SSP_TypeDef *SSP) {
    const uint8_t DSS = 0;
    SSP->CR0 |= (SSP_DATA_SIZE_8 << DSS);
    SSP->CR1 |= (1 << 1);   // Enable as master
    SetClock(1000000); // 1MHz
}