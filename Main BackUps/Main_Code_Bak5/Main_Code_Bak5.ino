
#define BLYNK_TEMPLATE_ID "TMPL6LhiJp-GK"
#define BLYNK_TEMPLATE_NAME "LED ESP32"
#define BLYNK_AUTH_TOKEN "tEn8QiVQFJlK_GplVlUCYrjNlSyiwX-E"
#define BLYNK_DEVICE_NAME "MY ESP32"

////////////////////////////////////////////
//Changeable Variables/////////////////////
// Your WiFi credentials. 
char ssid[] = "ON";
char pass[] = "test1234";
const int s_time = 100; //10000; --> for rufly 10 minutes. Change this to increase or decrease Standby time
const int d_time = 60; // time is in mili seconds the value is s_time X d_time (D_time == Delay time) Must be less then 100



bool fetch_blynk_state = true;  //true or false

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <WebServer.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <time.h>
#include <NTPClient.h>


//Initing Webserver
WebServer server(80);

//Initing Display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define NUMFLAKES     10 
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// define the GPIO connected with Relays and switches
#define RelayPin1 25 //23 //32 //D32
#define RelayPin2 33 //33 //D33
#define RelayPin3 32 //23 //D23
#define RelayPin4 23 //25 //D19
// Relay pins
int numRelays = 5;
//LED LIGHT
#define LightPin 4 //D27
#define blynk_indicator   2   //D2


//Define IR input Code
#define IR_IN 19 //D18
#define PU_IN 18 //D19

//Change the virtual pins according the rooms
#define VPIN_BUTTON_1    V1 
#define VPIN_BUTTON_2    V2
#define VPIN_BUTTON_3    V3 
#define VPIN_BUTTON_4    V4
#define VPIN_BUTTON_5    V5

// Relay State integer to remember the toggle state for relay 1
bool toggleState_1 = LOW; 
bool toggleState_2 = LOW; 
bool toggleState_3 = LOW;
bool toggleState_4 = LOW;
bool toggleState_5 = LOW;
//#define NUM_RELAYS 5 // Define the number of relays
int relayStatus1 = LOW; // Initial status of relay 1
int relayStatus2 = LOW; // Initial status of relay 2
int relayStatus3 = LOW; // Initial status of relay 3
int relayStatus4 = LOW; // Initial status of relay 4
int relayStatus5 = LOW; // Initial status of relay 5
//for start ship animation

int hCenter = 64;                      // horizontal center of animation
int vCenter = 32;                      // vertical center of animation
int Hstar, Vstar;                      // star location currently
int Quantity = 50;                     // number of stars
int StarProgress[50];                  // array that tracks progress of each star
int StarAngle[50] = {};                // array that tracks angle of each star
int interval[75] = {};                 // array used to track speed of each star

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "bd.pool.ntp.org");  // NTP server in Bangladesh


int is_on1 ;
int is_on2 ;
int is_on3 ;
int c_time ;
int wifiFlag = 0;

char auth[] = "tEn8QiVQFJlK_GplVlUCYrjNlSyiwX-E";


String getCurrentTime() {
  // Get current time in 12-hour format
  int h = timeClient.getHours();
  int m = timeClient.getMinutes();
  String period = (h < 12) ? "AM" : "PM";
  if (h == 0) {
    h = 12;  // 12 AM
  } else if (h > 12) {
    h = h - 12;
  }
  String timeString = (h < 10 ? "0" : "")+String(h) + ":" + (m < 10 ? "0" : "") + String(m);
  return timeString;
}
String getCurrentWeekday() {
  // Get current epoch time
  unsigned long epochTime = timeClient.getEpochTime();

  // Convert epoch time to struct tm
  struct tm *timeInfo;
  time_t rawtime = (time_t)epochTime;
  timeInfo = localtime(&rawtime);

  // Format the weekday
  char formattedWeekday[20];
  strftime(formattedWeekday, 20, "%A", timeInfo);

  return String(formattedWeekday);
}
String getCurrentSeconds() {
  // Get current seconds
  int s = timeClient.getSeconds();
  return (s < 10 ? "0" : "") + String(s);
}

