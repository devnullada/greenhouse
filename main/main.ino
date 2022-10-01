#include "DHT.h"
#define DHTTYPE DHT11
#define DHTPIN 2

DHT dht(DHTPIN, DHTTYPE);

// CONFIG
boolean debug = false;
unsigned long waitSeconds = 60;
float tempThreshold = 21;
unsigned long heatSeconds = 5;
// END CONFIG

int pinHeater = 11;
bool isHeating = false;
unsigned long lastStarted;
unsigned long lastStopped;


void setup() {
  pinMode(pinHeater, OUTPUT);

  digitalWrite(pinHeater, LOW);
  delay(1000);
  digitalWrite(pinHeater, HIGH);
  delay(5000);
  digitalWrite(pinHeater, LOW);
  delay(1000);

  lastStopped = millis() - (waitSeconds * 1000);
  lastStarted = millis() - (waitSeconds * 1000) - (heatSeconds * 1000);

  dht.begin();

  if (debug) {
    Serial.begin(9600);  
  }
}

void loop() {
  delay(1000);
  float celsius = dht.readTemperature();
  unsigned long stoppedSeconds = (millis() - lastStopped) / 1000;
  
  long addSeconds = (7 - (celsius / 2));
  if (addSeconds < 0) addSeconds = 0;
  if (addSeconds > 5) addSeconds = 5;

  

  if (
      !isHeating && 
      celsius < tempThreshold &&
      stoppedSeconds > waitSeconds) {
    heatSeconds = 5 + addSeconds;    
    startHeater();
  }

  unsigned long runningSeconds = (millis() - lastStarted) / 1000;

  if (isHeating && runningSeconds > heatSeconds) {
    stopHeater();
  }

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


void startHeater() {
  digitalWrite(pinHeater, HIGH);
  isHeating = true;
  lastStarted = millis();
  if (debug) {
    Serial.print(F("START\n"));
  }
  delay(1000);
}

void stopHeater() {
  digitalWrite(pinHeater, LOW);
  isHeating = false;
  lastStopped = millis();
  if (debug) {
    Serial.print(F("STOP\n"));
  }
  delay(1000);
}
