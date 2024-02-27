

String statusWe(int state){
  String weinfo;
  String errorMes ="Did't got the right information."
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
    }elae{
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
    }elae{
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
      Serial.prinln(errorMes);
    }
  }else {
    Serial.println(errorMes);
  }


  return weinfo;
}


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
