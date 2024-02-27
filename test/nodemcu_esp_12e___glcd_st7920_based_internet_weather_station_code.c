/*Developer: Rajeev Mehndiratta
   Date: 09/08/2021
   Project: Internet Weather Station
   Target Hardware: nodeMCU ESP-12E
*/

// include necessary header files
#include <Arduino.h>
#include <U8g2lib.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Timezone.h>

// some macros

#define ROHTAK -1
#define CREDITS -2
#define THUNDER 0
#define DRIZZLE 1
#define RAIN 2
#define SNOW 3
#define SUN  4
#define FOG 5
#define CLEAR 6
#define CLOUD 7
#define HUMIDITY 8
#define WIND 9
#define PRESSURE 10
#define TEMPMIN 11
#define TEMPMAX 12
#define FEELSLIKE 13
#define CLOUDS 14
#define WINDDEGREE 15
#define WINDGUST 16
#define VISIBILITY 17
#define SUNRISE 18
#define SUNSET 19
#define QUOTE 20

#define API D4
#define DELAY1 4000
#define DELAY2 4000

// some global variables
String locationn;
String countryy;
int temperaturee;
int humidityy;
String weatherr;
String descriptionn;
int pressuree;
unsigned long int sunrisee;
unsigned long int sunsett;
int temp_minn;
int temp_maxx;
float speedd;
int visibilityy;
int wind_degreee;
String idstringg;
int feels_likee;
float wind_speedd;
float wind_gustt;
int cloudss;
int sr_hrs;
int sr_mnt;
int st_hrs;
int st_mnt;


//for first call only later overwritten
//you may comment "latt" "lonn"
float latt = 28.8955;
float lonn = 76.6066;


// wifi credentials
const char* ssid = "your-router-name";
const char* password = "your-password";

// your open weather https://openweathermap.org/api api
String openWeatherMapApiKey = "your-api";

// Replace with your country code and city
String city = "Rohtak"; 
String countryCode = "IN";

// httpp call data variables
String payload;
String result;
char servername[] = "api.openweathermap.org";

//IN Indian Standard Time (KAsia/Kolkata)
TimeChangeRule mySTD = {"IST", Second, Sun, Mar, 2, 330};    // Standard time = UTC+5.30 hours
//India presently does not observe daylight saving time (DST or summer time).
TimeChangeRule myDST = {"IST", First, Sun, Nov, 2, 330};

Timezone myTZ(mySTD);

TimeChangeRule *tcr;


// udg2 st7920 construction
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, 14, 13, 15, U8X8_PIN_NONE);


//wifi client instance
WiFiClient client;

// starting screen function Rohtak
void drawRohtak()
{
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_8x13B_mr);
    u8g2.setCursor(6, 20);
    u8g2.print(F("Weather Station"));
    u8g2.setFont(u8g2_font_tenstamps_mf);
    u8g2.setCursor(20, 40);
    u8g2.print(F("Rohtak"));
    u8g2.setFont(u8g2_font_nokiafc22_tf);
    u8g2.setCursor(7, 55);
    u8g2.print(F("Lat="));
    u8g2.print(latt, 2);
    u8g2.print("°N");
    u8g2.setFont(u8g2_font_nokiafc22_tf);
    u8g2.setCursor(68, 55);
    u8g2.print(F("Lon="));
    u8g2.print(lonn, 2);
    u8g2.print(F("°E"));

  } while ( u8g2.nextPage() );

}

// credit screen function
void drawCredits()
{
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_fewture_tf);
    u8g2.setCursor(15, 22);
    u8g2.print(F("CREDITS"));
    u8g2.setFont(u8g2_font_open_iconic_human_4x_t);
    u8g2.drawGlyph(15, 60, 68);
    u8g2.setFont(u8g2_font_tenfatguys_t_all);
    u8g2.setCursor(50, 40);
    u8g2.print(F("Ayansh"));
    u8g2.setFont(u8g2_font_open_iconic_text_2x_t);
    u8g2.drawGlyph(74, 55, 80);
    u8g2.setFont(u8g2_font_mademoiselle_mel_tr);
    u8g2.setCursor(60, 63);
    u8g2.print(F("Hridya"));

  } while ( u8g2.nextPage() );
  delay(1000);
}

