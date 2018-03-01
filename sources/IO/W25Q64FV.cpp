#include "W25Q64FV_enum.h"
#include "W25Q64FV.h"
#include "mbed.h"

using namespace W25;

W25Q64FV::W25Q64FV() : mSPI(PTD6, PTD7, PTD5, NC), chipSelect(PTD4){
    mSPI.frequency(8000000);
    mSPI.format(8, 0);
}

int W25Q64FV::init(){
    while(isBusy());
    return 0;
}

int W25Q64FV::deinit(){
    return 0;
}

int W25Q64FV::read(void *buffer, bd_addr_t addr, bd_size_t size){
    char data[4];
    data[0] = READ;
    for(uint8_t i=0;i<3;i++){
        data[i+1] = (addr>>(2-i))&0xF;
    } 
    while(isBusy()) ;
    chipSelect = 0;
    int result = mSPI.write(data, 4, (char*) buffer, size);
    chipSelect = 1;
    return result;
}

int W25Q64FV::program(const void *buffer, bd_addr_t addr, bd_size_t size){
    char data[4];
    char dummy;
    int result1, result2;
    data[0] = WRITE_PAGE;
    for(uint8_t i=0;i<3;i++){
        data[i+1] = (addr>>(2-i))&0xF;
    }
    while(isBusy()) ;
    chipSelect = 0;
    mSPI.write(WRITE_EN);
    chipSelect = 1;
    chipSelect = 0;
    result1 = mSPI.write(data, 4, &dummy, 0);
    result2 = mSPI.write((char*) buffer, size, &dummy, 0);
    chipSelect = 1;
    return (result1<result2) ? result1 : result2;
}

int W25Q64FV::erase(bd_addr_t addr, bd_size_t size){
    int result, tempResult;
    result = 0;
    char data[4];
    char dummy;
    uint factor;
    while(size>0){   
        if(size > 64000){
            data[0] = LONG_WORD_ERASE;
            factor = 64000;
        }
        else if(size>32000){
            data[0] = WORD_ERASE;
            factor = 32000;
        }
        else{
            data[0] = SECTOR_ERASE;
            factor = 4000;
        }
        for(uint8_t i=0;i<3;i++){
            data[i+1] = (addr>>(2-i))&0xF;
        }
        while(isBusy()) ;
        chipSelect = 0;
        mSPI.write(WRITE_EN);
        chipSelect = 1;
        chipSelect = 0;
        tempResult = mSPI.write(data, 4, &dummy, 0);
        chipSelect = 1;
        if(tempResult < result){result = tempResult;}
        addr+=factor;
        size-=factor;
    }
    return result;
}

bd_size_t W25Q64FV::get_read_size(){
    return 1;
}

bd_size_t W25Q64FV::get_program_size(){
    return 1;
}

bd_size_t W25Q64FV::get_erase_size(){
    return 4000;
}

bd_size_t W25Q64FV::size(){
    return 8388608;
}

bool W25Q64FV::isBusy(){
    char result, data;
    data = READ_STATUS_1;
    chipSelect = 0;
    mSPI.write(&data, 1, &result, 1);
    chipSelect = 1;
    return data&1;
}

