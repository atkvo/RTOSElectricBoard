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

void FlashMemoryDriver::UpdateAllRegisters(bool print=false) {
    UpdateStatusRegisters(print);
    UpdateIDRegisters(print);
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

void FlashMemoryDriver::UpdateIDRegisters(bool print=false) {
    const int ID_BYTES_TO_READ = 3;
    uint8_t idreg[ID_BYTES_TO_READ];
    mSSP->AssertCS();
    mSSP->TransferByte(READ_ID);
    for (int i = 0; i < ID_BYTES_TO_READ; i++) {
        idreg[i] = mSSP->TransferByte(0xFF);
    }

    mSSP->DeassertCS();
    parseIDBits(idreg[0], idreg[1], idreg[2]);

    if (print) {
        PrintID();
    }
}

void FlashMemoryDriver::parseStatusBits(uint8_t b1, uint8_t b2) {
    // Byte 1
    mState.Ready = (b1 & (1 << READY_BIT));
    mState.CompareResult = (b1 & (1 << CMP_BIT));
    mState.DensityCode = (0b1011 & (b1 >> DENSITY_BIT));
    mState.ProtectionEnabled = (b1 & (1 << PROTECT_BIT));
    mState.PageSize = (b1 & (1 << PAGE_SIZE_BIT) ? Power2 : Standard);

    // Byte 2
    mState.EraseProgramErr = (b2 & (1 << EPE));
    mState.SectorLockdown  = (b2 & (1 << SLE));
    mState.ProgramSuspend2 = (b2 & (1 << PS2));
    mState.ProgramSuspend1 = (b2 & (1 << PS1));
    mState.EraseSuspend    = (b2 & (1 << ES ));
}

void FlashMemoryDriver::parseIDBits(uint8_t b1, uint8_t b2, uint8_t b3) {
    mID.ManufacturerID = b1;
    mID.DeviceID1 = b2;
    mID.DeviceID2 = b3;
}

void FlashMemoryDriver::PrintStatusRegisters() {
    printf("\n=========================\n");
    printf("   FLASH MEMORY STATUS   \n");
    printf("-------------------------\n");
    printf("   Ready:           %d\n", mState.Ready ? 1 : 0);
    printf("   Compare Result:  %d\n", mState.CompareResult ? 1 : 0);
    printf("   DensityCode:     0x%x\n", mState.DensityCode);
    printf("   Erase/Prgrm Err: %d\n", mState.EraseProgramErr ? 1 : 0);
    printf("   Sector Locked:   %d\n", mState.SectorLockdown ? 1 : 0);
    printf("   Erase Suspend:   %d\n", mState.EraseSuspend ? 1 : 0);
    printf("=========================\n\n");
}

void FlashMemoryDriver::PrintID() {
    printf("\n=========================\n");
    printf("   FLASH ID              \n");
    printf("-------------------------\n");
    printf("   Manufacturer:    0x%x\n", mID.ManufacturerID);
    printf("   Device Byte 1:   0x%x\n", mID.DeviceID1);
    printf("   Device Byte 2:   0x%x\n", mID.DeviceID2);
    printf("=========================\n\n");
}