// client function to send/receive HTTP GET request data
void getWeatherData()
{

  if (client.connect(servername, 80))
  {
    //starts client connection, checks for connection
    client.println("GET /data/2.5/weather?q=" + city + "," + countryCode + "&units=metric&APPID=" + openWeatherMapApiKey);
    //debug
    digitalWrite(API, HIGH);
    delay(1000);
    digitalWrite(API, LOW);
    //debug
    client.println("Host: api.openweathermap.org");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();
    //////////////////////////////
    while (client.connected())
    {
      if (client.available())
      {
        String line = client.readStringUntil('\n');
        Serial.println(line);
      }
    }
    //////////////////////////
  }
  else {
    Serial.println("Remote Server Connection Failed");
    Serial.println();
  }

  while (client.connected() && !client.available())
    delay(1);
  while (client.connected() || client.available())
  {
    char c = client.read();
    result = result + c;
  }
  client.stop();
  result.replace('[', ' ');
  result.replace(']', ' ');
  //debug
  Serial.println(result);
  char jsonArray [result.length() + 1];
  result.toCharArray(jsonArray, sizeof(jsonArray));
  jsonArray[result.length() + 1] = '\0';
  StaticJsonBuffer<1024> json_buf;
  JsonObject &root = json_buf.parseObject(jsonArray);

  if (!root.success())
  {
    Serial.println("parseObject() failed");
  }


  //store data
  String location = root["name"];
  String country = root["sys"]["country"];
  int temperature = root["main"]["temp"];
  int humidity = root["main"]["humidity"];
  String weather = root["weather"]["main"];
  String description = root["weather"]["description"];
  int pressure = root["main"]["pressure"];
  unsigned long int sunrise = root["sys"]["sunrise"];
  unsigned long int sunset = root["sys"]["sunset"];
  int temp_min = root["main"]["temp_min"];
  int temp_max = root["main"]["temp_max"];
  float wind_speed = root["wind"]["speed"];
  int visibility = root["visibility"];
  int wind_degree = root["wind"]["deg"];
  float wind_gust = root["wind"]["gust"];
  String idstring = root["weather"]["id"];
  int feels_like = root["main"]["feels_like"];
  float lat = root["coord"]["lat"];
  float lon = root["coord"]["lon"];
  int clouds = root["clouds"]["all"];

  //assign to global variables
  locationn = location;
  countryy = country;
  temperaturee = temperature;
  humidityy = humidity;
  weatherr = weather;
  descriptionn = description;
  pressuree = pressure;
  sunrisee = sunrise;
  sunsett = sunset;
  temp_minn = temp_min;
  temp_maxx = temp_max;
  visibilityy = (visibility / 1000);
  wind_degreee = wind_degree;
  idstringg = idstring;
  feels_likee = feels_like;
  wind_speedd = wind_speed;
  wind_gustt = wind_gust;
  cloudss = clouds;
  latt = lat;
  lonn = lon;

  //Convert UNIX Time Stamp epoch to Local IST
  UTC_Local(sunrisee,1);
  delay(1000);
  UTC_Local(sunsett,0);

  // debug
  Serial.println(temperature);
  Serial.println(humidity);
  Serial.println(weatherr);
  Serial.println(pressuree);
  Serial.println(temp_minn);
  Serial.println(temp_maxx);
  Serial.println(visibility);
  Serial.println(wind_degree);
  Serial.println(wind_speedd);
  Serial.println(wind_gustt);
  Serial.println(temp_maxx);
  Serial.println(wind_gustt);
  Serial.println(feels_likee);
  Serial.println(latt);
  Serial.println(lonn);
  Serial.println(cloudss);
  Serial.println();
  Serial.print(sr_hrs);
  Serial.print(":");
  Serial.print(sr_mnt);
  Serial.println();
  Serial.print(st_hrs);
  Serial.print(":");
  Serial.print(st_mnt);



}

