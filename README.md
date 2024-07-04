<h1 align="center">Watch Weather Station with ESP8266 and U8g2lib</h1>


## Overview
This project is a weather station and alarm clock built using an ESP8266 microcontroller and an OLED display. 
It fetches the current weather data from an online API and displays it on the OLED screen. It also includes an alarm function and various timers.
<img src="https://github.com/rhshourav/Watch-WeatherStation_Esp822-U8g2lib/blob/main/Img/img_1.png">
## Hardware Requirements
- Used Hardware:
  - ESP8266 Board (e.g., NodeMCU)
  - OLED Display (128x64, I2C)
  - LEDs
  - Joystick
  - Breadboard and Jumper Wires
- Software Requirements
  - Arduino IDE
- Libraries:
    - ESP8266WiFi
    - Wire
    - U8g2lib
    - NTPClient
    - ArduinoJson
    - ESP8266HTTPClient
      
## Installation
- Step 1: Install Arduino IDE
Download and install the Arduino IDE from the official Arduino website.
- Step 2: Install ESP8266 Board Package
    - Open Arduino IDE. <img src="https://github.com/rhshourav/Watch-WeatherStation_Esp822-U8g2lib/blob/main/Img/img_2.png">
    - Go to File > Preferences.
    - In the Additional Board Manager URLs field, add the following URL: ```http://arduino.esp8266.com/stable/package_esp8266com_index.json```
    - Go to Tools > Board > Boards Manager.
    - Search for esp8266 and install the esp8266 package.
- Step 3: Install Required Libraries
    - Go to Sketch > Include Library > Manage Libraries.
-Search for and install the following libraries:
```      
ESP8266WiFi
Wire
U8g2
NTPClient
ArduinoJson
ESP8266HTTPClient
```
- Step 4: Clone the Repository
Clone the project repository from GitHub:
```
git clone https://github.com/rhshourav/Watch-WeatherStation_Esp822-U8g2lib
```
Open the MAIN.ino file in the Arduino IDE.


## Setup
- Wiring
- Connect the OLED display to the ESP8266 as follows:
```
OLED GND to ESP8266 GND
OLED VCC to ESP8266 3.3V
OLED SCL to ESP8266 D1 (GPIO 5)
OLED SDA to ESP8266 D2 (GPIO 4)
```
Connect buttons and LEDs as per your design.
```
SELECT Button TO ESP8266 GPIO 5
UP Button TO ESP8266 GPIO 16
DOWN Button TO ESP8266 GPIO 13
LEFT Button TO ESP8266 GPIO 15
RIGHT Button TO ESP8266 GPIO 4
EXIT Button TO ESP8266 GPIO 0
```
Configure WiFi Credentials
In the MAIN.ino file, update the WiFi credentials:
```cpp


char ssid[] = "YOUR_SSID";
char pass[] = "YOUR_PASSWORD";
```
Add your API & Location:
```cpp
String getWeatherData() {
String url = "http://api.openweathermap.org/data/2.5/weather?q=***YOUR LOCATION***&units=metric&appid=**********YOUR*API****";
}
```
Hear you can hardcode your alarms:
```cpp
/*************************************************************
Alarm Related Variables;
**************************************************************/
bool alarmStateMain = true; // set this to true to turn on alarm function.
const int alarmLedDelay = 300;
//For Alram 1 plese set alarm1 = HIGH and set time as 24 format 
const char* alarmName1 ="SLEEP";
const int alarm1 = HIGH; // LOW = OFf and HIGH = ON.
const int aHt1 = 13; //set your Hour (24 hour format);
const int aMt1 = 30; // set your  desired Minutes;
const int aSt1 = 5; // set your desired on time in seconds for LED state 
//For Alram 2 plese set alarm1 = HIGH and set time as 24 format 
const char* alarmName2 ="Alarm 2";
const int alarm2 = LOW; // LOW = OFf and HIGH = ON.
const int aHt2 = 0; //set your Hour (24 hour format);
const int aMt2 = 0; // set your  desired Minutes;
const int aSt2 = 5; // set your desired on time in seconds for LED state 
//For Alram 3 plese set alarm1 = HIGH and set time as 24 formate 
const char* alarmName3 ="Alarm 3";
const int alarm3 = LOW; // LOW = OFf and HIGH = ON.
const int aHt3 = 0; //set your Hour (24 hour format);
const int aMt3 =0; // set your  desired Minutes;
const int aSt3 = 5; // set your desired on time in seconds for LED state 
//For Alram 4 plese set alarm1 = HIGH and set time as 24 format 
const char* alarmName4 ="Alarm 4";
const int alarm4 = LOW; // LOW = OFf and HIGH = ON.
const int aHt4 = 0; //set your Hour (24 hour format);
const int aMt4 =0; // set your  desired Minutes;
const int aSt4 = 5; // set your desired on time in seconds for LED state 
```
### Upload the Code
- Select the correct board and port:
- Go to Tools > Board and select Genric ESP8266 Module.
- Go to Tools > Port and select the appropriate COM port.
- Upload the code to the ESP8266 by clicking the right arrow button in the toolbar or using Sketch > Upload.

  
## Usage
Once the code is uploaded and the ESP8266 is connected to WiFi, the OLED display will show the current weather data and time. Use the buttons to navigate through the menu and set the alarms or timers.
Menu Options:
- Weather: Displays the current weather information.
- Timer: Allows setting a countdown timer.
- Stop Watch: A simple stopwatch function.
- Alarms
- Alarms can be set by modifying the following variables in the code:
```cpp
const int aHt1 = 13; // Hour (24-hour format)
const int aMt1 = 30; // Minute
const int aSt1 = 5;  // Alarm duration in seconds
```


## Troubleshooting
Ensure that all connections are correct and secure.
Verify that the WiFi credentials are correct.
Check the serial monitor for any error messages.

## License
This project is licensed under the MIT License.
## Acknowledgements
- [OpenWeatherMap API](https://openweathermap.org/api)
- [U8g2 Library](https://github.com/olikraus/u8g2)
