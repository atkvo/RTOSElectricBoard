#ifndef _L5_APPLICATION_FlashMemoryDriver_H_
#define _L5_APPLICATION_FlashMemoryDriver_H_

#include "sspdriver.hpp"

typedef enum {
    READ_BUF1_LF    = 0xD1,
    READ_BUF2_LF    = 0xD3,
    READ_BUF1_HF    = 0xD4,
    READ_BUF2_HF    = 0xD6,
    READ_STATUS_REG = 0xD7,

    WRITE_BUF1      = 0x84,
    WRITE_BUF2      = 0x87
} eFLASH_CMDS;

typedef enum {
    Standard,
    Power2
} ePageSize;

typedef struct {
    // Byte 1
    bool Ready;             // Bit 7
    bool CompareResult;     // Bit 6
    uint8_t DensityCode;    // Bit 5:2
    bool ProtectionEnabled; // Bit 1
    ePageSize PageSize;     // Bit 0

    // Byte 2
    // bool Ready;          // Bit 7
    // reserved             // Bit 6
    bool EraseProgramErr;   // Bit 5
    // reserved             // Bit 4
    bool SectorLockdown;    // Bit 3
    bool ProgramSuspend2;   // Bit 2
    bool ProgramSuspend1;   // Bit 1
    bool EraseSuspend;      // Bit 0
} sFlashMemoryStatus;

class FlashMemoryDriver {
private:
    SSPDriver *mSSP;
    sFlashMemoryStatus state;
    void parseStatusBits(uint8_t b1, uint8_t b2);
    void initSSP();
public:
    FlashMemoryDriver(e_ssp sspNumber);
    void UpdateStatusRegisters(bool print);
    void PrintStatusRegisters();
    ~FlashMemoryDriver();
};

#endif