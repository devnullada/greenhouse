#include "DHT.h"
#define DHTTYPE DHT11
#define DHTPIN 2

DHT dht(DHTPIN, DHTTYPE);

// CONFIG
boolean debug = false;
unsigned long waitSeconds = 40;
float tempThreshold = 22;
unsigned long heatSeconds = 6;
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
  
  if (celsius < tempThresholds) {
    long addSeconds = (7 - (celsius / 2));
    if (addSeconds < 0) addSeconds = 0;
    if (addSeconds > 5) addSeconds = 5;  
    long onSeconds = heatSeconds + addSeconds;    
    digitalWrite(pinHeater, HIGH);
    delay(onSeconds * 1000);
    digitalWrite(pinHeater, LOW);
  }

  delay(waitSeconds * 1000);

  if (debug) {
    Serial.print(F("Celsius: "));
    Serial.print(celsius);
    Serial.print(F("heatSeconds: "));
    Serial.print(heatSeconds);
    Serial.print(F("°C. Stopped: "));
    Serial.print(stoppedSeconds);
    Serial.print(F("s. Running: "));
    Serial.print(runningSeconds);
    Serial.print(F("s"));
    Serial.print(F("\n "));
  }
}
