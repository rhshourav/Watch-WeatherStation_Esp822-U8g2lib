//#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <NTPClient.h>
#include <time.h>
#include <WiFiUdp.h>

// WiFi credentials
char ssid[] = "TEST";
char pass[] = "test1234";

const int ScreenWidth = 64;
// Define NTP client
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "bd.pool.ntp.org");  // NTP server in Bangladesh
U8G2_SSD1306_128X64_NONAME_F_SW_I2C
u8g2(U8G2_R0, 12, 14, U8X8_PIN_NONE);
unsigned long lastDisplayTime = 0;
const unsigned long displayInterval = 1000;//300000;  // 5 minutes in milliseconds


int switchState = 0;
int numIncr = 5;
int Blevel ;
int del = 100;



String getCurrentTime() {
  // Get current time in 12-hour format
  int h = timeClient.getHours();
  int m = timeClient.getMinutes();
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
void ledFade(){
    if (switchState == 0){
    analogWrite(BUILTIN_LED, Blevel);
    Blevel = Blevel+ numIncr;
    if (Blevel == 255){
      switchState = 1;
    }
    delay(del);

  }
  if (switchState == 1){
    analogWrite(BUILTIN_LED, Blevel);
    Blevel = Blevel - numIncr;
    if (Blevel == 0){
      switchState = 0;
    }
    delay(del);
  }
}

void setup(){
	Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);
	Serial.println("Initing Program");
	u8g2.begin();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  u8g2.clearBuffer();
  timeClient.begin();
  timeClient.setTimeOffset(21600);   // GMT+6:00 for Bangladesh Standard Time
  timeClient.update();
}

void loop(){
  unsigned long currentMillis = millis();

  if (currentMillis - lastDisplayTime >= displayInterval) {
    lastDisplayTime = currentMillis;
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_sisterserif_tr);
    u8g2.drawStr(16, 16,getCurrentDate().c_str() );
    //u8g2.drawStr()
    u8g2.setFont(u8g2_font_mystery_quest_32_tr);
    u8g2.drawStr(11, 45, getCurrentTime().c_str());
    u8g2.setFont( u8g2_font_bitcasual_tf);
    u8g2.drawStr(90, 27, getCurrentPeriod().c_str());
    u8g2.setFont( u8g2_font_sisterserif_tr);
    u8g2.drawStr(90, 45, getCurrentSeconds().c_str());  
    int textPosition = ((ScreenWidth - getCurrentWeekday().length())/2);
    u8g2.setFont( u8g2_font_sisterserif_tr);
    u8g2.drawStr(textPosition, 60, getCurrentWeekday().c_str());
    u8g2.sendBuffer();
    

}
  ledFade();
}