String getCurrentPeriod() {
  // Get current AM/PM period
  int h = timeClient.getHours();
  return (h < 12) ? "AM" : "PM";
}
String getCurrentDate() {
  // Get current epoch time
  unsigned long epochTime = timeClient.getEpochTime();

  // Convert epoch time to struct tm
  struct tm *timeInfo;
  time_t rawtime = (time_t)epochTime;
  timeInfo = localtime(&rawtime);

  // Format the date
  char formattedDate[20];
  strftime(formattedDate, 20, "%d-%m-%Y", timeInfo);

  return String(formattedDate);
}

BlynkTimer timer;

void checkBlynkStatus() { // called every 2 seconds by SimpleTimer

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
    //Serial.println("Blynk Not Connected");
    digitalWrite(blynk_indicator, LOW);
  }
  if (isconnected == true) {
    wifiFlag = 0;
    if (!fetch_blynk_state){
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
    Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
    Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
    Blynk.virtualWrite(VPIN_BUTTON_5, toggleState_5);
    }
    digitalWrite(blynk_indicator, HIGH);
  }
}

BLYNK_CONNECTED() {
  // Request the latest state from the server
  if (fetch_blynk_state){
    Blynk.syncVirtual(VPIN_BUTTON_1);
    Blynk.syncVirtual(VPIN_BUTTON_2);
    Blynk.syncVirtual(VPIN_BUTTON_3);
    Blynk.syncVirtual(VPIN_BUTTON_4);
    Blynk.syncVirtual(VPIN_BUTTON_5);
  }
}

BLYNK_WRITE(V1){
  int value = param.asInt();
  c_time = 0;
  if (value == 1) {
    Serial.print("Relay 1 =");
    Serial.println(value);
    relayStatus1 = HIGH;
    res();
    cle();
    display.setTextSize(2);
    display.println("Relay 1:");
    display.setTextSize(3);
    display.setCursor(35,23);
    display.println("ON");
    display.display();
    delay(500);
  } else {
    Serial.print("Relay 1 = ");
    Serial.println(value);
    relayStatus1 = LOW;
    res();
    cle();
    display.setTextSize(2);
    display.println("Relay 1:");
    display.setTextSize(3);
    display.setCursor(35,23);
    display.println("OFF");
    display.display();
    delay(500);
  }
}

BLYNK_WRITE(V2){
  
  int value = param.asInt();
  c_time = 0;
  if (value == 1) {
    Serial.print("Relay 2 = ");
    Serial.println(value);
    relayStatus2 = HIGH;
    res();
    cle();
    display.setTextSize(2);
    display.println("Relay 2:");
    display.setTextSize(3);
    display.setCursor(35,23);
    display.println("ON");
    display.display();
    delay(500);
  } else {
    Serial.print("Relay 2 = ");
    Serial.println(value);
    relayStatus2 = LOW;
    res();
    cle();
    display.setTextSize(2);
    display.println("Relay 2:");
    display.setTextSize(3);
    display.setCursor(35,23);
    display.println("OFF");
    display.display();
    delay(500);
  }
}

BLYNK_WRITE(V3){
  
  int value = param.asInt();
  c_time = 0;
  if (value == 1) {
    Serial.print("Relay 3 = ");
    Serial.println(value);
    relayStatus3 = HIGH;
    res();
    cle();
    display.setTextSize(2);
    display.println("Relay 3:");
    display.setTextSize(3);
    display.setCursor(35,23);
    display.println("ON");
    display.display();
    delay(500);
  } else {
    Serial.print("Relay 3 = ");
    Serial.println(value);
    relayStatus3 = LOW;
    res();
    cle();
    display.setTextSize(2);
    display.println("Relay 3:");
    display.setTextSize(3);
    display.setCursor(35,23);
    display.println("OFF");
    display.display();
    delay(500);
  }
}

