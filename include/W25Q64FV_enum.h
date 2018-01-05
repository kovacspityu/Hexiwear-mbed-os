#ifndef _W25Q64FV_ENUM_
#define _W25Q64FV_ENUM_



enum W25Q64FV_Command{
    W25_WRITE_EN            = 0x06,     // Necessary before every other command that changes memory or non volatile status register bits
    W25_VOL_SR_WRITE_EN     = 0x50,     // Necessary to change volatile status registers bits and non volatile bits in a volatile manner
    W25_WRITE_DISABLE       = 0x04,     // Undoes a previous W25_WRITE_EN
    W25_READ_STATUS_1       = 0x05,     // Reads the status register 1
    W25_READ_STATUS_2       = 0x06,     // Reads the status register 2
    W25_WRITE_STATUS        = 0x01,     // Writes status register 1 and 2
    W25_WRITE_PAGE          = 0x02,     // Writes data in bytes, up to 256 bytes
    W25_SECTOR_ERASE        = 0x20,     // Erases data in 4 KB blocks
    W25_WORD_ERASE          = 0x52,     // Erases data in 32 KB blocks
    W25_LONG_WORD_ERASE     = 0xD8,     // Erases data in 64 KB blocks
    W25_CHIP_ERASE          = 0xC7,     // Erases the entire memory
    W25_WRITE_SUSPEND       = 0x75,     // Pauses any current operation that changes memory
    W25_WRITE_RESUME        = 0x7A,     // Resumes a suspended operation that changes memory
    W25_STANDBY             = 0xB9,     // Puts the storage device in standby
    W25_READ                = 0x03,     // Reads any amount of data in bytes
    W25_FAST_READ           = 0x0B,
    W25_WAKE_UP             = 0xAB,     // Wakes up the storage from standby, only command that doesn't get ignored
    W25_DEVICE_ID           = 0x90,
    W25_JEDEC_ID            = 0x9F,
    W25_UNIQUE_ID           = 0x4B,
    W25_READ_SFDP           = 0x5A,     // Reads 256 bytes of information about the device's commands and configs
    W25_ERASE_SEC_REGS      = 0x44,     // Erases the three 256 bytes security registers
    W25_PROGRAM_SEC_REGS    = 0x42,     // Program the three 256 bytes security registers
    W25_READ_SEC_REGS       = 0x48,     // Read the three 256 bytes security registers
    W25_ENABLE_RESET        = 0x66,     // Necessary to reset the storage device
    W25_RESET               = 0x99      // Resets the storage device
};














#endif