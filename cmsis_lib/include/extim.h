/* containers for weather info */
extern char temperaturePO[4];
extern char overviewPO[15];
extern char humidityPO[4];
extern char windPO[3];
extern char pressurePO[5];

extern char temperWA[4];
extern char overviewWA[15];
extern char humidityWA[4];
extern char windWA[3];
extern char pressureWA[5];

extern char tempKK[4];
extern char overviewKK[15];
extern char humidityKK[4];
extern char wind_kphKK[3];
extern char pressuKK[5];

extern char teWR[4];
extern char overWR[15];
extern char humidityWR[4];
extern char wiatr[3];
extern char pressureWR[5];

extern char tempGD[4];
extern char overviewGD[15];
extern char humidityGD[4];
extern char windGD[3];
extern char pressureGD[5];

extern int button1;
extern int button2;
extern int refresh_flag;

//exti buuton1 - cities
void EXTI1_IRQHandler(void)
 {
          	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
          	{
          		TIM_Cmd(TIM2, ENABLE);
          		EXTI_ClearITPendingBit(EXTI_Line1);
    	   	}
 }

//exti button2 - weather
 void EXTI2_IRQHandler(void)
 {
          	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
          	{
          		TIM_Cmd(TIM2, ENABLE);
          		EXTI_ClearITPendingBit(EXTI_Line2);
    	   	}
 }

