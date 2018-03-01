#ifndef _W25Q64FV_ENUM_
#define _W25Q64FV_ENUM_

namespace W25{

    enum W25Q64FV_Command{
        WRITE_EN            = 0x06,     // Necessary before every other command that changes memory or non volatile status register bits
        VOL_SR_WRITE_EN     = 0x50,     // Necessary to change volatile status registers bits and non volatile bits in a volatile manner
        WRITE_DISABLE       = 0x04,     // Undoes a previous WRITE_EN
        READ_STATUS_1       = 0x05,     // Reads the status register 1
        READ_STATUS_2       = 0x06,     // Reads the status register 2
        WRITE_STATUS        = 0x01,     // Writes status register 1 and 2
        WRITE_PAGE          = 0x02,     // Writes data in bytes, up to 256 bytes
        SECTOR_ERASE        = 0x20,     // Erases data in 4 KB blocks
        WORD_ERASE          = 0x52,     // Erases data in 32 KB blocks
        LONG_WORD_ERASE     = 0xD8,     // Erases data in 64 KB blocks
        CHIP_ERASE          = 0xC7,     // Erases the entire memory
        WRITE_SUSPEND       = 0x75,     // Pauses any current operation that changes memory
        WRITE_RESUME        = 0x7A,     // Resumes a suspended operation that changes memory
        STANDBY             = 0xB9,     // Puts the storage device in standby
        READ                = 0x03,     // Reads any amount of data in bytes
        FAST_READ           = 0x0B,
        WAKE_UP             = 0xAB,     // Wakes up the storage from standby, only command that doesn't get ignored
        DEVICE_ID           = 0x90,
        JEDEC_ID            = 0x9F,
        UNIQUE_ID           = 0x4B,
        READ_SFDP           = 0x5A,     // Reads 256 bytes of information about the device's commands and configs
        ERASE_SEC_REGS      = 0x44,     // Erases the three 256 bytes security registers
        PROGRAM_SEC_REGS    = 0x42,     // Program the three 256 bytes security registers
        READ_SEC_REGS       = 0x48,     // Read the three 256 bytes security registers
        ENABLE_RESET        = 0x66,     // Necessary to reset the storage device
        RESET               = 0x99      // Resets the storage device
    };
}

#endif