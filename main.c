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

void init_usart();

int button1=0, button2=0;

/* http get calls */
char getPoznan[94] = "GET /api/3d8b02539ee9b6a0/conditions/q/EPPO.json HTTP/1.1\r\nHost: api.wunderground.com\r\n\r\n";
char getWarszawa[94] = "GET /api/3d8b02539ee9b6a0/conditions/q/EPWA.json HTTP/1.1\r\nHost: api.wunderground.com\r\n\r\n";
char getKrakow[94] = "GET /api/3d8b02539ee9b6a0/conditions/q/EPKK.json HTTP/1.1\r\nHost: api.wunderground.com\r\n\r\n";

/* wifi ssid & password */
const char ssid[6] = "test";
const char pass[9] = "myesp8266";

/* buffer[count] to store get output */
char buffor[4096];
int count = 0;

/* containers for weather info */
char temperaturePO[4];
char overviewPO[15];
char humidityPO[4];
char wind_kphPO[3];
char pressurePO[5];

char temperatureWA[4];
char overviewWA[15];
char humidityWA[4];
char wind_kphWA[3];
char pressureWA[5];

char temperatureKK[4];
char overviewKK[15];
char humidityKK[4];
char wind_kphKK[3];
char pressureKK[5];

/* test our symbol */
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
			 if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) != RESET){
			 button1++;
			 if(button1>2)button1=0;
				if(button1==0){


					switch (button2) {
						case 0:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Poznan");
							TM_HD44780_PutCustom(0,1,0);
							TM_HD44780_Puts(1, 1, "temp");
							TM_HD44780_Puts(6, 1, temperaturePO);
							TM_HD44780_PutCustom(10,1,4);
							TM_HD44780_Puts(11, 1, "C");
							break;
						case 1:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Poznan");
							TM_HD44780_Puts(0, 1, overviewPO);

							break;
						case 2:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Poznan");
							TM_HD44780_PutCustom(0,1,3);
							TM_HD44780_Puts(1, 1, "humidity");
							TM_HD44780_Puts(10, 1, humidityPO);
							break;
						case 3:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Poznan");
							TM_HD44780_PutCustom(0,1,1);
							TM_HD44780_Puts(1, 1, "wind");
							TM_HD44780_Puts(6, 1, wind_kphPO);
							TM_HD44780_Puts(9, 1, "km/h");
							break;
						case 4:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Poznan");
							TM_HD44780_PutCustom(0,1,2);
							TM_HD44780_Puts(1, 1, "press");
							TM_HD44780_Puts(7, 1, pressurePO);
							TM_HD44780_Puts(12, 1, "hpa");
							break;
						default:
							break;
					}

					}
				 else if(button1==1){

					 switch (button2) {
						case 0:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Warszawa");
							TM_HD44780_PutCustom(0,1,0);
							TM_HD44780_Puts(1, 1, "temp");
							TM_HD44780_Puts(6, 1, temperatureWA);
							TM_HD44780_PutCustom(10,1,4);
							TM_HD44780_Puts(11, 1, "C");
							break;
						case 1:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Warszawa");
							TM_HD44780_Puts(0, 1, overviewWA);
							break;
						case 2:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Warszawa");
							TM_HD44780_PutCustom(0,1,3);
							TM_HD44780_Puts(1, 1, "humidity");
							TM_HD44780_Puts(10, 1, humidityWA);
							break;
						case 3:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Warszawa");
							TM_HD44780_PutCustom(0,1,1);
							TM_HD44780_Puts(1, 1, "wind");
							TM_HD44780_Puts(6, 1, wind_kphWA);
							TM_HD44780_Puts(9, 1, "km/h");
							break;
						case 4:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Warszawa");
							TM_HD44780_PutCustom(0,1,2);
							TM_HD44780_Puts(1, 1, "press");
							TM_HD44780_Puts(7, 1, pressureWA);
							TM_HD44780_Puts(12, 1, "hpa");
							break;
						default:
							break;
						}

					 }
				 else if(button1==2){

					switch (button2) {
						case 0:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Krakow");
							TM_HD44780_PutCustom(0,1,0);
							TM_HD44780_Puts(1, 1, "temp");
							TM_HD44780_Puts(6, 1, temperatureKK);
							TM_HD44780_PutCustom(10,1,4);
							TM_HD44780_Puts(11, 1, "C");
							break;
						case 1:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Krakow");
							TM_HD44780_Puts(0, 1, overviewKK);
							break;
						case 2:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Krakow");
							TM_HD44780_PutCustom(0,1,3);
							TM_HD44780_Puts(1, 1, "humidity");
							TM_HD44780_Puts(10, 1, humidityKK);
							break;
						case 3:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Krakow");
							TM_HD44780_PutCustom(0,1,1);
							TM_HD44780_Puts(1, 1, "wind");
							TM_HD44780_Puts(6, 1, wind_kphKK);
							TM_HD44780_Puts(9, 1, "km/h");
							break;
						case 4:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Krakow");
							TM_HD44780_PutCustom(0,1,2);
							TM_HD44780_Puts(1, 1, "press");
							TM_HD44780_Puts(7, 1, pressureKK);
							TM_HD44780_Puts(12, 1, "hpa");
							break;
						default:
							break;
							}

							}
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
			 if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2) != RESET){
			 button2++;
			 if(button2>4)button2=0;
			 if(button2==0){
				 switch (button1) {
					case 0:
						TM_HD44780_Clear();
						TM_HD44780_Puts(0, 0, "Poznan");
						TM_HD44780_PutCustom(0,1,0);
						TM_HD44780_Puts(1, 1, "temp");
						TM_HD44780_Puts(6, 1, temperaturePO);
						TM_HD44780_PutCustom(10,1,4);
						TM_HD44780_Puts(11, 1, "C");
						break;
					case 1:
						TM_HD44780_Clear();
						TM_HD44780_Puts(0, 0, "Warszawa");
						TM_HD44780_PutCustom(0,1,0);
						TM_HD44780_Puts(1, 1, "temp");
						TM_HD44780_Puts(6, 1, temperatureWA);
						TM_HD44780_PutCustom(10,1,4);
						TM_HD44780_Puts(11, 1, "C");
						break;
					case 2:
						TM_HD44780_Clear();
						TM_HD44780_Puts(0, 0, "Krakow");
						TM_HD44780_PutCustom(0,1,0);
						TM_HD44780_Puts(1, 1, "temp");
						TM_HD44780_Puts(6, 1, temperatureKK);
						TM_HD44780_PutCustom(10,1,4);
						TM_HD44780_Puts(11, 1, "C");
						break;
					default:
						break;
				}

				}
				else if(button2==1){

					switch (button1) {
						case 0:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Poznan");
							TM_HD44780_Puts(0, 1, overviewPO);
							break;
						case 1:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Warszawa");
							TM_HD44780_Puts(0, 1, overviewWA);
							break;
						case 2:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Krakow");
							TM_HD44780_Puts(0, 1, overviewKK);
							break;
						default:
							break;
					}

					}
				else if(button2==2){

					switch (button1) {
						case 0:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Poznan");
							TM_HD44780_PutCustom(0,1,3);
							TM_HD44780_Puts(1, 1, "humidity");
							TM_HD44780_Puts(10, 1, humidityPO);
							break;
						case 1:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Warszawa");
							TM_HD44780_PutCustom(0,1,3);
							TM_HD44780_Puts(1, 1, "humidity");
							TM_HD44780_Puts(10, 1, humidityWA);
							break;
						case 2:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Krakow");
							TM_HD44780_PutCustom(0,1,3);
							TM_HD44780_Puts(1, 1, "humidity");
							TM_HD44780_Puts(10, 1, humidityKK);
							break;
						default:
							break;
							}

							}
				else if(button2==3){

					switch (button1) {
						case 0:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Poznan");
							TM_HD44780_PutCustom(0,1,1);
							TM_HD44780_Puts(1, 1, "wind");
							TM_HD44780_Puts(6, 1, wind_kphPO);
							TM_HD44780_Puts(9, 1, "km/h");
							break;
						case 1:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Warszawa");
							TM_HD44780_PutCustom(0,1,1);
							TM_HD44780_Puts(1, 1, "wind");
							TM_HD44780_Puts(6, 1, wind_kphWA);
							TM_HD44780_Puts(9, 1, "km/h");
							break;
						case 2:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Krakow");
							TM_HD44780_PutCustom(0,1,1);
							TM_HD44780_Puts(1, 1, "wind");
							TM_HD44780_Puts(6, 1, wind_kphKK);
							TM_HD44780_Puts(9, 1, "km/h");
							break;
						default:
							break;
								}

								}
				else if(button2==4){

					switch (button1) {
						case 0:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Poznan");
							TM_HD44780_PutCustom(0,1,2);
							TM_HD44780_Puts(1, 1, "press");
							TM_HD44780_Puts(7, 1, pressurePO);
							TM_HD44780_Puts(12, 1, "hpa");
							break;
						case 1:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Warszawa");
							TM_HD44780_PutCustom(0,1,2);
							TM_HD44780_Puts(1, 1, "press");
							TM_HD44780_Puts(7, 1, pressureWA);
							TM_HD44780_Puts(12, 1, "hpa");
							break;
						case 2:
							TM_HD44780_Clear();
							TM_HD44780_Puts(0, 0, "Krakow");
							TM_HD44780_PutCustom(0,1,2);
							TM_HD44780_Puts(1, 1, "press");
							TM_HD44780_Puts(7, 1, pressureKK);
							TM_HD44780_Puts(12, 1, "hpa");
							break;
						default:
							break;
							}

							}
		  }
		  TIM_Cmd(TIM3, DISABLE);
		  TIM_SetCounter(TIM3, 0);
		  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
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
	TM_HD44780_Init(16, 2);
	init_EXTI();
	Init_Usart();
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

	//C
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

	int i;
	for(i=0;i<4096;i++){
		buffor[i]=0;
	}

	TM_HD44780_Clear();
	TM_HD44780_Puts(0, 0, "Wczytywanie");

	initAT();

	TM_HD44780_PutCustom(0,1,5);
		TM_HD44780_PutCustom(1,1,5);

	initNetwork();

	cleanBuff();

	TM_HD44780_PutCustom(2,1,5);
		TM_HD44780_PutCustom(3,1,5);
		TM_HD44780_PutCustom(4,1,5);
		TM_HD44780_PutCustom(5,1,5);

	getHTTP(getPoznan);
	strncpy(overviewPO, parseJson("\"weather"), 15);
	strncpy(temperaturePO, parseJson("temp_c"), 4);
	strncpy(humidityPO, parseJson("relative_humidity"), 5);
	strncpy(wind_kphPO, parseJson("wind_kph"), 3);
	strncpy(pressurePO, parseJson("pressure_mb"), 5);



	cleanBuff();

	TM_HD44780_PutCustom(6,1,5);
		TM_HD44780_PutCustom(7,1,5);
		TM_HD44780_PutCustom(8,1,5);
		TM_HD44780_PutCustom(9,1,5);

	getHTTP(getWarszawa);
	strncpy(overviewWA, parseJson("\"weather"), 15);
	strncpy(temperatureWA, parseJson("temp_c"), 4);
	strncpy(humidityWA, parseJson("relative_humidity"), 5);
	strncpy(wind_kphWA, parseJson("wind_kph"), 3);
	strncpy(pressureWA, parseJson("pressure_mb"), 5);



	cleanBuff();

	TM_HD44780_PutCustom(10,1,5);
	TM_HD44780_PutCustom(11,1,5);
	TM_HD44780_PutCustom(12,1,5);
	TM_HD44780_PutCustom(13,1,5);

	getHTTP(getKrakow);
	strncpy(overviewKK, parseJson("\"weather"), 15);
	strncpy(temperatureKK, parseJson("temp_c"), 4);
	strncpy(humidityKK, parseJson("relative_humidity"), 5);
	strncpy(wind_kphKK, parseJson("wind_kph"), 3);
	strncpy(pressureKK, parseJson("pressure_mb"), 5);



	TM_HD44780_Clear();
	TM_HD44780_Puts(0, 0, "Ukonczone");
	int o=0;
	for(o;o<=15;o++) {TM_HD44780_PutCustom(o,1,5);}
	//TM_HD44780_Puts(0, 0, "STM32F4");
	//TM_HD44780_Puts(0, 1, "20x4 HD44780 LCD");
	//Delayms(3000);
	//TM_HD44780_Clear();
	//TM_HD44780_Puts(0, 0, "Michal Gozdek");
	//TM_HD44780_Puts(0, 1, "DominikKaczmarek");


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
	TIM_TimeBaseStructure.TIM_Period = 2099;
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



	TIM_TimeBaseStructure.TIM_Period = 2099;
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
