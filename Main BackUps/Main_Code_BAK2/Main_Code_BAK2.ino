
#define BLYNK_TEMPLATE_ID "TMPL6LhiJp-GK"
#define BLYNK_TEMPLATE_NAME "LED ESP32"
#define BLYNK_AUTH_TOKEN "tEn8QiVQFJlK_GplVlUCYrjNlSyiwX-E"
#define BLYNK_DEVICE_NAME "MY ESP32"

// Your WiFi credentials. 
// Set password to "" for open networks.
char ssid[] = "On";
char pass[] = "123456789";

bool fetch_blynk_state = true;  //true or false

//#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <WebServer.h>

//Initing Webserver
WebServer server(80);
// define the GPIO connected with Relays and switches
#define RelayPin1 23  //D23
#define RelayPin2 22  //D22
#define RelayPin3 21  //D21
#define RelayPin4 19  //D19
#define LightPin 4 //D4
#define WifiLed 19 //D19
#define wifiLed   2   //D2
// Relay pins
int numRelays = 5;
//Define IR input Code
#define IR_IN 20

//Change the virtual pins according the rooms
#define VPIN_BUTTON_1    V1 
#define VPIN_BUTTON_2    V2
#define VPIN_BUTTON_3    V3 
#define VPIN_BUTTON_4    V4

// Relay State integer to remember the toggle state for relay 1
bool toggleState_1 = LOW; 
bool toggleState_2 = LOW; 
bool toggleState_3 = LOW;
bool toggleState_4 = LOW;
#define NUM_RELAYS 5 // Define the number of relays

// Define the array of relay states
int relayState[NUM_RELAYS] = {0}; ;

int wifiFlag = 0;

char auth[] = "tEn8QiVQFJlK_GplVlUCYrjNlSyiwX-E";

BlynkTimer timer;

void checkBlynkStatus() { // called every 2 seconds by SimpleTimer

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
    Serial.println("Blynk Not Connected");
    digitalWrite(wifiLed, LOW);
  }
  if (isconnected == true) {
    wifiFlag = 0;
    if (!fetch_blynk_state){
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
    Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
    Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
    }
    digitalWrite(wifiLed, HIGH);
    Serial.println("Blynk Connected");
  }
}

BLYNK_CONNECTED() {
  // Request the latest state from the server
  if (fetch_blynk_state){
    Blynk.syncVirtual(VPIN_BUTTON_1);
    Blynk.syncVirtual(VPIN_BUTTON_2);
    Blynk.syncVirtual(VPIN_BUTTON_3);
    Blynk.syncVirtual(VPIN_BUTTON_4);
  }
}

BLYNK_WRITE(V1){
  int value = param.asInt();
  if (value == 1) {
    digitalWrite(RelayPin1, HIGH);
    Serial.print("Relay 1 =");
    Serial.println(value);
  } else {
    digitalWrite(RelayPin1, LOW);
    Serial.print("Relay 1 = ");
    Serial.println(value);
  }
}

BLYNK_WRITE(V2){
  
  int value = param.asInt();
  if (value == 1) {
    digitalWrite(RelayPin2, HIGH);
    Serial.print("Relay 2 = ");
    Serial.println(value);
  } else {
    digitalWrite(RelayPin2, LOW);
    Serial.print("Relay 2 = ");
    Serial.println(value);
  }
}

BLYNK_WRITE(V3){
  
  int value = param.asInt();
  if (value == 1) {
    digitalWrite(RelayPin3, HIGH);
    Serial.print("Relay 3 = ");
    Serial.println(value);
  } else {
    digitalWrite(RelayPin3, LOW);
    Serial.print("Relay 3 = ");
    Serial.println(value);
  }
}

BLYNK_WRITE(V4){
  
  int value = param.asInt();
  if (value == 1) {
    digitalWrite(RelayPin4, HIGH);
    Serial.print("Relay 4 = ");
    Serial.println(value);
  } else {
    digitalWrite(RelayPin4, LOW);
    Serial.print("Relay 4 = ");
    Serial.println(value);
  }
}
BLYNK_WRITE(V5){
  
  int value = param.asInt();
  if (value == 1) {
    digitalWrite(LightPin, HIGH);
    Serial.print("Light = ");
    Serial.println(value);
  } else {
    digitalWrite(LightPin, LOW);
    Serial.print("Light = ");
    Serial.println(value);
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
      background-color: #FF6F61; /* Coral color */
    }

    #relay2 {
      background-color: #5DADE2; /* Sky blue color */
    }

    #relay3 {
      background-color: #2E86C1; /* Dark cerulean color */
    }

    #relay4 {
      background-color: #48C9B0; /* Medium turquoise color */
    }

    #relay5 {
      background-color: #F4D03F; /* Sahara color */
    }
  </style>
