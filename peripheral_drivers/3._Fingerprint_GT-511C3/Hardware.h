
#define UART_BR  		9600
#define ERASE			GPIO_PIN_10
#define LOGIN			GPIO_PIN_11
#define DEL				GPIO_PIN_10
#define LED_ENROLL		GPIO_PIN_4
#define LED_LOGIN		GPIO_PIN_3
#define LED_DEL			GPIO_PIN_2
#define LED_PASS		GPIO_PIN_15
#define LED_FAIL		GPIO_PIN_14
#define FP_POWER		GPIO_PIN_13
#define COIL_CONTROL	GPIO_PIN_12

void NVIC_Configuration(void);
void CKCU_Configuration(void);
void GPIO_Configuration(void);
void USART_Configuration(void);
void USART_Configuration_2(void);
void delay_ms(u32 wCnt);
void USART_Configuration_1(void);
short ReceiveCommand(unsigned char * ucData,unsigned char length);
short SendCommand(unsigned char * ucData,unsigned char length);
bool Erase_Detect();
bool Login_Detect();
bool Del_Detect();