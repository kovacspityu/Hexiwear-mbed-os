#ifndef _W25Q64FV_
#define _W25Q64FV_

#include "W25Q64FV_enum.h"
#include "mbed.h"
#include "BlockDevice.h"

using namespace W25;

class W25Q64FV : public BlockDevice{
    public:
        W25Q64FV();
        ~W25Q64FV();

        virtual int init();
        virtual int deinit();
        virtual int read(void *buffer, bd_addr_t addr, bd_size_t size);
        virtual int program(const void *buffer, bd_addr_t addr, bd_size_t size);
        virtual int erase(bd_addr_t addr, bd_size_t size);
        virtual bd_size_t get_read_size();
        virtual bd_size_t get_program_size();
        virtual bd_size_t get_erase_size();
        virtual bd_size_t size();


    private:

    SPI mSPI;
    DigitalOut chipSelect;
    bool isBusy();


};











#endif