// draw weather glpyph
void drawWeatherSymbol_t(u8g2_uint_t x, u8g2_uint_t y, uint8_t symbol)
{

  if (symbol <= 7)
  {
    switch (symbol)
    {
      case THUNDER:
        u8g2.setFont(u8g2_font_open_iconic_embedded_6x_t);
        u8g2.drawGlyph(x, y, 67);
        break;
      case DRIZZLE:
        u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
        u8g2.drawGlyph(x, y, 65);
        break;
      case RAIN:
        u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
        u8g2.drawGlyph(x, y, 67);
        break;
      case SNOW:
        u8g2.setFont(u8g2_font_open_iconic_text_6x_t);
        u8g2.drawGlyph(x, y, 69);
        break;
      case SUN:
        u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
        u8g2.drawGlyph(x, y, 69);
        break;
      case FOG:
        u8g2.setFont(u8g2_font_open_iconic_mime_6x_t);
        u8g2.drawGlyph(x, y, 64);
        break;
      case CLEAR:
        u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
        u8g2.drawGlyph(x, y, 69);
        break;
      case CLOUD:
        u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
        u8g2.drawGlyph(x, y, 64);
        break;
    }
  }
}
// humidity glyph
void drawWeatherSymbol_h(u8g2_uint_t x, u8g2_uint_t y, uint8_t symbol)
{
  u8g2.setFont(u8g2_font_open_iconic_thing_6x_t);
  u8g2.drawGlyph(x, y, 72);
}

// wind glyph
void drawWeatherSymbol_w(u8g2_uint_t x, u8g2_uint_t y, uint8_t symbol)
{
  u8g2.setFont(u8g2_font_open_iconic_all_6x_t);
  u8g2.drawGlyph(x, y, 254);
}

// pressure glyph
void drawWeatherSymbol_p(u8g2_uint_t x, u8g2_uint_t y, uint8_t symbol)
{
  u8g2.setFont(u8g2_font_open_iconic_app_6x_t);
  u8g2.drawGlyph(x, y, 72);
}

// temp min
void drawWeatherSymbol_tn(u8g2_uint_t x, u8g2_uint_t y, uint8_t symbol)
{
  u8g2.setFont(u8g2_font_open_iconic_arrow_6x_t);
  u8g2.drawGlyph(x, y, 72);
}

// temp max
void drawWeatherSymbol_tm(u8g2_uint_t x, u8g2_uint_t y, uint8_t symbol)
{
  u8g2.setFont(u8g2_font_open_iconic_arrow_6x_t);
  u8g2.drawGlyph(x, y, 75);
}

// feels like
void drawWeatherSymbol_fl(u8g2_uint_t x, u8g2_uint_t y, uint8_t symbol)
{
  u8g2.setFont(u8g2_font_open_iconic_text_6x_t);
  u8g2.drawGlyph(x, y, 69);
}

// wind degree
void drawWeatherSymbol_wd(u8g2_uint_t x, u8g2_uint_t y, uint8_t symbol)
{
  u8g2.setFont(u8g2_font_open_iconic_arrow_6x_t);
  u8g2.drawGlyph(x, y, 87);
}

// wind gust
void drawWeatherSymbol_wg(u8g2_uint_t x, u8g2_uint_t y, uint8_t symbol)
{
  u8g2.setFont(u8g2_font_open_iconic_arrow_4x_t);
  u8g2.drawGlyph(x, y, 90);
}

//visibility
void drawWeatherSymbol_v(u8g2_uint_t x, u8g2_uint_t y, uint8_t symbol)
{
  u8g2.setFont(u8g2_font_open_iconic_human_6x_t);
  u8g2.drawGlyph(x, y, 64);
}

