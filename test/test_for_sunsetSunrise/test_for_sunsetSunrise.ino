#include <Wire.h>
#include <U8g2lib.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Time.h> // Include TimeLib for time functions

// Replace with your network credentials
const char* ssid = "WiFi Ki Tor Bap Er";
const char* password = "f0kirni007";

// Replace with your OpenWeatherMap API key
const char* apiKey = "e542006c2fc74216f5ef208e45def0bd";
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0,12, 14, U8X8_PIN_NONE);

void setup() {
    u8g2.begin();
    u8g2.setFont(u8g2_font_ncenB14_tr);
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    // Fetch weather data
    String weatherData = getWeatherData();
    parseWeatherData(weatherData);
}

void loop() {
    // Your main loop code (if any)
}

String getWeatherData() {
    String url = "http://api.openweathermap.org/data/2.5/weather?q=Dhaka,BD&units=metric&appid=e542006c2fc74216f5ef208e45def0bd";
    WiFiClient client; 
    HTTPClient http;
    http.begin(client, url);

    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
        String response = http.getString();
        http.end();
        return response;
    } else {
        Serial.println("Error fetching weather data");
        http.end();
        return "";
    }
}

void parseWeatherData(String weatherData) {
    StaticJsonDocument<500> doc; // Adjust the size as needed
    DeserializationError error = deserializeJson(doc, weatherData);
    if (error) {
        Serial.print("Error parsing JSON: ");
        Serial.println(error.c_str());
        return;
    }

    time_t sunriseTime = doc["sys"]["sunrise"];
    time_t sunsetTime = doc["sys"]["sunset"];
    int maxTemp = doc["main"]["temp_max"];
    int minTemp = doc["main"]["temp_min"];
    String sunriseTimeString = getTimeString(sunriseTime);
    String sunsetTimeString = getTimeString(sunsetTime);
    Serial.println(maxTemp);
    Serial.println(minTemp);
   Serial.println(sunriseTime);
   Serial.println(sunsetTime);
   Serial.println(sunriseTimeString);
   Serial.println(sunsetTimeString);
}

String getTimeString(int time) {
  int hour = (time + 10800) % 86400 / 3600;  // Adjust for timezone offset (if needed)
  int minute = (time + 10800) % 3600 / 60;
  int hour12 = (hour + 11) % 12 + 1;  // Convert to 12-hour format
  String period = (hour < 12) ? "AM" : "PM";  // Determine the period (AM or PM)
  String timeString = String(hour12) + ":" + String(minute) + " " + period;
  return timeString;
}
