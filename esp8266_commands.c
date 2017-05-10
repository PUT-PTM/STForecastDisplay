#include "esp8266_commands.h"
#include "tm_stm32f4_delay.h"

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

/* init esp8266 */
void initAT()
{
	// esp client+AP mode enabled
	SendString("AT+CWMODE=3\r\n");
	Delayms(500);

	// esp reset command
	SendString("AT+RST\r\n");
	Delayms(1000);

	// disable echo
	SendString("ATE0\r\n");
	Delayms(500);

}

/* configure network */
void initNetwork()
{
	// connecting esp to network
	SendString("AT+CWJAP=\"networktes\",\"myesp8266\"\r\n");
	Delayms(5000);

	// connect to wunderground.com
	SendString("AT+CIPSTART=\"TCP\",\"api.wunderground.com\",80\r\n");
	Delayms(5000);

}

/* send http get request */
void getHTTP()
{
	/* send request */
	SendString("AT+CIPSEND=93\r\n");
	Delayms(1000);
	SendString("GET /api/3d8b02539ee9b6a0/conditions/q/EPPO.json HTTP/1.1\r\nHost: api.wunderground.com\r\n\r\n");
	Delayms(6000);
	SendString("+IPD,150:\r\n");
	Delayms(3000);
}

