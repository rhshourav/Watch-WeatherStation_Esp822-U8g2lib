//Written  By Frenoy Osburn
// Modified by Emmanuel Odunlade


#include <stdio.h>
#include <ArduinoJson.h>
#include <U8g2lib.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define SUN  0
#define SUN_CLOUD  1
#define CLOUD 2
#define RAIN 3
#define THUNDER 4

U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, 12, 14, U8X8_PIN_NONE);   // All Boards without Reset of the Display

//initiate the WifiClient
WiFiClient client;

const char* ssid = "TEST";                          
const char* password = "test1234";                 

unsigned long lastCallTime = 0;    
unsigned long previousMillis = 0;
const long interval = 30 * 60 * 1000;           // last time you called the parseWeatherData function, in milliseconds
const unsigned long postingInterval = 5L * 1000L;  // delay between updates, in milliseconds
String weatherData ;
String savedData;
String statusWe(int state){
  String weinfo;
  String errorMes ="Did't got the right information.";
  if (state >= 200 && state <= 299){
    if (state ==  200){
      weinfo = "Thunderstorm with light rain";
    }else if (state == 201){
      weinfo = "Thunderstorm with rain";
    }else if (state == 202){
      weinfo = "Thunderstorm with heavy rain";
    }else if (state == 210){
      weinfo = "Light thunderstorm";
    }else if (state == 211){
      weinfo = "Thunderstorm";
    }else if (state == 212){
      weinfo = "Heavy thunderstorm";
    }else if (state == 221){
      weinfo = "Ragged thunderstorm";
    }else if (state == 230){
      weinfo = "Thunderstorm with light drizzle";
    }else if (state == 231){
      weinfo = "Thunderstorm with drizzle";
    }else if (state == 232){
      weinfo = "Thunderstorm with heavy drizzle";
    }else{
      Serial.println(errorMes);
    }
  }else if (state >= 300 && state <= 399){
    if (state == 300){
      weinfo = "Light intensity drizzle";
    }else if (state == 301){
      weinfo = "Drizzle";
    }else if (state == 302){
      weinfo = "Heavy intensity drizzle";
    }else if (state == 310){
      weinfo = "Light intensity drizzle rain";
    }else if (state == 311){
      weinfo = "Drizzle Rain";
    }else if (state == 312){
      weinfo = "Heavy intensity drizzle rain";
    }else if (state == 313){
      weinfo = "Shower rain and drizzle";
    }else if (state == 314){
      weinfo = "Heavy shower rain and drizzle";
    }else if (state == 321){
      weinfo = "Shower drizzle";
    }else{
      Serial.println(errorMes);
    }
  }else if (state >= 500 && state <= 599){
    if (state == 500){
      weinfo = "Light Rain";
    }else if (state == 501){
      weinfo = "Moderate rain";
    }else if (state == 502){
      weinfo = "Heavy intensity rain";
    }else if ( state == 503){
      weinfo = "Very Heavy Rain";
    }else if ( state == 504){
      weinfo = "Extreme Rain";
    }else if (state == 511){
      weinfo = "Freezin Rain";
    }else if (state == 520){
      weinfo = "Light intensity shower rain";
    }else if (state == 521){
      weinfo = "Shower Rain";
    }else if (state == 522){
      weinfo = "Heavy intensity shower rain";
    }else if (state == 531){
      weinfo = "Ragged Shower rain";
    }else {
      Serial.println(errorMes);
    }
  }else if (state >= 600 && state <= 699){
    if (state == 600){
      weinfo = "Light snow";
    }else if (state == 601){
      weinfo = "Snow";
    }else if (state == 602){
      weinfo = "Heavy Snow";
    }else if (state == 611){
      weinfo = "Sleet";
    }else if (state == 612){
      weinfo = "Light Shower sleet";
    }else if (state == 613){
      weinfo = "Shower sleet";
    }else if (state == 615){
      weinfo = "Light rain and snow";
    }else if (state == 616){
      weinfo = "rain and snow";
    }else if (state == 620){
      weinfo = "Light shower snow";
    }else if (state == 621){
      weinfo ="Shower snow";
    }else if (state == 622){
      weinfo = "Heavy Shower snow";
    }else {
      Serial.println(errorMes);
    }
  }else if(state >= 700 && state <= 799){
    if (state == 701){
      weinfo = "Mist";
    }else if (state == 711){
      weinfo = "Smoke";
    }else if (state == 721){
      weinfo = "Haze";
    }else if (state == 731){
      weinfo = "Dust";
    }else if (state == 741){
      weinfo = "Sand/Dust whirls";
    }else if (state == 751){
      weinfo = "Fog";
    }else if (state == 761){
      weinfo = "dust";
    }else if (state == 771){
      weinfo = "Squalls";
    }else if (state == 781){
      weinfo = "Tornado";
    }else {
      Serial.println(errorMes);
    }
  }else if (state >= 800 & state <= 899){
    if (state == 800){
      weinfo = "Clear Sky";
    }else if (state == 801){
      weinfo = "Few Clouds: 11-25%";
    }else if (state == 802){
      weinfo = "Scattered Clouds: 25-50%";
    }else if (state == 803){
      weinfo = "Broken Clouds: 51-84%";
    }else if (state == 804){
      weinfo = "Overcast Clouds: 85-100%";
    }else {
      Serial.println(errorMes);
    }
  }else {
    Serial.println(errorMes);
  }


  return weinfo;
}
void drawScrollString(int16_t offset, const char *s)
{
  static char buf[36];  // should for screen with up to 256 pixel width 
  size_t len;
  size_t char_offset = 0;
  u8g2_uint_t dx = 0;
  size_t visible = 0;
  len = strlen(s);
  if ( offset < 0 )
  {
    char_offset = (-offset)/8;
    dx = offset + char_offset*8;
    if ( char_offset >= u8g2.getDisplayWidth()/8 )
      return;
    visible = u8g2.getDisplayWidth()/8-char_offset+1;
    strncpy(buf, s, visible);
    buf[visible] = '\0';
    u8g2.setFont(u8g2_font_8x13_mf);
    u8g2.drawStr(char_offset*8-dx, 10, buf);
  }
  else
  {
    char_offset = offset / 8;
    if ( char_offset >= len )
      return; // nothing visible
    dx = offset - char_offset*8;
    visible = len - char_offset;
    if ( visible > u8g2.getDisplayWidth()/8+1 )
      visible = u8g2.getDisplayWidth()/8+1;
    strncpy(buf, s+char_offset, visible);
    buf[visible] = '\0';
    u8g2.setFont(u8g2_font_8x13_mf);
    u8g2.drawStr(-dx, 10, buf);
  }
}
void drawWeatherSymbol(u8g2_uint_t x, u8g2_uint_t y, uint8_t symbol)
{
  // fonts used:
  // u8g2_font_open_iconic_embedded_6x_t
  // u8g2_font_open_iconic_weather_6x_t
  // encoding values, see: https://github.com/olikraus/u8g2/wiki/fntgrpiconic
  
  switch(symbol)
  {
    case SUN:
      u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
      u8g2.drawGlyph(x, y, 69);  
      break;
    case SUN_CLOUD:
      u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
      u8g2.drawGlyph(x, y, 65); 
      break;
    case CLOUD:
      u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
      u8g2.drawGlyph(x, y, 64); 
      break;
    case RAIN:
      u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
      u8g2.drawGlyph(x, y, 67); 
      break;
    case THUNDER:
      u8g2.setFont(u8g2_font_open_iconic_embedded_6x_t);
      u8g2.drawGlyph(x, y, 67);
      break;      
  }
}


