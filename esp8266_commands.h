/* ESP inputs */
//	---------------------------
// |TX 	> C11	|GND	> GND  |
// |CH 	> 3V	|GPIO2	> Null |
// |RST > 3V	|GPIO0	> Null |
// |Vcc > 3v	|RX		> C10  |
//	---------------------------

/* buffer[count] to store get output */
extern char buffor[4096];
extern count;

/* containers for weather info */
extern char temperaturePO[4];
extern char overviewPO[15];
extern char humidityPO[4];
extern char wind_kphPO[3];
extern char pressurePO[5];

extern char temperatureWA[4];
extern char overviewWA[15];
extern char humidityWA[4];
extern char wind_kphWA[3];
extern char pressureWA[5];

extern char temperatureKK[4];
extern char overviewKK[15];
extern char humidityKK[4];
extern char wind_kphKK[3];
extern char pressureKK[5];


/* find toFind scheme and its value */
/* input: 	array of chars
 * output: 	array of chars with value
 * 		   	if failed sends NULL
 */
	char *parseJson(char *toFind);

/* clear buffer */
	void cleanBuff();

/* find OK in buffor */
	int findOK(char *recv);

/* send AT command */
	int sendCommand(char *command, char *recv);

/* init esp8266 */
	void initAT();

/* configure network */
	int initNetwork();

/* send http get request */
	int getHTTP(char *getRequest);
