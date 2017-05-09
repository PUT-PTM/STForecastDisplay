/* ESP inputs */
//	---------------------------
// |TX 	> C11	|GND	> GND  |
// |CH 	> 3V	|GPIO2	> Null |
// |RST > 3V	|GPIO0	> Null |
// |Vcc > 3v	|RX		> C10  |
//	---------------------------

extern char buffor[4096];
extern count;

/* containers for weather info */
extern char temperature[4];
extern char overview[15];
extern char humidity[4];
extern char wind_kph[3];
extern char pressure[5];

/* find toFind scheme and its value */
/* input: 	array of chars
 * output: 	array of chars with value
 * 		   	if failed sends NULL
 */
	char *parseJson(char *toFind);

/* clear buffer */
	void cleanBuff();

/* init esp8266 */
	void initAT();

/* configure network */
	void initNetwork();

/* send http get request */
	void getHTTP();
