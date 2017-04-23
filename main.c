/* ESP inputs */
//	---------------------------
// |TX 	> C11	|GND	> GND  |
// |CH 	> 3V	|GPIO2	> Null |
// |RST > 3V	|GPIO0	> Null |
// |Vcc > 3v	|RX		> C10  |
//	---------------------------

#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_usart.h"
#include "misc.h"
//#include "defines.h"
//#include "stm32f4xx.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_hd44780.h"
void init_usart();

//const int msg 512;
char getPoznan[89] = ">GET http://api.wunderground.com/api/3d8b02539ee9b6a0/conditions/q/EPPO.json HTTP/1.1\r\n";
//3d8b02539ee9b6a0

/* wifi ssid & password */
const char ssid[6] = "test";
const char pass[9] = "myesp8266";

/* buffer pointers for debugging purpose */
char buffor[64];
char *buf = &buffor;
char *start = &buffor;
//char msgbuffer[msg];

/* read from USART */
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		if(USART3->DR != '\r'){
			*buf = USART3->DR;
			buf++;
		}

	}
}

/* send to USART */
void SendString(char *s)
{
	while(*s)
	{
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET);
		USART_SendData(USART3,*s);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
		s++;
	}

}

char* ConnecttoWiFi(char *ssid, char *pass){

	return "hello";
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	SystemInit();

	TM_HD44780_Init(16, 2);
	Init_Usart();

	// esp reset command
	SendString("AT+RST\r\n");
	Delayms(2000);
	printf(buffor);
	buf = start;

	// esp client mode enabled
	SendString("AT+CWMODE=1\r\n");
	Delayms(2000);
	printf(buffor);
	buf = start;

	// connecting esp to network
	SendString("AT+CWJAP=\"networktes\",\"myesp8266\"\r\n");
	Delayms(10000);
	printf(buffor);
	buf = start;

	// get local IP adress
	/*SendString("AT+CIFSR\r\n");
	Delayms(5000);
	printf(buffor);
	buf = start;
	*/

	// connect to wunderground.com
	SendString("AT+CIPSTART=\"TCP\",\"google.com\",80\r\n");
	Delayms(10000);
	printf(buffor);
	buf=start;

	// close connection with site
	SendString("AT+CIPSEND=18\r\n");
	Delayms(500);
	SendString("GET / HTTP/1.0\r\n");
	Delayms(10000);
	printf(buffor);
	buf=start;

	TM_HD44780_Puts(0, 0, "STM32F4");
	TM_HD44780_Puts(0, 1, "20x4 HD44780 LCD");
	//Delayms(3000);
	TM_HD44780_Clear();
	TM_HD44780_Puts(0, 0, "Michal Gozdek");
	TM_HD44780_Puts(0, 1, "DominikKaczmarek");
	while(1)
	{

	}

}

void Init_Usart(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	// konfiguracja linii Rx i Tx
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	// ustawienie funkcji alternatywnej dla pinów (USART)
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

	//struktura do konfiguracji kontrolera NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_EnableIRQ(USART3_IRQn);

	USART_Cmd(USART3, ENABLE);
}
