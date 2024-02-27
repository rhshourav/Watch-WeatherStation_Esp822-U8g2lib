#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino.h>
#include <ArduinoJson.h>
// Replace with your Wi-Fi credentials
const char* ssid = "TEST";
const char* password = "test1234";

// Replace with your OpenWeatherMap API key
const char* apiKey = "e542006c2fc74216f5ef208e45def0bd";

// Define LED pins
const int blueLedPin = D1;
const int yellowLedPin = D2;
const int greenLedPin = D3;
const int redLedPin = D4;
void setup() {
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

    float temperature = doc["main"]["feels_like"];
    int humidity = doc["main"]["humidity"];
    String population = doc["clouds"]["all"];
    
 float windSpeed = doc["timezone"];
   
    int timezone = doc[""]["timezone"];


    unsigned long country = doc["sys"]["sunrise"];
 






    // Print the extracted values
    Serial.print("Wind Speed: ");
    Serial.print(windSpeed);
    Serial.println(" m/s");

    Serial.print("Country: ");
    Serial.println(country);

    Serial.print("Timezone: ");
    Serial.print(timezone);
    Serial.println(" seconds");

    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Humidity: ");
    Serial.println(humidity);
    Serial.print("pop: ");
    Serial.println(population);
    
    // You can add more data points as needed (e.g., weather description, pressure, etc.)

    // Control LEDs based on weather conditions (example)
    digitalWrite(LED_BUILTIN, HIGH); // Turn on built-in LED
}