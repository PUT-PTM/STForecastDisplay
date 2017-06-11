# STForecastDisplay

## Overview

STForecastDisplay is simple program, provide weather forecast using board STM32. It's based on cheap wireless module ESP8266 and easy to use LCD display. The program was created as open source project at Poznan University of Technology.

## Description

The base of our project is microcontroller STM32F407VG DISCOVERY, which works with LCD HD44780 display and wireless module ESP8266.    
STM32F407VG DISCOVERY handles input and output operations,and parses information gained from ESP8266. To communicate with module board uses short string commands knowns as AT commands. STM32F4 sends them via USART, and after second stores response in buffor. 
Method responsible for this also checks for proper response from ESP8266, and after too long time calls error.    
STM32F4 work can be divided into 5 parts:   
- Initialization of ESP8266.
- Link to WLAN and open communication with API .
- Parse information from buffor and display them.
- Handle interrupt from buttons.
- Refresh information every 10 minutes.

Error during every step is call by proper information on display.
- 'ESP Init Error' is called when ESP8266 is not connected properly.
- 'Connection Error' is called when ESP8266 cannot link to wlan (maybe wrong ssid, password or network is off), or cannot link to api server.
- 'Refresh Error' is called when ESP8266 cannot get information from server.

//Michał napisz coś o tym jak działa twój wyświetlacz, jak się komunikuje z płytką, co wyświetla, coś o guziczkach

## Tools

IDE: CoIDE v1.7.8    
Language: C

## How to run

Connections:     
* ESP8266 to STM32    
	TX -> C11   
	CH -> 3V    
	RST -> 3V    
	Vcc -> 3V    
	GND -> GND    
	GPIO2 & GPIO0 -> NULL     
	RX -> C10     
	
//Michał: Podłączenie wyświetlacza, opis jaki guzik za co odpowiada, jak obsługiwać urzadzenie

## How to compile

Before compile:
1) Clone project from repository
2) Type your WLAN ssid and password in file esp8266_commands.c > function void initNetwork()
3) Copy your own key from Weather Underground API in file main.c > /*http calls*/ getCity[]
4) Compile program and send to your board

If CoIDE will have problem with open the project just copy files main.c, esp8266_commands.c and esp8266_commands.h and add to existing project. Remember to add libraries from CooCox built-in base.

## Future improvements

Plans for future:
- Make refreshing data working in background.
- Add cities.

Known bugs:
- Board ignores variable tempKK, and doesn't display it. Changing its name lead to crash.

## Attributions

//Michał dodaj te biblioteki, z których korzystałeś z linkiem, opisem i creditsami

Used API: Weather Underground - Weather API    
https://www.wunderground.com/api    
Recommended to run our project.    

## License

MIT License

Copyright (c) 2017 Dominik Kaczmarek, Michał Gozdek

## Credits

Created by: Dominik Kaczmarek, Michał Gozdek

The project was conducted during the Microprocessor Lab course held by the Institute of Control and Information Engineering, Poznan University of Technology.

Supervisor: Tomasz Mańkowski