BLYNK_WRITE(V4){
  
  int value = param.asInt();
  c_time = 0;
  if (value == 1) {
    Serial.print("Relay 4 = ");
    Serial.println(value); 
    relayStatus4 = HIGH;
    res();
    cle();
    display.setTextSize(2);
    display.println("Relay 4:");
    display.setTextSize(3);
    display.setCursor(35,23);
    display.println("ON");
    display.display();
    delay(500);
  } else {
    Serial.print("Relay 4 = ");
    Serial.println(value);
    relayStatus4 = LOW;
    res();
    cle();
    display.setTextSize(2);
    display.println("Relay 4:");
    display.setTextSize(3);
    display.setCursor(35,23);
    display.println("OFF");
    display.display();
    delay(500);
  }
}
BLYNK_WRITE(V5){
  
  int value = param.asInt();
  c_time = 0;
  if (value == 1) {
    Serial.print("Light = ");
    Serial.println(value);
    relayStatus5 = HIGH;
    res();
    cle();
    display.setTextSize(2);
    display.println("Light:");
    display.setTextSize(3);
    display.setCursor(35,23);
    display.println("ON");
    display.display();
    delay(500);
  } else {
    Serial.print("Light = ");
    Serial.println(value);
    relayStatus5 = LOW;
    res();
    cle();
    display.setTextSize(2);
    display.println("Light:");
    display.setTextSize(3);
    display.setCursor(35,23);
    display.println("OFF");
    display.display();
    delay(500);
  }
}

const char* html = R"=====(

<!DOCTYPE html>
<html>
<head>
  <style>
    /* Your provided CSS styles */
    body {
      background-color: #222;
      color: #fff;
      font-family: Arial, sans-serif;
      text-align: center;
    }

    h2 {
      color: #00f7ff;
    }

    .button-container {
      display: flex;
      flex-direction: column;
      justify-content: center;
      align-items: center;
      margin-top: 20px;
    }

    .button {
      width: 80px;
      height: 80px;
      border-radius: 50%;
      color: #fff;
      margin: 10px;
      display: flex;
      flex-direction: column;
      justify-content: center;
      align-items: center;
      text-align: center;
      transition: background-color 0.3s;
    }

    .button:hover {
      opacity: 0.8;
    }

    .button.active {
      background-color: #0f0; /* Change to green when active */
    }

    /* New styles for each button */
    #relay1 {
      background-color: #fe0000; /* Coral color */
    }

    #relay2 {
      background-color: #ffd401; /* Sky blue color */
    }

    #relay3 {
      background-color: #29ff01; /* Dark cerulean color */
    }

    #relay4 {
      background-color: #2800ff; /* Medium turquoise color */
    }

    #relay5 {
      background-color: #02f9ff; /* Sahara color */
    }
  </style>
</head>
<body>

<h2>Relay Control</h2>

<div class="button-container">
  <div class="button" id="relay1" onclick="toggleRelay(1)">
    <div>Relay 1</div>
    <div id="relayButton1">Off</div>
  </div>
  <div class="button" id="relay2" onclick="toggleRelay(2)">
    <div>Relay 2</div>
    <div id="relayButton2">Off</div>
  </div>
  <div class="button" id="relay3" onclick="toggleRelay(3)">
    <div>Relay 3</div>
    <div id="relayButton3">Off</div>
  </div>
  <div class="button" id="relay4" onclick="toggleRelay(4)">
    <div>Relay 4</div>
    <div id="relayButton4">Off</div>
  </div>
  <div class="button" id="relay5" onclick="toggleRelay(5)">
    <div>Light</div>
    <div id="relayButton5">Off</div>
  </div>
</div>

<script>

