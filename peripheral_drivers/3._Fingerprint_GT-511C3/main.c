/*********************************************************************************************************//**
 * @file    GPIO/InputOutput/main.c
 * @version V1.0
 * @date    04/11/2011
 * @brief   Main program.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2011 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32f125x.h"
#include "ht32f125x_board.h"
#include "LCD.h"
#include "Protocol.h"
#include "Hardware.h"
#include "main.h"



SB_OEM_PKT Command_Packet;
extern  vu32 gwSysTickCnt;
char cFlagA, cFlagB, cFlagC,gJump;

void buzzer_delay(int count)
{
  int i = 0;
  for(i = 0; i < count; i++)
    ;
}

void buzzer_control(char state)
{
  int i = 0, j = 0;
  if(state == 1)
  {
    for(i = 0; i < 500; i++)
    {
      GPIO_SetOutBits(GPIOB,COIL_CONTROL);
      buzzer_delay(500);
      GPIO_ClearOutBits(GPIOB,COIL_CONTROL);
      buzzer_delay(500);
    }
  }
  else if(state == 2)
  {
     for(j = 0; j < 2; j++)
    {
      for(i = 0; i < 500; i++)
      {
        GPIO_SetOutBits(GPIOB,COIL_CONTROL);
        buzzer_delay(500);
        GPIO_ClearOutBits(GPIOB,COIL_CONTROL);
        buzzer_delay(500);
      }
      delay_ms(10);
    }
  }
  else
  {
    for(j = 0; j < 3; j++)
    {
      for(i = 0; i < 50; i++)
      {
        GPIO_SetOutBits(GPIOB,COIL_CONTROL);
        buzzer_delay(500);
        GPIO_ClearOutBits(GPIOB,COIL_CONTROL);
        buzzer_delay(500);
      }
      delay_ms(5);
    }
  }
}

int main(void)
{
	u32 i = 0;
  int Button_State = FAIL;
	int ID_Count = 0;
	int rtn;
	int iTimeCount = 0;
	char cLoginCount = 0;
	char j =0xaa;
  unsigned char DataBuffer[12];
  char ttmp = 0;
  unsigned char FlowState = 0;
	cFlagA = 0;
	cFlagB = 0;
	cFlagC = 0;
	gJump = 0;
  
#if (HT32F_LIB_DEBUG == 1)
  debug();
#endif
	NVIC_Configuration();               /* NVIC configuration                                                 */
	CKCU_Configuration();               /* System Related configuration                                       */
	//  while(1);
	
	GPIO_Configuration();               /* GPIO Related configure                                             */
	USART_Configuration();              /* USART Related configure     										  */
  delay_ms(1);
  GPIO_SetOutBits(GPIOB,FP_POWER);
  delay_ms(1);
  GPIO_ClearOutBits(GPIOB,LED_ENROLL);
  GPIO_ClearOutBits(GPIOB,LED_LOGIN);
  GPIO_ClearOutBits(GPIOB,LED_DEL);
	i = Open(0,	&DataBuffer[0]);
	if(i != OK)
		while(1)
			;
  
  GPIO_SetOutBits(GPIOB,LED_ENROLL);
  GPIO_SetOutBits(GPIOB,LED_LOGIN);
  GPIO_SetOutBits(GPIOB,LED_DEL);
  ChangeBaudRate(115200);
  USART_Configuration_1();
	while(1)
	{
		iTimeCount = 0;
		Button_State = FAIL;
		while(Button_State == FAIL)
			Button_State = Button_Detect(); 
 		switch(Button_State)
		{
			case ENROLL_CLICK:
				GPIO_ClearOutBits(GPIOB,LED_ENROLL);
				LED(1);
				for(ID_Count = 0; ID_Count < FP_MAX_USERS; ID_Count++)
				{
					rtn = CheckEnroll(ID_Count);
					if(rtn == NACK_IS_NOT_USED)
						break;
				}
        if(ID_Count == FP_MAX_USERS)
        {
					Button_State = FAIL;
					while(Button_State != DEL_CLICK)
						Button_State = Button_Detect();
					LED(0);
					GPIO_SetOutBits(GPIOB,LED_ENROLL);
					break;
        }						
				rtn = Enroll_Start(ID_Count);
				if(rtn < 0)
				{
					Button_State = FAIL;
					while(Button_State != DEL_CLICK)
						Button_State = Button_Detect();
					LED(0);
					GPIO_SetOutBits(GPIOB,LED_ENROLL);
					break;
				}
				else if(rtn >= NACK_NONE)
				{
					Button_State = FAIL;
					while(Button_State != DEL_CLICK)
						Button_State = Button_Detect();
					LED(0);
					GPIO_SetOutBits(GPIOB,LED_ENROLL);
					break;
				}
				
				for(j = 1; j < 4; j++)
				{
					SYSTICK_CounterCmd(SYSTICK_COUNTER_ENABLE);
					gwSysTickCnt = 0;
					iTimeCount = 0;
					while(iTimeCount < TimeOut)
					{
            if(ttmp == 0)
            {    
              GPIO_ClearOutBits(GPIOB,LED_ENROLL);
              ttmp = 1;
            }
            else
            {
              GPIO_SetOutBits(GPIOB,LED_ENROLL);
              ttmp = 0;
            }
						rtn = CaptureFinger(1);
            
						if(Command_Packet.wCmd == ACK)
						{
							delay_ms(10);
							rtn = CaptureFinger(1);
              						GPIO_ClearOutBits(GPIOB,LED_ENROLL);
             						 buzzer_control(1);
						}
						if(Command_Packet.wCmd == ACK)
							break;
            				if(rtn == FAIL)
            				{
    						Button_State = FAIL;
    						while(Button_State != DEL_CLICK)
    						Button_State = Button_Detect();
    						LED(0);
    						GPIO_SetOutBits(GPIOB,LED_ENROLL);
              					break;
            				}
						if(gwSysTickCnt >= DELAY_TIME)
						{
							gwSysTickCnt = 0;
							iTimeCount++;
						}
						Button_State = Button_Detect();
						if(Button_State == DEL_CLICK)
							break;
					}
					/* Disable SysTick Counter                                                                                */
					SYSTICK_CounterCmd(SYSTICK_COUNTER_DISABLE);
					/* Clear SysTick Counter                                                                                  */
					SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
					if(Button_State == DEL_CLICK)
					{
						break;
					}
					if(iTimeCount >= TimeOut || rtn == FAIL)
					{
						iTimeCount = 0;
						rtn = NACK_TIMEOUT;
						break;
					}
					rtn = Enroll_nth(j);
					if(rtn < 0)
					{
						break;
					}
					if(Command_Packet.wCmd == NACK)
					{
						break;
					}
          
					if(j < 3)
					{
						while(1)
						{
							rtn = RemoveFinger();
							if(Command_Packet.nParam != 0)
					      break;
            }
					}
				}
				if(Command_Packet.wCmd != ACK)
				{
					LED(0);
          GPIO_ClearOutBits(GPIOB,LED_FAIL);
          delay_ms(50);
          GPIO_SetOutBits(GPIOB,LED_FAIL);
          GPIO_SetOutBits(GPIOB,LED_ENROLL);
          FlowState = 0;
          buzzer_control(2);
				}
				else
				{
					LED(0);
          GPIO_ClearOutBits(GPIOB,LED_PASS);
          delay_ms(50);
          GPIO_SetOutBits(GPIOB,LED_PASS);
          GPIO_SetOutBits(GPIOB,LED_ENROLL);
          buzzer_control(0);
				}
				break;
			case LOGIN_CLICK:
				LED(1);
				GPIO_ClearOutBits(GPIOB,LED_LOGIN);
				rtn = GetEnrollCount();
				if(Command_Packet.wCmd != ACK || Command_Packet.nParam == 0)
				{
					LED(0);
					GPIO_SetOutBits(GPIOB,LED_LOGIN);
					break;
				}						
				for(cLoginCount = 0; cLoginCount < LOGIN_NUMBER; cLoginCount++)
				{
					SYSTICK_CounterCmd(SYSTICK_COUNTER_ENABLE);
					gwSysTickCnt = 0;
					iTimeCount = 0;
					while(iTimeCount < TimeOut)
					{
            if(ttmp == 0)
            {    
              GPIO_ClearOutBits(GPIOB,LED_LOGIN);
              ttmp = 1;
            }
            else
            {
              GPIO_SetOutBits(GPIOB,LED_LOGIN);
              ttmp = 0;
            }
						rtn = CaptureFinger(0);
        		if(Command_Packet.wCmd == ACK)
        		{
        			delay_ms(10);
        			rtn = CaptureFinger(0);
              break;
        		}
						Button_State = Button_Detect();
						if(Button_State == DEL_CLICK)
						{
							//Display_Main();
							rtn = NACK_TIMEOUT;
							break;
						}
						if(gwSysTickCnt >= DELAY_TIME)
						{
							gwSysTickCnt = 0;
							iTimeCount++;
						}
					}
					/* Disable SysTick Counter                                                                                */
					SYSTICK_CounterCmd(SYSTICK_COUNTER_DISABLE);
					/* Clear SysTick Counter                                                                                  */
					SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
//					_t0on = 0;
					if(iTimeCount >= TimeOut)
					{
						iTimeCount = 0;
						rtn == NACK_TIMEOUT;
						break;
					}
					else if(rtn != OK)
					{
						LED(0);
						GPIO_SetOutBits(GPIOB,LED_LOGIN);
						break;
					}
					//Display_Remove();
					rtn = Identify();
					if(Command_Packet.nParam < FP_MAX_USERS)
						break;
				}
				if(rtn == NACK_TIMEOUT)
					break;

        
		    if(Command_Packet.nParam >= NACK_NONE)
				{
          LED(0);
          FlowState = 1;
					GPIO_ClearOutBits(GPIOB,LED_FAIL);
          buzzer_control(2);
				}
				else				   
				{
          LED(0);
					GPIO_ClearOutBits(GPIOB,LED_PASS);
					GPIO_SetOutBits(GPIOB,COIL_CONTROL);
          buzzer_control(0);
				}
        delay_ms(50);
				
				GPIO_SetOutBits(GPIOB,LED_LOGIN);
				GPIO_SetOutBits(GPIOB,LED_FAIL);
				GPIO_SetOutBits(GPIOB,LED_PASS);
				GPIO_ClearOutBits(GPIOB,COIL_CONTROL);
				break;
			case DEL_CLICK:
				GPIO_ClearOutBits(GPIOB,LED_DEL);			
					rtn = DeleteAll();
					if(rtn < 0)
					{
            FlowState = 3;
						GPIO_ClearOutBits(GPIOB,LED_FAIL);
            buzzer_control(2);
					}
					if(rtn != OK)
					{
            FlowState = 3;
						GPIO_ClearOutBits(GPIOB,LED_FAIL);
            buzzer_control(2);
					}
					else
					{
						GPIO_ClearOutBits(GPIOB,LED_PASS);		
            buzzer_control(0);				
					}
          delay_ms(50);
          GPIO_SetOutBits(GPIOB,LED_DEL);
          GPIO_SetOutBits(GPIOB,LED_PASS);
          GPIO_SetOutBits(GPIOB,LED_FAIL);
          GPIO_SetOutBits(GPIOB,LED_DEL);
				break;
			default:
				break;
		}
	}
}

int Button_Detect()
{
	if(cFlagA == 0)
	{
		if(Erase_Detect())
		{
			cFlagA = 1;
			gJump = 0;
			return ENROLL_CLICK;
		}
	}
	else if(!Erase_Detect())
	{
		cFlagA = 0;
	}
	if(cFlagB == 0)
	{
		if(!Login_Detect())
		{
			cFlagB = 1;
			gJump = 0;
			return LOGIN_CLICK;
		}
	}
	else if(Login_Detect())
	{
		cFlagB = 0;
	}
	if(cFlagC == 0)
	{
		if(!Del_Detect())
		{
			cFlagC = 1;
			gJump = 0;
			return DEL_CLICK;
		}
	}
	else if(Del_Detect())
	{
		cFlagC = 0;
	}
	return FAIL;
}
