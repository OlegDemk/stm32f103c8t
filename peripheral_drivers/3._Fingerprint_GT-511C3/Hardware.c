#include "ht32f125x.h"
#include "Hardware.h"
#include "Protocol.h"
#include "main.h"

 vu32 gwSysTickCnt;
 /*********************************************************************************************************//**
  * @brief  This function is used to configure NVIC.
  * @param  None
  * @retval None
  ***********************************************************************************************************/
void NVIC_Configuration(void)
{
#ifdef VECTOR_AT_RAM
  //NVIC_SetVectorTable(NVIC_VECTTABLE_RAM, 0x0);       /* Set the Vector Table base location at 0x20000000   */
#elif VECTOR_AT_FLASH
  //NVIC_SetVectorTable(NVIC_VECTTABLE_FLASH, 0x0);     /* Set the Vector Table base location at 0x00000000   */
#endif

	SYSTICK_ClockSourceConfig(SYSTICK_SRC_STCLK);  /* Default : 9MHz ON CHIP */
  SYSTICK_SetReloadValue(9000);                  /* (9k/9MHz) = 1ms ON CHIP */
  SYSTICK_IntConfig(ENABLE);
}


/*********************************************************************************************************//**
  * @brief  Configures the different system clocks..
  * @param  None
  * @brief  None
  ***********************************************************************************************************/
void CKCU_Configuration(void)
{

	ErrStatus HSEReadyStatus;
	
	/* Wait till HSE is ready                                                                                 */
	HSEReadyStatus = CKCU_WaitHSEReady();
	
	if (HSEReadyStatus == SUCCESS)
	{
		/*------------------------------------------------------------------------------------------------------*/
		/* Peripheral function clock config                                                                     */
		/*------------------------------------------------------------------------------------------------------*/
		CKCU_SetUSARTPrescaler(CKCU_URPRE_DIV1); 
		
		CKCU_APBPerip0ClockConfig(CKCU_APBEN0_AFIO | CKCU_APBEN0_USART | CKCU_APBEN0_PA | CKCU_APBEN0_PB , ENABLE);

	//	CKCU_APBPerip1ClockConfig(CKCU_APBEN1_RTC, ENABLE);
	}
}

/*********************************************************************************************************//**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  ***********************************************************************************************************/
void GPIO_Configuration(void)
{
  //configuration uart
  AFIO_GPAConfig(AFIO_PIN_8 | AFIO_PIN_9, AFIO_MODE_2);  
  GPIO_PullResistorConfig(GPIOA,GPIO_PIN_8 | AFIO_PIN_9,GPIO_PR_UP);

  //configuration display data
  AFIO_GPAConfig(AFIO_PIN_0	| AFIO_PIN_1 | AFIO_PIN_2 | AFIO_PIN_3 | AFIO_PIN_4 | AFIO_PIN_5 | AFIO_PIN_6 | AFIO_PIN_7,	AFIO_MODE_DEFAULT);
	//configuration LCD_RS
   AFIO_GPBConfig(AFIO_PIN_7 , AFIO_MODE_DEFAULT);
   //configuration LCD_RW
   AFIO_GPBConfig(AFIO_PIN_6 , AFIO_MODE_DEFAULT);
   //configuration LCD_E
   AFIO_GPBConfig(AFIO_PIN_5 , AFIO_MODE_DEFAULT);
   
   //configuration ERASE
   AFIO_GPBConfig(AFIO_PIN_10 , AFIO_MODE_1);
   
   //configuration LOGIN
   AFIO_GPBConfig(AFIO_PIN_11 , AFIO_MODE_DEFAULT);	

   //configuration DEL
   AFIO_GPAConfig(AFIO_PIN_10 , AFIO_MODE_DEFAULT);	

  //initial display data pin
  GPIO_DirectionConfig(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_DIR_OUT);
  GPIO_WriteOutData(GPIOA, 0x00);
  //initial LCD_RS pin
  GPIO_DirectionConfig(GPIOB, GPIO_PIN_7, GPIO_DIR_OUT);
  GPIO_ClearOutBits(GPIOB, GPIO_PIN_7);

  //initial LCD_RW pin
  GPIO_DirectionConfig(GPIOB, GPIO_PIN_6, GPIO_DIR_OUT);
  GPIO_ClearOutBits(GPIOB, GPIO_PIN_6);

  //initial LCD_E pin
  GPIO_DirectionConfig(GPIOB, GPIO_PIN_5, GPIO_DIR_OUT);
  GPIO_ClearOutBits(GPIOB, GPIO_PIN_5);

  //initial Button ERASE
  GPIO_DirectionConfig(GPIOB, ERASE, GPIO_DIR_IN);
  GPIO_InputConfig(GPIOB, ERASE, ENABLE);
  GPIO_PullResistorConfig(GPIOB, ERASE, GPIO_PR_DOWN);

  //initial Button LOGIN
  GPIO_DirectionConfig(GPIOB, LOGIN, GPIO_DIR_IN);
  GPIO_InputConfig(GPIOB, LOGIN, ENABLE);

  //initial Button DEL
  GPIO_DirectionConfig(GPIOA, DEL, GPIO_DIR_IN);
  GPIO_InputConfig(GPIOA, DEL, ENABLE);

  //initial LED ENROLL
  AFIO_GPBConfig(AFIO_PIN_4 , AFIO_MODE_DEFAULT);
  GPIO_DirectionConfig(GPIOB, LED_ENROLL, GPIO_DIR_OUT);
  GPIO_SetOutBits(GPIOB,LED_ENROLL);

  //initial LED LOGIN
  AFIO_GPBConfig(AFIO_PIN_3 , AFIO_MODE_DEFAULT);
  GPIO_DirectionConfig(GPIOB, LED_LOGIN, GPIO_DIR_OUT);
  GPIO_SetOutBits(GPIOB,LED_LOGIN);

  //initial LED DEL
  AFIO_GPBConfig(AFIO_PIN_2 , AFIO_MODE_DEFAULT);
  GPIO_DirectionConfig(GPIOB, LED_DEL, GPIO_DIR_OUT);
  GPIO_SetOutBits(GPIOB,LED_DEL);

  //initial LED PASS
  AFIO_GPBConfig(AFIO_PIN_15 , AFIO_MODE_DEFAULT);
  GPIO_DirectionConfig(GPIOB, LED_PASS, GPIO_DIR_OUT);
  GPIO_SetOutBits(GPIOB,LED_PASS);

  //initial LED FAIL
  AFIO_GPBConfig(AFIO_PIN_14 , AFIO_MODE_DEFAULT);
  GPIO_DirectionConfig(GPIOB, LED_FAIL, GPIO_DIR_OUT);
  GPIO_SetOutBits(GPIOB,LED_FAIL);

  //initial FP_POWER
  AFIO_GPBConfig(AFIO_PIN_13 , AFIO_MODE_DEFAULT);
  GPIO_DirectionConfig(GPIOB, FP_POWER, GPIO_DIR_OUT);
  GPIO_ClearOutBits(GPIOB,FP_POWER);

  //initial COIL_CONTROL
  AFIO_GPBConfig(AFIO_PIN_12 , AFIO_MODE_DEFAULT);
  GPIO_DirectionConfig(GPIOB, COIL_CONTROL, GPIO_DIR_OUT);
  GPIO_ClearOutBits(GPIOB,COIL_CONTROL);
}

