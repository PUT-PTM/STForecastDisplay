/* ESP inputs */
//	---------------------------
// |TX 	> C11	|GND	> GND  |
// |CH 	> 3V	|GPIO2	> Null |
// |RST > 3V	|GPIO0	> Null |
// |Vcc > 3v	|RX		> C10  |
//	---------------------------

/* http get calls */
extern char getPoznan[94];
extern char getWarszawa[94];
extern char getKrakow[94];

/* buffer[count] to store get output */
extern char buffor[4096];
extern count;

/* containers for weather info */
extern char temperaturePO[4];
extern char overviewPO[15];
extern char humidityPO[4];
extern char windPO[3];
extern char pressurePO[5];

extern char temperatureWA[4];
extern char overviewWA[15];
extern char humidityWA[4];
extern char windWA[3];
extern char pressureWA[5];

extern char tempKK[4];
extern char overviewKK[15];
extern char humidityKK[4];
extern char wind_kphKK[3];
extern char pressureKK[5];


/* find toFind scheme and its value */
/* input: 	value you looking for : toFind
 * return: 	pointer to array of chars with value
 * 		   	if failed sends NULL
 */
	char *parseJson(char *toFind);

/* clear buffer from buffor[0] to buffor[count] */
/* return: 	buffor filled with ' '
 */
	void cleanBuff();

/* look for 'mess' segment in buffor */
/* input:	string you looking for : mess
 * return:	1 if find
 * 			-1 if not
 */
	int findOK(char *mess);

/* send AT command to ESP8266 */
/* input:	string you want to send : command
 *			expected answer : recv
 * return:	1 if send command and get expected answer
 * 			-1 if failed
 */
	int sendCommand(char *command, char *recv);

/* init esp8266 */
/* sends "AT+CWMODE" > choose AP+CLIENT mode
 * sends "AT+RST" > reset ESP8266 to apply changes
 * return:	1 if initialization is successful
 * 			-1 if failed
 */
	int initAT();

/* configure network */
/* sends "AT+CWJAP" > connect to WLAN
 * sends "AT+CIPSTART" > open communication with api server
 * return:	1 if operations are successful
 * 			-1 if failed
 */
	int initNetwork();

/* send http get request */
/* sends HTTP get request to api server
 * input:	get request header for particular city
 * return:	1 if gets answer
 * 			-1 if failed
 */
	int getHTTP(char *getRequest);

/* refresh forecast info */
	int refreshInfo();
