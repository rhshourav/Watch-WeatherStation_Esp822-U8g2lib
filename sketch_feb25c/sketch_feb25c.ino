#include <ESP8266WiFi.h>
int button = 4;
int led = 1;
void setup() {
  Serial.begin(115200);
  pinMode(button, INPUT);
  pinMode(led, OUTPUT);

  // put your setup code here, to run once:

}

void loop() {
  digitalWrite(led, HIGH);
  delay(300);
  digitalWrite(led, LOW);
  delay(300);
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(button));
  //delay(500);
}
