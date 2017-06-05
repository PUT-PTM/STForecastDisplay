#include "esp8266_commands.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_hd44780.h"

/* find toFind scheme and its value */
char *parseJson(char *toFind)
{
	char result[32] = { 0 };
	char *temp = toFind;
	int i,k = 0;
	int toFindLength = strlen(toFind);

	for(i=0; i<count; i++)
	{
		if(buffor[i] == *temp){
			temp++;
			k++;
			if(k == toFindLength){
				i += 3;
				k = 0;
				while(buffor[i] != ','){
					if(buffor[i] != '\"')
					{
						result[k] = buffor[i];
						k++;
					}
					i++;
				}
				temp = &result;
				return temp;
			}
		}
		else{
			temp = toFind;
			k = 0;
		}
	}
	return "NULL";
}

/* clear buffer */
void cleanBuff()
{
	int i;
	for(i=0; i<count; i++){
		buffor[i] = ' ';
	}
	count = 0;
}

/* find recv in buffor */
int findOK(char *mess)
{
	int i, temp = 0;
	int messLenght = strlen(mess);
	char *messStart = mess;
	for(i = 0; i<count; i++)
	{
		if(buffor[i] == *mess){
			temp++;
			mess++;
			if(messLenght == temp){
				return 1;
			}
		}
		else{
			temp = 0;
			mess = messStart;
		}

	}
	return -1;
}

/* send AT command */
int sendCommand(char *command, char *recv)
{
	cleanBuff();
	int wait = 500;
	SendString(command);
	Delayms(500);
	while(findOK(recv) != 1){
		Delayms(wait);
		wait += 500;
		if(wait > 2500) return -1;
	}
	return 1;
}

/* init esp8266 */
int initAT()
{
	// check init configuration
	int flag = 1;

	// esp client+AP mode enabled
	flag = sendCommand("AT+CWMODE=3\r\n", "OK");
	if(flag != 1) return -1;

	// esp reset command
	flag = sendCommand("AT+RST\r\n", "OK");
	if(flag != 1) return -1;
}

/* configure network */
int initNetwork()
{
	// check connection
	int flag = 1;

	// connecting esp to network
	flag = sendCommand("AT+CWJAP=\"networktes\",\"myesp8266\"\r\n", "OK");
	if(flag != 1) return -1;

	// connect to wunderground.com
	/*flag = sendCommand("AT+CIPSTART=\"TCP\",\"api.wunderground.com\",80\r\n", "OK");
	if(flag != 1) return -1;
	return 1;*/
}

/* send http get request */
int getHTTP(char *getRequest)
{
	int flag = 1;

	/* send request */
	flag = sendCommand("AT+CIPSEND=89\r\n", ">");
	if(flag != 1) return -1;

	flag = sendCommand(getRequest, "SEND OK");
	if(flag != 1) return -1;

	SendString("+IPD,150:\r\n");
	Delayms(2000);

	if(count < 3000) return -1;
	else return 1;
}

/* refresh forecast info */
int refreshInfo()
{
	int flag = 1;
	char temp[4];

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
	TM_HD44780_CreateChar(0, &customChar[0]);

	TM_HD44780_Clear();
	TM_HD44780_Puts(0, 0, "Loading");
	TM_HD44780_PutCustom(0,1,5);
	TM_HD44780_PutCustom(1,1,5);

	TM_HD44780_PutCustom(2,1,5);
	TM_HD44780_PutCustom(3,1,5);

	flag = sendCommand("AT+CIPSTART=\"TCP\",\"api.wunderground.com\",80\r\n", "OK");
	if(flag != 1) return -1;

	TM_HD44780_PutCustom(4,1,5);
	TM_HD44780_PutCustom(5,1,5);

	flag = getHTTP(getPoznan);
	if(flag != 1) return -1;
	strncpy(overviewPO, parseJson("\"weather"), 15);
	strncpy(temperaturePO, parseJson("temp_c"), 4);
	strncpy(humidityPO, parseJson("relative_humidity"), 5);
	strncpy(windPO, parseJson("wind_kph"), 3);
	strncpy(pressurePO, parseJson("pressure_mb"), 5);

	TM_HD44780_PutCustom(6,1,5);
	TM_HD44780_PutCustom(7,1,5);

	flag = getHTTP(getWarszawa);
	if(flag != 1) return -1;
	strncpy(overviewWA, parseJson("\"weather"), 15);
	strncpy(humidityWA, parseJson("relative_humidity"), 5);
	strncpy(windWA, parseJson("wind_kph"), 3);
	strncpy(pressureWA, parseJson("pressure_mb"), 5);
	strncpy(temperWA, parseJson("temp_c"), 4);

	TM_HD44780_PutCustom(8,1,5);
	TM_HD44780_PutCustom(9,1,5);

	flag = getHTTP(getKrakow);
	if(flag != 1) return -1;
	strncpy(overviewKK, parseJson("\"weather"), 15);
	strncpy(humidityKK, parseJson("relative_humidity"), 5);
	strncpy(wind_kphKK, parseJson("wind_kph"), 3);
	strncpy(pressuKK, parseJson("pressure_mb"), 5);
	strncpy(tempKK, parseJson("temp_c"), 4); //bugged
	
	TM_HD44780_PutCustom(10,1,5);
	TM_HD44780_PutCustom(11,1,5);

	flag = getHTTP(getWroclaw);
	if(flag != 1) return -1;
	strncpy(overWR, parseJson("\"weather"), 15);
	strncpy(teWR, parseJson("temp_c"), 4);
	strncpy(humidityWR, parseJson("relative_humidity"), 5);
	strncpy(wiatr, parseJson("wind_kph"), 3);
	strncpy(pressureWR, parseJson("pressure_mb"), 5);

	TM_HD44780_PutCustom(12,1,5);
	TM_HD44780_PutCustom(13,1,5);
	
	flag = getHTTP(getGdansk);
	if(flag != 1) return -1;
	strncpy(overviewGD, parseJson("\"weather"), 15);
	strncpy(tempGD, parseJson("temp_c"), 4);
	strncpy(humidityGD, parseJson("relative_humidity"), 5);
	strncpy(windGD, parseJson("wind_kph"), 3);
	strncpy(pressureGD, parseJson("pressure_mb"), 5);
	
	TM_HD44780_PutCustom(14,1,5);
	TM_HD44780_PutCustom(15,1,5);

	flag = sendCommand("AT+CIPCLOSE\r\n", "OK");
	if(flag != 1) return -1;

	TM_HD44780_Clear();
	TM_HD44780_Puts(0, 0, "Complete");
	int o=0;
	for(o;o<=15;o++) {TM_HD44780_PutCustom(o,1,5);}

	return 1;
}
