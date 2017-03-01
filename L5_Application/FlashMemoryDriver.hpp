#ifndef _L5_APPLICATION_FlashMemoryDriver_H_
#define _L5_APPLICATION_FlashMemoryDriver_H_

#include "sspdriver.hpp"

typedef enum {
    READ_BUF1_LF    = 0xD1,
    READ_BUF2_LF    = 0xD3,
    READ_BUF1_HF    = 0xD4,
    READ_BUF2_HF    = 0xD6,
    READ_STATUS_REG = 0xD7,
    READ_ID         = 0x9F,

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


typedef struct {
    uint8_t ManufacturerID;     // 0x1F
    uint8_t DeviceID1;          // 0x26
    uint8_t DeviceID2;          // 0x00
} sFlashID;

class FlashMemoryDriver {
private:
    SSPDriver *mSSP;
    sFlashMemoryStatus mState;
    sFlashID mID;
    void parseStatusBits(uint8_t b1, uint8_t b2);
    void parseIDBits(uint8_t b1, uint8_t b2, uint8_t b3);
    void initSSP();
public:
    FlashMemoryDriver(e_ssp sspNumber);
    void UpdateStatusRegisters(bool print);
    void UpdateIDRegisters(bool print);
    void UpdateAllRegisters(bool print);
    void PrintStatusRegisters();
    void PrintID();
    ~FlashMemoryDriver();
};

#endif