void drawWeather(uint8_t symbol, int degree)
{
  drawWeatherSymbol(0, 64, symbol);
  u8g2.setFont(u8g2_font_logisoso32_tf);
  u8g2.setCursor(48+3, 62);
  u8g2.print(degree);
  u8g2.print("°C");   // requires enableUTF8Print()
}


void draw(const char *s, uint8_t symbol, int degree)
{
  int16_t offset = -(int16_t)u8g2.getDisplayWidth();
  int16_t len = strlen(s);
  for(;;)
  {
    u8g2.firstPage();
    do {
      drawWeather(symbol, degree);      
      drawScrollString(offset, s);
    } while ( u8g2.nextPage() );
    delay(20);
    offset+=2;
    if ( offset > len*8+1 )
      break;
  }
}String getWeatherData() {
    String url = "http://api.openweathermap.org/data/2.5/weather?q=Dhaka,BD&units=metric&appid=e542006c2fc74216f5ef208e45def0bd";
    WiFiClient client; 
    HTTPClient http;
    http.begin(client, url);

    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
        String response = http.getString();
        savedData = response ;
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
  const char* icon = doc["weather"][0]["icon"];

        
    int weatherId = doc["weather"][0]["id"].as<int>();
    float weatherTemperature = doc["main"]["temp"].as<float>();
    int weatherHumidity = doc["main"]["humidity"].as<int>();
    
    //Disconnect
    client.stop();
    Serial.println(icon);
    Serial.println(F("Response:"));
    Serial.print("Weather: ");
    Serial.println(weatherId);
    Serial.print("Temperature: ");
    Serial.println(weatherTemperature);
    Serial.print("Humidity: ");
    Serial.println(weatherHumidity);
    Serial.println();
    String weData = "Humidity:"+ String(weatherHumidity) +"%% " + statusWe(weatherId).c_str() ;
    char scrollText[15];
    sprintf(scrollText, weData.c_str());
    if (weatherId == 800){
      draw(scrollText, SUN, weatherTemperature);
    }else if(weatherId >= 200 && weatherId <= 299){
      draw(scrollText, THUNDER, weatherTemperature);
    }else if(weatherId >= 300 && weatherId <= 399){
      draw(scrollText, CLOUD, weatherTemperature);
    }else if(weatherId >= 400 && weatherId <= 699){
      draw(scrollText, RAIN, weatherTemperature);
    }else if(weatherId >= 700 && weatherId <= 799){
      draw(scrollText, CLOUD, weatherTemperature);
    }else if(weatherId >= 801 && weatherId <= 899){
      draw(scrollText, SUN_CLOUD, weatherTemperature);
    } 
  } 
  


void printWifiStatus() 
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


void setup()
{  
  u8g2.begin();
  u8g2.enableUTF8Print();   //for the degree symbol
  
  Serial.begin(115200);
  Serial.println("\n\nOnline Weather Display\n");

  Serial.println("Connecting to network");
  WiFi.begin(ssid, password);

  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(200);    
    if (++counter > 100) 
      ESP.restart();
    Serial.print( "." );
  }
  Serial.println("\nWiFi connected");
  printWifiStatus();
   String weatherData = getWeatherData();
    parseWeatherData(weatherData);
}

void loop() 
{    
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval && WiFi.status() == WL_CONNECTED) {
    previousMillis = currentMillis;

    String weatherData = getWeatherData(); 
    parseWeatherData(weatherData); 
  }else{
    parseWeatherData(savedData); 
  }
    
}