</head>
<body>

<h2>Relay Control</h2>

<div class="button-container">
  <div class="button" id="relay1" onclick="toggleRelay(1)">
    <div>Relay 1</div>
    <div id="relay1-state">Off</div>
  </div>
  <div class="button" id="relay2" onclick="toggleRelay(2)">
    <div>Relay 2</div>
    <div id="relay2-state">Off</div>
  </div>
  <div class="button" id="relay3" onclick="toggleRelay(3)">
    <div>Relay 3</div>
    <div id="relay3-state">Off</div>
  </div>
  <div class="button" id="relay4" onclick="toggleRelay(4)">
    <div>Relay 4</div>
    <div id="relay4-state">Off</div>
  </div>
  <div class="button" id="relay5" onclick="toggleRelay(5)">
    <div>Light</div>
    <div id="relay5-state">Off</div>
  </div>
</div>

<script>
function toggleRelay(relayNum) {
  var state = document.getElementById("relay" + relayNum + "-state").innerHTML;
  var newState = state === "Off" ? "On" : "Off";
  document.getElementById("relay" + relayNum + "-state").innerHTML = newState;
  // Send a GET request to the ESP32 to control the relay
  fetch("/control?relay=" + relayNum + "&state=" + newState);
}

</script>

</body>
</html>
)=====";

void handleRoot() {
  server.send(200, "text/html", html);
}


void handleControl() {
  if (server.args() > 0) {
    for (uint8_t i = 0; i < server.args(); i++) {
      if (server.argName(i) == "relay") {
        int relayNum = server.arg("relay").toInt();
        String state = server.arg("state");
        if (state == "On") {
          Serial.print("Relay ");
          Serial.print(relayNum);
          Serial.println(" turned on");
          if (relayNum == 1){
            Blynk.virtualWrite(V1, 1);
          }else if (relayNum == 2){
            Blynk.virtualWrite(V2, 1);
          }else if (relayNum ==3){
            Blynk.virtualWrite(V3, 1);
          }else if (relayNum == 4 ){
            Blynk.virtualWrite(V4, 1);
          }else if (relayNum == 5 ){
            Blynk.virtualWrite(V5, 1);
          }
        } else if (state == "Off") {
          Serial.print("Relay ");
          Serial.print(relayNum);
          Serial.println(" turned off");
          if (relayNum == 1){
            Blynk.virtualWrite(V1, 0);
          }else if (relayNum == 2){
            Blynk.virtualWrite(V2, 0);
          }else if (relayNum ==3){
            Blynk.virtualWrite(V3, 0);
          }else if (relayNum == 4 ){
            Blynk.virtualWrite(V4, 0);
          }else if (relayNum == 5 ){
            Blynk.virtualWrite(V5, 0);
          }
        }
      }
    }
  }
  server.send(200, "text/plain", "OK");
}
  


void setup()
{
  Serial.begin(115200);

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);

  pinMode(wifiLed, OUTPUT);

  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, !toggleState_1);
  digitalWrite(RelayPin2, !toggleState_2);
  digitalWrite(RelayPin3, !toggleState_3);
  digitalWrite(RelayPin4, !toggleState_4);

  digitalWrite(wifiLed, LOW);

  //Blynk.begin(auth, ssid, pass);
  WiFi.begin(ssid, pass);
  timer.setInterval(2000L, checkBlynkStatus); // check if Blynk server is connected every 2 seconds
  Blynk.config(auth);
  delay(1000);
  
  if (!fetch_blynk_state){
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
    Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
    Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
  }
  
  server.on("/", HTTP_GET, handleRoot);
  server.on("/control", HTTP_GET, handleControl);

  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{ 
  
  Blynk.run();
  timer.run();
  server.handleClient();
}