//clouds
void drawWeatherSymbol_c(u8g2_uint_t x, u8g2_uint_t y, uint8_t symbol)
{
  u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
  u8g2.drawGlyph(x, y, 64);
}

//sunrise
void drawWeatherSymbol_sr(u8g2_uint_t x, u8g2_uint_t y, uint8_t symbol)
{
  u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
  u8g2.drawGlyph(0, 48, 69);
}

//sunset
void drawWeatherSymbol_ss(u8g2_uint_t x, u8g2_uint_t y, uint8_t symbol)
{
  u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
  u8g2.drawGlyph(0, 48, 66);
}

//quote
void drawWeatherSymbol_wq(u8g2_uint_t x, u8g2_uint_t y, uint8_t symbol)
{
  u8g2.setFont(u8g2_font_open_iconic_mime_6x_t);
  u8g2.drawGlyph(0, 48, 66);
}

// call to glyphs
void drawWeather(uint8_t symbol, int degree)
{
  if (symbol <= 7)  //weather conditions
  {
    drawWeatherSymbol_t(0, 48, symbol);
    u8g2.setFont(u8g2_font_logisoso32_tf);
    u8g2.setCursor(48 + 3, 42);
    u8g2.print(temperaturee);
    u8g2.print("°C");		// requires enableUTF8Print()
  }

  if (symbol == 8)    //humidity
  {
    drawWeatherSymbol_h(0, 48, symbol);
    u8g2.setFont(u8g2_font_logisoso32_tf);
    u8g2.setCursor(48, 42);
    u8g2.print(humidityy);
    u8g2.print("%");
  }

  if (symbol == 9)  //wind
  {
    drawWeatherSymbol_w(0, 48, symbol);
    //u8g2.setFont(u8g2_font_logisoso28_tf);
    u8g2.setFont(u8g2_font_logisoso32_tf);
    u8g2.setCursor(40, 42);
    u8g2.print(wind_speedd, 1);
    u8g2.setFont(u8g2_font_t0_17b_tf);
    u8g2.print("m/s");
  }

  if (symbol == 10)   //pressure
  {
    drawWeatherSymbol_p(0, 48, symbol);
    //u8g2.setFont(u8g2_font_logisoso28_tf);
    u8g2.setFont(u8g2_font_logisoso24_tf);
    u8g2.setCursor(46, 42);
    u8g2.print(pressuree);
    u8g2.setFont(u8g2_font_7x14B_tf);
    //u8g2.print(" ");
    u8g2.print("hPa");
  }

  if (symbol == 11)   //temp min
  {
    drawWeatherSymbol_tn(0, 48, symbol);
    u8g2.setFont(u8g2_font_logisoso32_tf);
    u8g2.setCursor(48 + 3, 42);
    u8g2.print(temp_minn);
    u8g2.print("°C");
  }

  if (symbol == 12)   //temp max
  {
    drawWeatherSymbol_tm(0, 48, symbol);
    u8g2.setFont(u8g2_font_logisoso32_tf);
    u8g2.setCursor(48 + 3, 42);
    u8g2.print(temp_maxx);
    u8g2.print("°C");
  }

  if (symbol == 13)   //feels like
  {
    drawWeatherSymbol_fl(0, 48, symbol);
    u8g2.setFont(u8g2_font_logisoso32_tf);
    u8g2.setCursor(48 + 3, 42);
    u8g2.print(feels_likee);
    u8g2.print("°C");
  }

  if (symbol == 14)   //clouds
  {
    drawWeatherSymbol_c(0, 48, symbol);
    u8g2.setFont(u8g2_font_logisoso32_tf);
    u8g2.setCursor(48 , 42);
    u8g2.print(cloudss);
    u8g2.print("%");
  }

  if (symbol == 15)   //wind degree
  {
    drawWeatherSymbol_wd(0, 48, symbol);
    u8g2.setFont(u8g2_font_logisoso32_tf);
    u8g2.setCursor(48 + 3, 42);
    u8g2.print(wind_degreee);
    u8g2.print("°");
  }

  if (symbol == 16)   //wind gust
  {
    drawWeatherSymbol_wg(0, 48, symbol);
    u8g2.setFont(u8g2_font_logisoso28_tf);
    u8g2.setCursor(35, 42);
    u8g2.print(wind_gustt);
    u8g2.setFont(u8g2_font_t0_17b_tf);
    u8g2.print("m/s");
  }

  if (symbol == 17)   //visiblity
  {
    drawWeatherSymbol_v(0, 48, symbol);
    u8g2.setFont(u8g2_font_logisoso32_tf);
    u8g2.setCursor(48 + 3, 42);
    u8g2.print(visibilityy);
    u8g2.setFont(u8g2_font_lubB14_tr);
    u8g2.print("km");
  }

  if (symbol == 18)   //sunrise
  {
    drawWeatherSymbol_sr(0, 48, symbol);
    u8g2.setFont(u8g2_font_freedoomr25_tn);
    u8g2.setCursor(48 + 5 , 42);
    u8g2.print(sr_hrs);
    u8g2.print(":");
    u8g2.print(sr_mnt);
  }

  if (symbol == 19)   //sunset
  {
    drawWeatherSymbol_ss(0, 48, symbol);
    u8g2.setFont(u8g2_font_freedoomr25_tn);
    u8g2.setCursor(48 + 3 , 42);
    u8g2.print(st_hrs);
    u8g2.print(":");
    u8g2.print(st_mnt);
  }

  if (symbol == 20)   //quote
  {
    drawWeatherSymbol_wq(0, 48, symbol);
    u8g2.setFont(u8g2_font_adventurer_tf);
    u8g2.setCursor(48 + 3 , 20);
    u8g2.print("I live for");
    u8g2.setCursor(43 , 40);
    u8g2.print("this Weather");
  }

}

