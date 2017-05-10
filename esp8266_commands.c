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

/* send AT command */
int sendCommand(char *command)
{
	cleanBuff();
	int wait = 250;
	SendString(command);
	while((buffor[2] != 'O') && (buffor[3] != 'K')){
		Delayms(wait);
		wait += 250;
		if(wait > 10000) return -1;
	}
	return 1;
}

/* init esp8266 */
void initAT()
{
	// disable echo
	SendString("ATE0\r\n");
	Delayms(500);

	// esp client+AP mode enabled
	sendCommand("AT+CWMODE=3\r\n");

	// esp reset command
	sendCommand("AT+RST\r\n");
}

/* configure network */
void initNetwork()
{
	// connecting esp to network
	sendCommand("AT+CWJAP=\"networktes\",\"myesp8266\"\r\n");

	// connect to wunderground.com
	sendCommand("AT+CIPSTART=\"TCP\",\"api.wunderground.com\",80\r\n");

}

/* send http get request */
int getHTTP(char *getRequest)
{
	/* send request */
	SendString("AT+CIPSEND=93\r\n");
	Delayms(1000);
	SendString(getRequest);
	Delayms(6000);
	SendString("+IPD,150:\r\n");
	Delayms(5000);
	if(count < 3000) return -1;
	else return 1;
}

