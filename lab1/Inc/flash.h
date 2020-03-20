#ifndef FLASH_H
#define FLASH_H


#include "stm32f4xx_hal.h"         


void read_data(uint32_t address,uint8_t* data,uint16_t size);
uint8_t read_status_register(void);
void write_status_register(uint8_t data);
void write_array_with_aai(uint32_t address,uint8_t* data,uint16_t size);
void full_clear(void);
void* memcpy(void* dest, const void* src, size_t bytes);
#endif