//monochrome font with 8 pixel per glyph

void drawScrollString(int16_t offset, const char *s)
{
  static char buf[36];	// should for screen with up to 256 pixel width
  size_t len;
  size_t char_offset = 0;
  u8g2_uint_t dx = 0;
  size_t visible = 0;
  len = strlen(s);
  if ( offset < 0 )
  {
    char_offset = (-offset) / 8;
    dx = offset + char_offset * 8;
    if ( char_offset >= u8g2.getDisplayWidth() / 8 )
      return;
    visible = u8g2.getDisplayWidth() / 8 - char_offset + 1;
    strncpy(buf, s, visible);
    buf[visible] = '\0';
    u8g2.setFont(u8g2_font_8x13_mf);
    u8g2.drawStr(char_offset * 8 - dx, 62, buf);
  }
  else
  {
    char_offset = offset / 8;
    if ( char_offset >= len )
      return;	// nothing visible
    dx = offset - char_offset * 8;
    visible = len - char_offset;
    if ( visible > u8g2.getDisplayWidth() / 8 + 1 )
      visible = u8g2.getDisplayWidth() / 8 + 1;
    strncpy(buf, s + char_offset, visible);
    buf[visible] = '\0';
    u8g2.setFont(u8g2_font_8x13_mf);
    u8g2.drawStr(-dx, 62, buf);
  }

}

// weather and scroll call
void draw(const char *s, uint8_t symbol, float degree)
{
  int16_t offset = -(int16_t)u8g2.getDisplayWidth();
  int16_t len = strlen(s);
  for (;;)
  {
    u8g2.firstPage();
    do {
      drawWeather(symbol, degree);
      drawScrollString(offset, s);
    } while ( u8g2.nextPage() );
    delay(20);
    offset += 2;
    if ( offset > len * 8 + 1 )
      break;
  }
}

