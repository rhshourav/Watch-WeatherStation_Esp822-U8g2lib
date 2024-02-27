#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino.h>
#include <Arduino_JSON.h>
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

String getWeatherData() {
  WiFiClientSecure client;
  String url = "http://api.openweathermap.org/data/2.5/weather?q=Dhaka,BD&units=metric&appid=" + String(apiKey);

  if (client.connect("api.openweathermap.org", 443)) {
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: api.openweathermap.org\r\n" +
                 "Connection: close\r\n\r\n");
    while (!client.available()) {
      delay(100);
    }
    String response = client.readString();
    return response;
  } else {
    Serial.println("Error connecting to OpenWeatherMap");
    return "";
  }
}
void setup() {
  Serial.begin(115200);
  pinMode(blueLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
 /* // Fetch weather data from OpenWeatherMap
  String url = "http://api.openweathermap.org/data/2.5/weather?q=Dhaka,BD&units=metric&appid=" + String(apiKey);
  WiFiClient client; // Create a WiFiClient object
  HTTPClient http;
  http.begin(client, url); // Use the correct begin method

  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    // Parse the JSON response to get weather conditions
    // You'll need to implement this part based on the API response
    // Example: check if it's clear sky, cloudy, rainy, etc.
    // Set the corresponding LEDs accordingly
    // For simplicity, I'll assume clear sky here:
    digitalWrite(blueLedPin, HIGH);
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, LOW);

    // Print weather data to Serial Monitor
    Serial.println("Weather Data:");
    Serial.println(payload);
  } else {
    Serial.println("Error fetching weather data");
  }
*/
  http.end();
  delay(60000); // Wait for 1 minute before fetching data again
}