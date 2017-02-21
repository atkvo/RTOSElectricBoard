/*
 * FlashMemoryDriver.cpp
 *
 *  Created on: Feb 19, 2017
 *      Author: atkvo
 */

#include <FlashMemoryDriver.hpp>
#include <stdio.h>          // printf

typedef enum {
    READY_BIT       = 7,
    CMP_BIT         = 6,
    DENSITY_BIT     = 2,    // 5:2
    PROTECT_BIT     = 1,
    PAGE_SIZE_BIT   = 0
} eStatusBits1;

typedef enum {
    EPE = 5,
    SLE = 3,
    PS2 = 2,
    PS1 = 1,
    ES  = 0
} eStatusBits2;

FlashMemoryDriver::FlashMemoryDriver(e_ssp sspNumber)
{
    initSSP();
}

FlashMemoryDriver::~FlashMemoryDriver() {
    delete mSSP;
}

void FlashMemoryDriver::initSSP() {
    mSSP = new SSPDriver(SSP1);
}

void FlashMemoryDriver::UpdateStatusRegisters(bool print=false) {
    uint8_t statusreg[2];
    mSSP->AssertCS();
    mSSP->TransferByte(READ_STATUS_REG);

    for (int i = 0; i < 2; i++) {
        statusreg[i] = mSSP->TransferByte(0xFF);    // filler
        // printf("Status reg: %x\n", statusreg[i]);
    }

    mSSP->DeassertCS();
    parseStatusBits(statusreg[0], statusreg[1]);

    if (print) {
        PrintStatusRegisters();
    }
}

void FlashMemoryDriver::parseStatusBits(uint8_t b1, uint8_t b2) {
    // Byte 1
    state.Ready = (b1 & (1 << READY_BIT));
    state.CompareResult = (b1 & (1 << CMP_BIT));
    state.DensityCode = (0b1011 & (b1 >> DENSITY_BIT));
    state.ProtectionEnabled = (b1 & (1 << PROTECT_BIT));
    state.PageSize = (b1 & (1 << PAGE_SIZE_BIT) ? Power2 : Standard);

    // Byte 2
    state.EraseProgramErr = (b2 & (1 << EPE));
    state.SectorLockdown  = (b2 & (1 << SLE));
    state.ProgramSuspend2 = (b2 & (1 << PS2));
    state.ProgramSuspend1 = (b2 & (1 << PS1));
    state.EraseSuspend    = (b2 & (1 << ES ));
}

void FlashMemoryDriver::PrintStatusRegisters() {
    printf("\n=========================\n");
    printf("   FLASH MEMORY STATUS   \n");
    printf("-------------------------\n");
    printf("   Ready:           %d\n", state.Ready ? 1 : 0);
    printf("   Compare Result:  %d\n", state.CompareResult ? 1 : 0);
    printf("   DensityCode:     0x%x\n", state.DensityCode);
    printf("   Erase/Prgrm Err: %d\n", state.EraseProgramErr ? 1 : 0);
    printf("   Sector Locked:   %d\n", state.SectorLockdown ? 1 : 0);
    printf("   Erase Suspend:   %d\n", state.EraseSuspend ? 1 : 0);
    printf("=========================\n\n");
}