function updateButtonStatus1(status) {
      document.getElementById("relayButton1").innerText = status;
    }
    function updateButtonStatus2(status) {
      document.getElementById("relayButton2").innerText = status;
    }
    function updateButtonStatus3(status) {
      document.getElementById("relayButton3").innerText = status;
    }
    function updateButtonStatus4(status) {
      document.getElementById("relayButton4").innerText = status;
    }
    function updateButtonStatus5(status) {
      document.getElementById("relayButton5").innerText = status;
    }
    function fetchRelayStatus() {
      fetch('/status1').then(response => response.text()).then(status => updateButtonStatus1(status));
      fetch('/status2').then(response => response.text()).then(status => updateButtonStatus2(status));
      fetch('/status3').then(response => response.text()).then(status => updateButtonStatus3(status));
      fetch('/status4').then(response => response.text()).then(status => updateButtonStatus4(status));
      fetch('/status5').then(response => response.text()).then(status => updateButtonStatus5(status));
    }
    document.getElementById("relayButton1").addEventListener("click", function() {
      fetch('/toggle1').then(fetchRelayStatus);
    });
    document.getElementById("relayButton2").addEventListener("click", function() {
      fetch('/toggle2').then(fetchRelayStatus);
    });
    document.getElementById("relayButton3").addEventListener("click", function() {
      fetch('/toggle3').then(fetchRelayStatus);
    });
    document.getElementById("relayButton4").addEventListener("click", function() {
      fetch('/toggle4').then(fetchRelayStatus);
    });
    document.getElementById("relayButton5").addEventListener("click", function() {
      fetch('/toggle5').then(fetchRelayStatus);
    });
    setInterval(fetchRelayStatus, 1000);

  </script>


</body>
</html>
)=====";

void handleRoot() {
  server.send(200, "text/html", html);
}
/////////////////////////////
//function to clear display
void cle(){
  display.clearDisplay();
  display.display();
}
/////////////////////////////////////
//function to reset display settings
void res(){
  display.setTextSize(1);
  display.setCursor(0, 0);
}

void status_anim(){
  
  // Update the display based on the relay status
  display.clearDisplay();
  
  // Draw relay 1 status
if(relayStatus1) {
  display.fillCircle(25, 20, 10, SSD1306_WHITE);
} else {
  display.drawCircle(25, 20, 10, SSD1306_WHITE);
}
  
// Draw relay 2 status
if(relayStatus2) {
  display.fillCircle(65, 20, 10, SSD1306_WHITE);
} else {
  display.drawCircle(65, 20, 10, SSD1306_WHITE);
}
  
// Draw relay 3 status
if(relayStatus3) {
  display.fillCircle(105, 20, 10, SSD1306_WHITE);
} else {
  display.drawCircle(105, 20, 10, SSD1306_WHITE);
}

// Draw relay 4 status below
if(relayStatus4) {
  display.fillCircle(48, 45, 10, SSD1306_WHITE);
} else {
  display.drawCircle(48, 45, 10, SSD1306_WHITE);
}

// Draw relay 5 status below
if(relayStatus5) {
  display.fillCircle(89, 45, 10, SSD1306_WHITE);
} else {
  display.drawCircle(89, 45, 10, SSD1306_WHITE);
}
  
  display.display();
}
//THIS WILL CONNECT TO WIFI IF DISCONNECTED
void wf_con(){
  
    while (WiFi.status() != WL_CONNECTED) {
    res();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print("Connecting To:");
    display.println(ssid);
    display.setCursor(0, 48);
    display.println("....");
    display.display();
    delay(1000);
    cle();
    Serial.println("Connecting to WiFi..");
    res();
    display.setTextSize(2);
    display.print("Connecting To:");
    display.println(ssid);
    display.setCursor(0, 48);
    display.println("........");
    display.display();
    delay(1000);
    cle();}
  
  }


