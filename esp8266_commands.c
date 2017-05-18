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

/* find OK in buffor */
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
	int wait = 250;
	SendString(command);
	Delayms(1000);
	while(findOK(recv) != 1){
		Delayms(wait);
		wait += 500;
		if(wait > 5000) return -1;
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
	sendCommand("AT+CWMODE=3\r\n", "OK");

	// esp reset command
	sendCommand("AT+RST\r\n", "OK");

}

/* configure network */
int initNetwork()
{
	// check connection
	int flag;

	// connecting esp to network
	flag = sendCommand("AT+CWJAP=\"networktes\",\"myesp8266\"\r\n", "OK");
	if(flag != 1) return -1;

	// connect to wunderground.com
	sendCommand("AT+CIPSTART=\"TCP\",\"api.wunderground.com\",80\r\n", "OK");

	return 1;
}

/* send http get request */
int getHTTP(char *getRequest)
{
	/* send request */
	sendCommand("AT+CIPSEND=89\r\n", ">");
	sendCommand(getRequest, "SEND OK");
	SendString("+IPD,150:\r\n");
	Delayms(3000);

	if(count < 3000) return -1;
	else return 1;
}