//buttons timer
 void TIM2_IRQHandler(void)
 {
 		 if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
 			 if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) != RESET){
 			 TIM_Cmd(TIM3, DISABLE);
 			 TIM_SetCounter(TIM3, 0);
 		     TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
 			 button1++;
 			 if(button1>4)button1=0;
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
 							TM_HD44780_Puts(6, 1, windPO);
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
 							TM_HD44780_Puts(6, 1, temperWA);
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
 							TM_HD44780_Puts(6, 1, windWA);
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
 							TM_HD44780_Puts(6, 1, tempKK);
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
 							TM_HD44780_Puts(7, 1, pressuKK);
 							TM_HD44780_Puts(12, 1, "hpa");
 							break;
 						default:
 							break;
 							}
 							}
 				 else if(button1==3){
 					switch (button2) {
 						case 0:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Wroclaw");
 							TM_HD44780_PutCustom(0,1,0);
 							TM_HD44780_Puts(1, 1, "temp");
 							TM_HD44780_Puts(6, 1, teWR);
 							TM_HD44780_PutCustom(10,1,4);
 							TM_HD44780_Puts(11, 1, "C");
 							break;
 						case 1:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Wroclaw");
 							TM_HD44780_Puts(0, 1, overWR);
 							break;
 						case 2:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Wroclaw");
 							TM_HD44780_PutCustom(0,1,3);
 							TM_HD44780_Puts(1, 1, "humidity");
 							TM_HD44780_Puts(10, 1, humidityWR);
 							break;
 						case 3:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Wroclaw");
 							TM_HD44780_PutCustom(0,1,1);
 							TM_HD44780_Puts(1, 1, "wind");
 							TM_HD44780_Puts(6, 1, wiatr);
 							TM_HD44780_Puts(9, 1, "km/h");
 							break;
 						case 4:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Wroclaw");
 							TM_HD44780_PutCustom(0,1,2);
 							TM_HD44780_Puts(1, 1, "press");
 							TM_HD44780_Puts(7, 1, pressureWR);
 							TM_HD44780_Puts(12, 1, "hpa");
 							break;
 						default:
 							break;
 							}
 							}
 				 else if(button1==4){
 					switch (button2) {
 						case 0:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Gdansk");
 							TM_HD44780_PutCustom(0,1,0);
 							TM_HD44780_Puts(1, 1, "temp");
 							TM_HD44780_Puts(6, 1, tempGD);
 							TM_HD44780_PutCustom(10,1,4);
 							TM_HD44780_Puts(11, 1, "C");
 							break;
 						case 1:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Gdansk");
 							TM_HD44780_Puts(0, 1, overviewGD);
 							break;
 						case 2:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Gdansk");
 							TM_HD44780_PutCustom(0,1,3);
 							TM_HD44780_Puts(1, 1, "humidity");
 							TM_HD44780_Puts(10, 1, humidityGD);
 							break;
 						case 3:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Gdansk");
 							TM_HD44780_PutCustom(0,1,1);
 							TM_HD44780_Puts(1, 1, "wind");
 							TM_HD44780_Puts(6, 1, windGD);
 							TM_HD44780_Puts(9, 1, "km/h");
 							break;
 						case 4:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Gdansk");
 							TM_HD44780_PutCustom(0,1,2);
 							TM_HD44780_Puts(1, 1, "press");
 							TM_HD44780_Puts(7, 1, pressureGD);
 							TM_HD44780_Puts(12, 1, "hpa");
 							break;
 						default:
 							break;
 							}
 							}
 			 }
 			 else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2) != RESET){
 			 TIM_Cmd(TIM3, DISABLE);
 		     TIM_SetCounter(TIM3, 0);
 			 TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
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
 						TM_HD44780_Puts(6, 1, temperWA);
 						TM_HD44780_PutCustom(10,1,4);
 						TM_HD44780_Puts(11, 1, "C");
 						break;
 					case 2:
 						TM_HD44780_Clear();
 						TM_HD44780_Puts(0, 0, "Krakow");
 						TM_HD44780_PutCustom(0,1,0);
 						TM_HD44780_Puts(1, 1, "temp");
 						TM_HD44780_Puts(6, 1, tempKK);
 						TM_HD44780_PutCustom(10,1,4);
 						TM_HD44780_Puts(11, 1, "C");
 						break;
 					case 3:
 						TM_HD44780_Clear();
 						TM_HD44780_Puts(0, 0, "Wroclaw");
 						TM_HD44780_PutCustom(0,1,0);
 						TM_HD44780_Puts(1, 1, "temp");
 						TM_HD44780_Puts(6, 1, teWR);
 						TM_HD44780_PutCustom(10,1,4);
 						TM_HD44780_Puts(11, 1, "C");
 						break;
 					case 4:
 						TM_HD44780_Clear();
 						TM_HD44780_Puts(0, 0, "Gdansk");
 						TM_HD44780_PutCustom(0,1,0);
 						TM_HD44780_Puts(1, 1, "temp");
 						TM_HD44780_Puts(6, 1, tempGD);
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
 						case 3:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Wroclaw");
 							TM_HD44780_Puts(0, 1, overWR);
 							break;
 						case 4:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Gdansk");
 							TM_HD44780_Puts(0, 1, overviewGD);
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
 						case 3:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Wroclaw");
 							TM_HD44780_PutCustom(0,1,3);
 							TM_HD44780_Puts(1, 1, "humidity");
 							TM_HD44780_Puts(10, 1, humidityWR);
 							break;
 						case 4:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Gdansk");
 							TM_HD44780_PutCustom(0,1,3);
 							TM_HD44780_Puts(1, 1, "humidity");
 							TM_HD44780_Puts(10, 1, humidityGD);
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
 							TM_HD44780_Puts(6, 1, windPO);
 							TM_HD44780_Puts(9, 1, "km/h");
 							break;
 						case 1:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Warszawa");
 							TM_HD44780_PutCustom(0,1,1);
 							TM_HD44780_Puts(1, 1, "wind");
 							TM_HD44780_Puts(6, 1, windWA);
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
 						case 3:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Wroclaw");
 							TM_HD44780_PutCustom(0,1,1);
 							TM_HD44780_Puts(1, 1, "wind");
 							TM_HD44780_Puts(6, 1, wiatr);
 							TM_HD44780_Puts(9, 1, "km/h");
 							break;
 						case 4:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Gdansk");
 							TM_HD44780_PutCustom(0,1,1);
 							TM_HD44780_Puts(1, 1, "wind");
 							TM_HD44780_Puts(6, 1, windGD);
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
 							TM_HD44780_Puts(7, 1, pressuKK);
 							TM_HD44780_Puts(12, 1, "hpa");
 							break;
 						case 3:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Wroclaw");
 							TM_HD44780_PutCustom(0,1,2);
 							TM_HD44780_Puts(1, 1, "press");
 							TM_HD44780_Puts(7, 1, pressureWR);
 							TM_HD44780_Puts(12, 1, "hpa");
 							break;
 						case 4:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Gdansk");
 							TM_HD44780_PutCustom(0,1,2);
 							TM_HD44780_Puts(1, 1, "press");
 							TM_HD44780_Puts(7, 1, pressureGD);
 							TM_HD44780_Puts(12, 1, "hpa");
 							break;
 						default:
 							break;
 							}
 							}
 			 }
 			 TIM_Cmd(TIM2, DISABLE);
 			 TIM_SetCounter(TIM2, 0);
 			 TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

 			 TIM_Cmd(TIM3, ENABLE);
 			}
 }



