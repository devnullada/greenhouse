#include "DHT.h"
#define DHTTYPE DHT11
#define DHTPIN 2

DHT dht(DHTPIN, DHTTYPE);

// CONFIG
boolean debug = false;
unsigned long waitSeconds = 30;
float tempThreshold = 24;
unsigned long heatSeconds = 15;
// END CONFIG

int pinHeater = 11;


void setup() {
  pinMode(pinHeater, OUTPUT);
  digitalWrite(pinHeater, LOW);

  dht.begin();

  if (debug) {
    Serial.begin(9600);  
  }
}

void loop() {
  float celsius = dht.readTemperature();
  
  if (celsius < tempThreshold) {
    long addSeconds = (7 - (celsius / 2));
    if (addSeconds < 0) addSeconds = 0;
    if (addSeconds > 5) addSeconds = 5;  
    long onSeconds = heatSeconds + addSeconds;    

    if (debug) {
      Serial.print(F("onSeconds: "));
      Serial.print(onSeconds);
      Serial.print(F("\n "));
      Serial.print(F("addSeconds: "));
      Serial.print(addSeconds);
      Serial.print(F("\n "));
    }
    
    digitalWrite(pinHeater, HIGH);
    delay(onSeconds * 1000);
    digitalWrite(pinHeater, LOW);
  }

  delay(waitSeconds * 1000);

  if (debug) {
    Serial.print(F("Celsius: "));
    Serial.print(celsius);
    Serial.print(F("\n "));
  }
}