/*********************************************************************************************************//**
  * @brief  This function is used to configure USART.
  * @param  None
  * @retval None
  ***********************************************************************************************************/
void USART_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;


	USART_InitStructure.USART_BaudRate = UART_BR ;
	USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B ;
	USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
	USART_InitStructure.USART_Parity = USART_PARITY_NO ;
	USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
	USART_Init(USART, &USART_InitStructure);  
}

void USART_Configuration_1(void)
{
	USART_InitTypeDef USART_InitStructure;

  USART_DeInit(USART);
	USART_InitStructure.USART_BaudRate = 115200 ;
	USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B ;
	USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
	USART_InitStructure.USART_Parity = USART_PARITY_NO ;
	USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
	USART_Init(USART, &USART_InitStructure);  
}



void delay_ms(u32 wCnt)
{
	SYSTICK_CounterCmd(SYSTICK_COUNTER_ENABLE);
	gwSysTickCnt = 0;
	while(gwSysTickCnt <= wCnt);
	
	/* Disable SysTick Counter                                                                                */
	SYSTICK_CounterCmd(SYSTICK_COUNTER_DISABLE);
	/* Clear SysTick Counter                                                                                  */
	SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
}

short SendCommand(unsigned char * ucData,unsigned char length)
{
	unsigned char i;
	unsigned char tmp;
	for(i = 0; i < length; i++)
	{
		SERIAL_PutChar(*(ucData + i));
	}
	return OK;
}

short ReceiveCommand(unsigned char * ucData,unsigned char length)
{
	unsigned char i;
	unsigned int tmp = 0;
  unsigned int iCount = 0;
  //SYSTICK_CounterCmd(SYSTICK_COUNTER_ENABLE);
	//gwSysTickCnt = 0;
	for(i = 0;i < length; i++)
	{
		//*(ucData + i) = SERIAL_GetChar();
    while (USART_GetLineStatus(RETARGET_USART_PORT, USART_LSR_RFDR) == RESET)
    {
      tmp++;
      if(tmp == 10000)
      {
        iCount++;
        tmp = 0;
      }
      if(iCount == 150)
      {
        return FAIL;
      }
    }
    *(ucData + i) = USART_ReceiveData(RETARGET_USART_PORT);
		tmp = 0;
    iCount = 0;
	}

	return OK;
}
bool Erase_Detect()
{
	bool bState;
	bState = GPIO_ReadInBit(GPIOB,ERASE);
	return bState;
}
bool Login_Detect()
{
	bool bState;
	bState = GPIO_ReadInBit(GPIOB,LOGIN);
	return bState;
}
bool Del_Detect()
{
	bool bState;
	bState = GPIO_ReadInBit(GPIOA,DEL);
	return bState;
}
