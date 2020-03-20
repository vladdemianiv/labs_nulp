#include "flash.h"

extern SPI_HandleTypeDef hspi1;

void read_data(uint32_t address,uint8_t* data,uint16_t size){
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_RESET);
	uint8_t _data[4];
	_data[0]=0x03;
	_data[1]=(address>>16)&0xFF;
	_data[2]=(address>>8)&0xFF;
	_data[3]=(address)&0xFF;
	HAL_SPI_Transmit(&hspi1,_data,4,0xFFFF);
	HAL_SPI_Receive(&hspi1,data,size,0xFFFF);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_SET);
}

uint8_t read_status_register(){
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_RESET);
	uint8_t _data=0x05;
	uint8_t status;
	HAL_SPI_Transmit(&hspi1,&_data,1,0xFFFF);
	HAL_SPI_Receive(&hspi1,&status,1,0xFFFF);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_SET);
	return status;
}

void write_status_register(uint8_t data){
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_RESET);
	uint8_t _data=0x50;
	HAL_SPI_Transmit(&hspi1,&_data,1,0xFFFF);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_RESET);
	_data = 0x01;
	HAL_SPI_Transmit(&hspi1,&_data,1,0xFFFF);
	HAL_SPI_Transmit(&hspi1,&data,1,0xFFFF);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_SET);
}

void write_array_with_aai(uint32_t address,uint8_t* data,uint16_t size){
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_RESET);
	data[0] = 0x06;
	HAL_SPI_Transmit(&hspi1,data,1,0xFFFF);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_RESET);
	uint8_t _data[] = { 0xAD, (address>>16)&0xFF, (address>>8)&0xFF, (address)&0xFF, data[0], data[1] };
	HAL_SPI_Transmit(&hspi1,data,6,0xFFFF);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_SET);
	
	for(uint8_t i=0;i<size/2;i++){
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_RESET);
			memcpy(data+1,(uint8_t*)(data[i*2],data[i*2+1]),2);
			HAL_SPI_Transmit(&hspi1,data,3,0xFFFF);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_SET);
			while(read_status_register());
	}
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_RESET);
	data[0] = 0x04;
	HAL_SPI_Transmit(&hspi1,data,1,0xFFFF);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_SET);
}


void* memcpy(void* dest, const void* src, size_t bytes)
{
    size_t i;
    char* cdest = (char*)dest;
    const char* csrc = (char*)src;
    for (i = 0; i < bytes; ++i)
        *cdest++ = *csrc++;
    
    return dest;
}
void full_clear(){
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_RESET);
	uint8_t _data=0x06;
	HAL_SPI_Transmit(&hspi1,&_data,1,0xFFFF);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_SET);
}
