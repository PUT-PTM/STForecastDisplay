#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_usart.h"
#include "misc.h"
//#include "defines.h"
//#include "stm32f4xx.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_hd44780.h"

void init_usart();

char napis[16]="Warszawa";
char buffor[32];
char *buf = &buffor;
char *start = &buffor;
int  button1=0, button2=0;

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		if(USART3->DR != '\n' && USART3->DR != '\r'){
			*buf = USART3->DR;
			buf++;
		}
		if(USART3->DR == '\r') buf = start;
	}
}

void EXTI1_IRQHandler(void)
{
         	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
         	{
         		TIM_Cmd(TIM4, ENABLE);
         		EXTI_ClearITPendingBit(EXTI_Line1);
   	   	}
}

void TIM4_IRQHandler(void)
				{
				         if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){
				        		if(button1==0&&button2==0){
				        	         TM_HD44780_Clear();
				        	         TM_HD44780_Puts(0, 0, "Poznan");
				        	         TM_HD44780_Puts(0, 1, "Michal Gozdek");
				        	         button1++;
				        	        }
				        	     else if(button1==1&&button2==0){
				        	          TM_HD44780_Clear();
				        	          TM_HD44780_Puts(0, 0, napis);
				        	          TM_HD44780_Puts(0, 1, "Michal Gozdek");
				        	          button1--;
				        	         }
				        	     else if(button1==0&&button2==1){
				        	     		TM_HD44780_Clear();
				        	     		TM_HD44780_Puts(0, 0, "Poznan");
				        	     		TM_HD44780_Puts(0, 1, "Dominik Kaczmare");
				        	     		button1++;
				        	     	}
				        	     else if(button1==1&&button2==1){
				        	     		TM_HD44780_Clear();
				        	     		TM_HD44780_Puts(0, 0, napis);
				        	     		TM_HD44780_Puts(0, 1, "Dominik Kaczmare");
				        	     		button1--;
				        	     }
				        	          TIM_Cmd(TIM4, DISABLE);
				        	          TIM_SetCounter(TIM4, 0);
				            TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
				         	}
				}


void EXTI2_IRQHandler(void)
{
         	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
         	{
         		TIM_Cmd(TIM3, ENABLE);
         		EXTI_ClearITPendingBit(EXTI_Line2);
   	   	}
}

void TIM3_IRQHandler(void)
				{
				         if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
				     		if(button1==0&&button2==0){
				     			TM_HD44780_Clear();
				     			TM_HD44780_Puts(0, 0, "Poznan");
				     			TM_HD44780_Puts(0, 1, "Michal Gozdek");
				     			button2++;
				     		}
				     		else if(button1==1&&button2==0){
				     			TM_HD44780_Clear();
				     			TM_HD44780_Puts(0, 0, napis);
				     			TM_HD44780_Puts(0, 1, "Michal Gozdek");
				     			button2++;
				     		 }
				     		else if(button1==0&&button2==1){
				     			TM_HD44780_Clear();
				     			TM_HD44780_Puts(0, 0, "Poznan");
				     			TM_HD44780_Puts(0, 1, "Dominik Kaczmare");
				     			button2--;
				     		}
				     		else if(button1==1&&button2==1){
				     			TM_HD44780_Clear();
				     			TM_HD44780_Puts(0, 0, napis);
				     			TM_HD44780_Puts(0, 1, "Dominik Kaczmare");
				     			button2--;
				     		}
				        	          TIM_Cmd(TIM3, DISABLE);
				        	          TIM_SetCounter(TIM3, 0);
				            TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
				         	}
				}


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

	TM_HD44780_Init(16, 2);
	init_usart();
	init_EXTI();
	SendString("AT\r\n");


	while(1)
	{

	}

}

void init_usart(){
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
 void init_EXTI(){

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* GPIOD Periph clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);



	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 1679;
	TIM_TimeBaseStructure.TIM_Prescaler = 9999;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

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


		///////////////////////////////////////////////////////////////

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);



	TIM_TimeBaseStructure.TIM_Period = 1679;
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
