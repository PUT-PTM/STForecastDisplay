#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_usart.h"
#include "misc.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_hd44780.h"
#include "esp8266_commands.h"
#include "extim.h"

void init_usart();

/* controls buttons */
int button1=0, button2=0;

/* http get calls */
char getPoznan[94] = "GET /api/<your api key>/conditions/q/EPPO.json HTTP/1.1\r\nHost: api.wunderground.com\r\n\r\n";
char getWarszawa[94] = "GET /api/<your api key>/conditions/q/EPWA.json HTTP/1.1\r\nHost: api.wunderground.com\r\n\r\n";
char getKrakow[94] = "GET /api/<your api key>/conditions/q/EPKK.json HTTP/1.1\r\nHost: api.wunderground.com\r\n\r\n";
char getWroclaw[94] = "GET /api/<your api key>/conditions/q/EPWR.json HTTP/1.1\r\nHost: api.wunderground.com\r\n\r\n";
char getGdansk[94] = "GET /api/<your api key>/conditions/q/EPGD.json HTTP/1.1\r\nHost: api.wunderground.com\r\n\r\n";

/* buffer[count] to store get output */
char buffor[4096];
int count = 0;
int refresh_flag = 0;

/* containers for weather info */
char temperaturePO[4];
char overviewPO[15];
char humidityPO[4];
char windPO[3];
char pressurePO[5];

char temperWA[4];
char overviewWA[15];
char humidityWA[4];
char windWA[3];
char pressureWA[5];

char tempKK[4];
char overviewKK[15];
char humidityKK[4];
char wind_kphKK[3];
char pressuKK[5];

char teWR[4];
char overWR[15];
char humidityWR[4];
char wiatr[3];
char pressureWR[5];

char tempGD[4];
char overviewGD[15];
char humidityGD[4];
char windGD[3];
char pressureGD[5];

/* read from USART */
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		if(USART3->DR != '/r'){
			buffor[count] = USART3->DR;
			count++;
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

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	SystemInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	TM_HD44780_Init(16, 2);
	init_EXTI_Refresh();
	Init_Usart();

	int check = 1;
	init_symbols();
	int i;
	for(i=0;i<4096;i++){
		buffor[i]=0;
	}

	TM_HD44780_Clear();
	TM_HD44780_Puts(0, 0, "Loading");

	check = initAT();
	TM_HD44780_PutCustom(0,1,5);
	TM_HD44780_PutCustom(1,1,5);

	if(check == 1)
	{
		check = initNetwork();
		if(check == 1)
		{
			check = refreshInfo();
			if(check != 1)
			{
				check = sendCommand("AT+CIPCLOSE\r\n", "OK");
				if(check != 1){
					TM_HD44780_Clear();
					TM_HD44780_Puts(0, 0, "Close Error");
					TM_HD44780_Puts(0, 1, "Restart Device");
				}
				else{
					TM_HD44780_Clear();
					TM_HD44780_Puts(0, 0, "Refresh Error");
					TM_HD44780_Puts(0, 1, "Try Again Later");
				}
			}
			else TIM_Cmd(TIM5, ENABLE);
		}
		else
			{
				TM_HD44780_Clear();
				TM_HD44780_Puts(0, 0, "Connection Error");
				TM_HD44780_Puts(0, 1, "Restart Device");
			}
		}
	else
	{
		TM_HD44780_Clear();
		TM_HD44780_Puts(0, 0, "ESP Init Error");
		TM_HD44780_Puts(0, 1, "Restart Device");
	}

	init_EXTI();
	TIM_Cmd(TIM3, ENABLE);

	while(1)
	{
		if(refresh_flag == 1)
		{
			TIM_Cmd(TIM3, DISABLE);
			TIM_SetCounter(TIM3, 0);
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
			TIM_Cmd(TIM5, DISABLE);
			TIM_SetCounter(TIM5, 0);
			TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
			check = refreshInfo();
			if(check != 1)
			{
				check = sendCommand("AT+CIPCLOSE\r\n", "OK");
				if(check != 1){
					TM_HD44780_Clear();
					TM_HD44780_Puts(0, 0, "Close Error");
					TM_HD44780_Puts(0, 1, "Restart Device");
				}
				else{
					TM_HD44780_Clear();
					TM_HD44780_Puts(0, 0, "Refresh Error");
					TM_HD44780_Puts(0, 1, "Try Again Later");
				}
			}
			refresh_flag = 0;
			TIM_Cmd(TIM3, ENABLE);
			TIM_Cmd(TIM5, ENABLE);
		}
	}
}

void Init_Usart(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
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

void init_EXTI_Refresh(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	/* auto refreshing timer */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 1049999;
	TIM_TimeBaseStructure.TIM_Prescaler = 9999;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = 2099;
	TIM_TimeBaseStructure.TIM_Prescaler = 9999;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	SYSCFG_EXTILineConfig(GPIOA, EXTI_PinSource3);
}

 void init_EXTI(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* buttons timer */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 2099;
	TIM_TimeBaseStructure.TIM_Prescaler = 9999;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	SYSCFG_EXTILineConfig(GPIOA, EXTI_PinSource1);

	/* toggle timer */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = 33599;
	TIM_TimeBaseStructure.TIM_Prescaler = 9999;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	SYSCFG_EXTILineConfig(GPIOA, EXTI_PinSource2);
 }

 void init_symbols(){
	//temperature
	uint8_t customChar[] = {
			0x0E,
			0x0A,
			0x0A,
			0x0A,
			0x0A,
			0x11,
			0x11,
			0x0E
	};
	TM_HD44780_CreateChar(0, &customChar[0]);

	//wind
	uint8_t wind[] = {
			0x00,
			0x18,
			0x05,
			0x02,
			0x18,
			0x05,
			0x02,
			0x00
	};
	TM_HD44780_CreateChar(1, &wind[0]);

	//pressure
	uint8_t pressure[] = {
			0x00,
			0x04,
			0x04,
			0x04,
			0x04,
			0x15,
			0x0E,
			0x04
	};
	TM_HD44780_CreateChar(2, &pressure[0]);

	//rain
	uint8_t rain[] = {
			0x00,
			0x0C,
			0x12,
			0x11,
			0x0E,
			0x00,
			0x08,
			0x02
	};
	TM_HD44780_CreateChar(3, &rain[0]);

	//Celcius
	uint8_t C[] = {
			0x00,
			0x0E,
			0x0A,
			0x0E,
			0x00,
			0x00,
			0x00,
			0x00
	};
	TM_HD44780_CreateChar(4, &C[0]);

	//blank
		uint8_t blank[] = {
				0x1F,
				0x1F,
				0x1F,
				0x1F,
				0x1F,
				0x1F,
				0x1F,
				0x1F
		};
		TM_HD44780_CreateChar(5, &blank[0]);

}