//Function that will show ip:
void show_ip(){
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
  res();
  display.setTextSize(2); 
  display.println("LOCAL IP :");
  display.setCursor(0, 20);
  display.println(WiFi.localIP());
  display.display();
}

void displayTask(void *pvParameters) {
  // OLED display setup code here

  while (1) {
      
  wf_con();
  
  if (c_time <= s_time) {
    c_time  ++;
    delay(d_time);
  }else {
    if (c_time >= s_time && is_on1 <= 260){ //IS_ON1 VALUE +- 7 SECONDS 
      status_anim();
      is_on1++;

    }else if (c_time >= s_time && is_on1 >= 260 && is_on2 <= 7970){ //is_on2 value = 5 minutes
      showTime();
      is_on2++;
    }else {
      is_on1 = 0;
      is_on2 = 0;
    }
  
  }
}}

void webServerTask(void *pvParameters) {
  // WiFi and server setup code here

  while (1) {
    server.handleClient();
    Blynk.run();
  timer.run();
  server.handleClient();
    // Additional web server code here
  }
}



void showTime(){
   display.clearDisplay();
    display.fillScreen(SSD1306_BLACK); // Fill the display with black color
    display.setTextSize(3);
    display.setTextColor(SSD1306_WHITE);
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(getCurrentTime(), 0, 0, &x1, &y1, &w, &h);
    display.setCursor(8, 5);
    display.println(getCurrentTime());
    display.setTextSize(2);
    display.getTextBounds(getCurrentTime(), 0, 0, &x1, &y1, &w, &h);
    //display.setCursor((SCREEN_WIDTH - w) / 2, 30);
    display.setTextSize(1);
    display.setCursor(101, 5);
    display.println(getCurrentPeriod());
    display.setTextSize(2);
    display.setCursor(101, 14);
    display.println(getCurrentSeconds());
    display.getTextBounds(getCurrentWeekday(), 0, 0, &x1, &y1, &w, &h);
    display.setCursor((SCREEN_WIDTH - w) / 2, 30);
    display.println(getCurrentWeekday());

    display.setTextSize(2);
    display.getTextBounds(getCurrentDate(), 0, 0, &x1, &y1, &w, &h);
    display.setCursor((SCREEN_WIDTH - w) / 2, 50);
    display.println(getCurrentDate());
    display.display();


}
void setup(){
  
  digitalWrite(RelayPin1, LOW);
  digitalWrite(RelayPin2, LOW);
  digitalWrite(RelayPin3, LOW);
  digitalWrite(RelayPin4, LOW);
  //for star ship animation
  for (int i = 0; i < Quantity; i++) {          // loop to setup initial stars
    StarAngle[i] = random(360);                 // generates random angle for each star
    interval[i] = (random(5))+1;                // generates random speed for each star +1 keeps stars from being zero
  }

  Serial.begin(115200);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }


  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);
  pinMode(blynk_indicator, OUTPUT);
  pinMode(LightPin, OUTPUT);
  pinMode(IR_IN, INPUT_PULLDOWN);
  pinMode(PU_IN, INPUT_PULLDOWN);

  //During Starting all Relays should TURN OFF

   digitalWrite(blynk_indicator, LOW);
   //Showing inting thigs
  display.clearDisplay();  
  Serial.println("WELCOME TAZIN");
  display.setTextSize(3);
  display.setCursor(0, 10);
  display.setTextColor(WHITE);
  display.println("WELCOME");
  display.display();
  display.setCursor(0, 40);
  display.println(" TAZIN");
  display.display();
  delay(4000);
  cle();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED){
    res();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print("Connecting To:");
    display.println(ssid);
    display.setCursor(0, 48);
    display.println("....");
    display.display();
    delay(1000);
    cle();

    Serial.print("Connecting to WiFi..");
    Serial.println(ssid);

    res();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print("Connecting To:");
    display.println(ssid);
    display.setCursor(0, 48);
    display.println("........");
    display.display();
    delay(1000);
    cle();
  }


  show_ip();
  delay(10000);
  timer.setInterval(2000L, checkBlynkStatus); // check if Blynk server is connected every 2 seconds
  Blynk.config(auth);
  delay(1000);
  
  if (!fetch_blynk_state){
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
    Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
    Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
    Blynk.virtualWrite(VPIN_BUTTON_5, toggleState_5);
  }
   // Start the OLED display task on core 1
  xTaskCreatePinnedToCore(
    displayTask,
    "Display Task",
    10000,
    NULL,
    1,
    NULL,
    1
  );

  // Start the web server task on core 0
  xTaskCreatePinnedToCore(
    webServerTask,
    "Web Server Task",
    10000,
    NULL,
    1,
    NULL,
    0
  );


  server.on("/", HTTP_GET, handleRoot);


 
  server.on("/status1", HTTP_GET, [](){
    server.send(200, "text/plain", relayStatus1 == HIGH ? "On" : "Off");
  });
  server.on("/status2", HTTP_GET, [](){
    server.send(200, "text/plain", relayStatus2 == HIGH ? "On" : "Off");
  });
  server.on("/status3", HTTP_GET, [](){
    server.send(200, "text/plain", relayStatus3 == HIGH ? "On" : "Off");
  });
  server.on("/status4", HTTP_GET, [](){
    server.send(200, "text/plain", relayStatus4 == HIGH ? "On" : "Off");
  });
  server.on("/status5", HTTP_GET, [](){
    server.send(200, "text/plain", relayStatus5 == HIGH ? "On" : "Off");
  });

  server.on("/toggle1", HTTP_GET, [](){
    Serial.println("1");
    c_time = 0;
    if (relayStatus1 == HIGH) {
      relayStatus1 = LOW;
      Blynk.virtualWrite(V1, 0);
      res();
      cle();
      display.setTextSize(2);
      display.println("Relay 1:");
      display.setTextSize(3);
      display.setCursor(35,23);
      display.println("OFF");
      display.display();
      delay(2000);
    } else {
      relayStatus1 = HIGH;
      Blynk.virtualWrite(V1, 1);
      res();
      cle();
      display.setTextSize(2);
      display.println("Relay 1:");
      display.setTextSize(3);
      display.setCursor(35,23);
      display.println("ON");
      display.display();
      delay(2000);
    }
    server.send(200, "text/plain", relayStatus2 == HIGH ? "On" : "Off");
  });
  server.on("/toggle2", HTTP_GET, [](){
    Serial.println("2");
    c_time = 0;
    if (relayStatus2 == HIGH) {
      relayStatus2 = LOW;
      Blynk.virtualWrite(V2, 0);
      res();
      cle();
      display.setTextSize(2);
      display.println("Relay 2:");
      display.setTextSize(3);
      display.setCursor(35,23);
      display.println("OFF");
      display.display();
      delay(2000);
    } else {
      relayStatus2 = HIGH;
      Blynk.virtualWrite(V2, 1);
      res();
      cle();
      display.setTextSize(2);
      display.println("Relay 2:");
      display.setTextSize(3);
      display.setCursor(35,23);
      display.println("ON");
      display.display();
      delay(2000);
    }
    server.send(200, "text/plain", relayStatus2 == HIGH ? "On" : "Off");
  });
  server.on("/toggle3", HTTP_GET, [](){
    Serial.println("3");
    c_time = 0;
    if (relayStatus3 == HIGH) {
      relayStatus3 = LOW;
      Blynk.virtualWrite(V3, 0);
      res();
      cle();
      display.setTextSize(2);
      display.println("Relay 3:");
      display.setTextSize(3);
      display.setCursor(35,23);
      display.println("OFF");
      display.display();
      delay(2000);
    } else {
      relayStatus3 = HIGH;
      Blynk.virtualWrite(V3, 1);
      res();
      cle();
      display.setTextSize(2);
      display.println("Relay 3:");
      display.setTextSize(3);
      display.setCursor(35,23);
      display.println("ON");
      display.display();
      delay(2000);
    }
    server.send(200, "text/plain", relayStatus3 == HIGH ? "On" : "Off");
  });
  server.on("/toggle4", HTTP_GET, [](){
    Serial.println("4");
    c_time = 0;
    if (relayStatus4 == HIGH) {
      relayStatus4 = LOW;
      Blynk.virtualWrite(V4, 0);
      res();
      cle();
      display.setTextSize(2);
      display.println("Relay 4:");
      display.setTextSize(3);
      display.setCursor(35,23);
      display.println("OFF");
      display.display();
      delay(2000);
    } else {
      relayStatus4 = HIGH;
      Blynk.virtualWrite(V4, 1);
      res();
      cle();
      display.setTextSize(2);
      display.println("Relay 4:");
      display.setTextSize(3);
      display.setCursor(35,23);
      display.println("ON");
      display.display();
      delay(2000);
    }
    server.send(200, "text/plain", relayStatus4 == HIGH ? "On" : "Off");
  });
  server.on("/toggle5", HTTP_GET, [](){
    Serial.println("5");
    c_time = 0;
    if (relayStatus5 == HIGH) {
      relayStatus5 = LOW;
      Blynk.virtualWrite(V5, 0);
      res();
      cle();
      display.setTextSize(2);
      display.println("Light :");
      display.setTextSize(3);
      display.setCursor(35,23);
      display.println("OFF");
      display.display();
      delay(2000);
    } else {
      relayStatus5 = HIGH;
      Blynk.virtualWrite(V5, 1);
      res();
      cle();
      display.setTextSize(2);
      display.println("Light :");
      display.setTextSize(3);
      display.setCursor(35,23);
      display.println("ON");
      display.display();
      delay(2000);
    }
    server.send(200, "text/plain", relayStatus5 == HIGH ? "On" : "Off");
  });

  server.begin();
  Serial.println("HTTP server started");
   timeClient.begin();
  timeClient.setTimeOffset(21600);   // GMT+6:00 for Bangladesh Standard Time
  timeClient.update();
}