//UTC epoch seconds to hrs mnts
void UTC_Local(time_t locl,int token)
{
  time_t utc = locl;
  time_t local = myTZ.toLocal(utc, &tcr);
  Serial.println("epoch =");
  Serial.println(locl);
  Serial.println();
  //printDateTime(utc, "UTC");
  time_t t = local;
  const char *tz = tcr -> abbrev;

  if(token==0) //sunset
  {
    st_hrs = hour(t);
    st_mnt = minute(t);
  }
  if(token==1) //sunrise
  {
    sr_hrs = hour(t);
    sr_mnt = minute(t);
  }
  

}
// necessary one time setup
void setup(void) {

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wi-Fi please wait!");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  setTime(myTZ.toUTC(compileTime()));

  Serial.println("API call set to 10 minutes, data will be updated after 10 minutes!!.");
  u8g2.begin();
  u8g2.enableUTF8Print();

}
// Function to return the compile date and time as a time_t value
time_t compileTime()
{
  const time_t FUDGE(10);     // fudge factor to allow for compile time (seconds, YMMV)
  const char *compDate = __DATE__, *compTime = __TIME__, *months = "JanFebMarAprMayJunJulAugSepOctNovDec";
  char chMon[4], *m;
  tmElements_t tm;

  strncpy(chMon, compDate, 3);
  chMon[3] = '\0';
  m = strstr(months, chMon);
  tm.Month = ((m - months) / 3 + 1);

  tm.Day = atoi(compDate + 4);
  tm.Year = atoi(compDate + 7) - 1970;
  tm.Hour = atoi(compTime);
  tm.Minute = atoi(compTime + 3);
  tm.Second = atoi(compTime + 6);

  time_t t = makeTime(tm);
  return t + FUDGE;           

}

void loop(void) {

  // call rohtak function
  drawRohtak();
  delay(DELAY1);

  // call weather function
  getWeatherData();

  //temperature + weather description see raw data

  if (weatherr == "Thunderstorm")
    draw("That sounds like thunder.", THUNDER, temperaturee);
  else if (weatherr == "Drizzle")
    draw("It's quite drizzle outside!", DRIZZLE, temperaturee);
  else if (weatherr == "Rain")
    draw("It's raining cats and dogs.", RAIN, temperaturee);
  else if (weatherr == "Snow")
    draw("It's raining cats and dogs.", SNOW, temperaturee); //
  else if (weatherr == "Fog")
    draw("It's foggy out there!", FOG, temperaturee); //
  else if (weatherr == "Clear")
    draw("The sun's come out!", CLEAR, temperaturee); //
  else if (weatherr == "Clouds")
    draw("It's raining cats and dogs.", CLOUD, temperaturee); //
  else
    Serial.println("No Weather data");
  //debug
  //Serial.println(temperaturee);
  //Serial.println(weatherr);

  //temp_min
  draw("Minimum temperature!!", TEMPMIN, temp_minn);


  //temp_max
  draw("Maximum temperature!!", TEMPMAX, temp_maxx);

  //feels_like
  draw("It feels like!!", FEELSLIKE, feels_likee);

  //clouds
  draw("There are clouds!!!", CLOUDS, cloudss);

  //sunrise
  draw("Hello! new sun!!", SUNRISE, sunrisee);

  //sunset
  draw("Bye bye sun!!", SUNSET, sunsett);

  // humidity
  draw("Humidity in the air!", HUMIDITY, humidityy);
  //debug
  //Serial.println(humidityy);

  //quote
  draw("Weather quote!!", QUOTE, visibilityy);

  //visibility
  draw("See visibility!!", VISIBILITY, visibilityy);

  // wind speed
  draw("Wind travelling speed!", WIND, wind_speedd);
  //Serial.println(wind_speedd);

  // wind degree
  draw("Wind travelling angle!", WINDDEGREE, wind_degreee);

  // wind gust
  draw("Wind gust there!", WINDGUST, wind_gustt);

  // pressure
  draw("Atomospheric pressue!!", PRESSURE, pressuree);
  //Serial.println(pressuree);

  // call credit function
  drawCredits();
  delay(DELAY2);

}