//toggle timer
 void TIM3_IRQHandler(void)
 {
 		 if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
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
 						TM_HD44780_Puts(6, 1, temperWA);
 						TM_HD44780_PutCustom(10,1,4);
 						TM_HD44780_Puts(11, 1, "C");
 						break;
 					case 2:
 						TM_HD44780_Clear();
 						TM_HD44780_Puts(0, 0, "Krakow");
 						TM_HD44780_PutCustom(0,1,0);
 						TM_HD44780_Puts(1, 1, "temp");
 						TM_HD44780_Puts(6, 1, tempKK);
 						TM_HD44780_PutCustom(10,1,4);
 						TM_HD44780_Puts(11, 1, "C");
 						break;
 					case 3:
 						TM_HD44780_Clear();
 						TM_HD44780_Puts(0, 0, "Wroclaw");
 						TM_HD44780_PutCustom(0,1,0);
 						TM_HD44780_Puts(1, 1, "temp");
 						TM_HD44780_Puts(6, 1, teWR);
 						TM_HD44780_PutCustom(10,1,4);
 						TM_HD44780_Puts(11, 1, "C");
 						break;
 					case 4:
 						TM_HD44780_Clear();
 						TM_HD44780_Puts(0, 0, "Gdansk");
 						TM_HD44780_PutCustom(0,1,0);
 						TM_HD44780_Puts(1, 1, "temp");
 						TM_HD44780_Puts(6, 1, tempGD);
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
 						case 3:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Wroclaw");
 							TM_HD44780_Puts(0, 1, overWR);
 							break;
 						case 4:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Gdansk");
 							TM_HD44780_Puts(0, 1, overviewGD);
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
 						case 3:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Wroclaw");
 							TM_HD44780_PutCustom(0,1,3);
 							TM_HD44780_Puts(1, 1, "humidity");
 							TM_HD44780_Puts(10, 1, humidityWR);
 							break;
 						case 4:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Gdansk");
 							TM_HD44780_PutCustom(0,1,3);
 							TM_HD44780_Puts(1, 1, "humidity");
 							TM_HD44780_Puts(10, 1, humidityGD);
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
 							TM_HD44780_Puts(6, 1, windPO);
 							TM_HD44780_Puts(9, 1, "km/h");
 							break;
 						case 1:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Warszawa");
 							TM_HD44780_PutCustom(0,1,1);
 							TM_HD44780_Puts(1, 1, "wind");
 							TM_HD44780_Puts(6, 1, windWA);
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
 						case 3:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Wroclaw");
 							TM_HD44780_PutCustom(0,1,1);
 							TM_HD44780_Puts(1, 1, "wind");
 							TM_HD44780_Puts(6, 1, wiatr);
 							TM_HD44780_Puts(9, 1, "km/h");
 							break;
 						case 4:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Gdansk");
 							TM_HD44780_PutCustom(0,1,1);
 							TM_HD44780_Puts(1, 1, "wind");
 							TM_HD44780_Puts(6, 1, windGD);
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
 							TM_HD44780_Puts(7, 1, pressuKK);
 							TM_HD44780_Puts(12, 1, "hpa");
 							break;
 						case 3:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Wroclaw");
 							TM_HD44780_PutCustom(0,1,2);
 							TM_HD44780_Puts(1, 1, "press");
 							TM_HD44780_Puts(7, 1, pressureWR);
 							TM_HD44780_Puts(12, 1, "hpa");
 							break;
 						case 4:
 							TM_HD44780_Clear();
 							TM_HD44780_Puts(0, 0, "Gdansk");
 							TM_HD44780_PutCustom(0,1,2);
 							TM_HD44780_Puts(1, 1, "press");
 							TM_HD44780_Puts(7, 1, pressureGD);
 							TM_HD44780_Puts(12, 1, "hpa");
 							break;
 						default:
 							break;
 							}
 							}

 		  TIM_SetCounter(TIM3, 0);
 		  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
 		 }
 }

 //exti button3 - refreshing
 void EXTI3_IRQHandler(void)
 {
          	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
          	{
          		TIM_Cmd(TIM4, ENABLE);
          		EXTI_ClearITPendingBit(EXTI_Line3);
    	   	}
 }

//refreshing button
 void TIM4_IRQHandler(void)
 {
 		 if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){
 			 if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3) != RESET){
 			 refresh_flag = 1;

 		 }
 		  TIM_Cmd(TIM4, DISABLE);
 		  TIM_SetCounter(TIM4, 0);
 		  TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
 		 }
 }

 //auto-refreshing timer
 void TIM5_IRQHandler(void)
 {
 		 if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET){
 			  refresh_flag = 1;
 		  	  TIM_SetCounter(TIM5, 0);
 		  	  TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
 		 }
 }