void loop()
{ 

  if (relayStatus1 == HIGH){
    digitalWrite(RelayPin1, HIGH);
  }else {
    digitalWrite(RelayPin1, LOW);
  }
  if (relayStatus2 == HIGH){
    digitalWrite(RelayPin2, HIGH);
  }else {
    digitalWrite(RelayPin2, LOW);
  }
  if (relayStatus3 == HIGH){
    digitalWrite(RelayPin3, HIGH);
  }else {
    digitalWrite(RelayPin3, LOW);
  }
  if (relayStatus4 == HIGH){
    digitalWrite(RelayPin4, HIGH);
  }else {
    digitalWrite(RelayPin4, LOW);
  }
  if (relayStatus5 == HIGH){
    digitalWrite(LightPin, HIGH);
  }else {
    digitalWrite(LightPin, LOW);
  }

  if (digitalRead(PU_IN) ==  HIGH){
    cle();
    show_ip();
    c_time = 0;
  }
  if (digitalRead(IR_IN) == LOW){
    relayStatus5 = !relayStatus5;
    c_time = 0;
    cle();
    if (relayStatus5 == LOW){
      Serial.println("From IR HIGH");
      Blynk.virtualWrite(V5, 0);
      res();
      cle();
      display.setTextSize(2);
      display.println("Light :");
      display.setTextSize(3);
      display.setCursor(35,23);
      display.println("OFF");
      display.display();
      delay(200);
    } else {
      Serial.println("From IR LOW");
      Blynk.virtualWrite(V5, 1);
      res();
      cle();
      display.setTextSize(2);
      display.println("Light :");
      display.setTextSize(3);
      display.setCursor(35,23);
      display.println("ON");
      display.display();
      delay(200);
    }
  }

}
