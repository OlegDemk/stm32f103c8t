/*
 * Flash_W25Q.c
 *
 *  Created on: Sep 26, 2020
 *      Author: odemki
 */
#include "stdio.h"
#include "stm32f1xx_hal.h"


extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef hspi2;

// ----------------------------------------------------------------------------
void test_flash_W25Q(void)
{
//	uint8_t init=0;
//	init = W25qxx_Init();
//
//	uint32_t id=0;
//	id = W25qxx_ReadID();
//
//	uint8_t test_variable = 1;
//	uint8_t uint8_t_test_buffer = 0;
//
//	uint16_t i = 0;
//
//	//W25qxx_EraseChip();
//	W25qxx_EraseSector();
//
//	W25qxx_WriteByte(test_variable, 0);					// Write one byte
//	HAL_Delay(20);
//	W25qxx_ReadByte(&uint8_t_test_buffer, 0);           // Read one byte





//
//	uint8_t size=0;
//	char str[100]={0};
//	uint8_t size_mas=sizeof(str);
//	sprintf(str,"Data: %d \r\n ", uint8_t_test_buffer );
//	size=sizeof(str);
//	HAL_UART_Transmit(&huart1 , (uint8_t *)str, size, 0xFFFF);

///////////////////////////////////////////////////////////////////////////////
//	for(i=0; i<10; i++)
//	{
//		W25qxx_WriteByte(test_variable, i);					// Write one byte
//		HAL_Delay(1);
//	}
//
//	for(i=0; i<10; i++)
//	{
//		W25qxx_ReadByte(&uint8_t_test_buffer, i);           // Read one byte
//
//		uint8_t size=0;
//		char str[100]={0};
//		uint8_t size_mas=sizeof(str);
//		sprintf(str,"Data: i=%d, %d \r\n ", i, uint8_t_test_buffer );
//		size=sizeof(str);
//		HAL_UART_Transmit(&huart1 , (uint8_t *)str, size, 0xFFFF);
//	}



	int g=0;

}
// ----------------------------------------------------------------------------
//uint8_t size=0;
//	char str3[100]={0};
//	uint8_t size_mas=sizeof(str3);
//	uint8_t i=0;
//	sprintf(str3,"I2C_1 SEARCH DEVISES... \r\n");      										// convert   in  str
//	size=sizeof(str3);
//	HAL_UART_Transmit(&huart1 , (uint8_t *)str3, size, 0xFFFF);
