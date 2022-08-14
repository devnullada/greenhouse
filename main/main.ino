#include "DHT.h"
#define DHTTYPE DHT11
#define DHTPIN 2

DHT dht(DHTPIN, DHTTYPE);

// CONFIG
unsigned long waitSeconds = 60;
float tempThreshold = 25;
unsigned long heatSeconds = 10;

int pinHeater = 10;
bool isHeating = false;
unsigned long lastStarted;
unsigned long lastStopped;


void setup() {
  lastStopped = millis() - (waitSeconds * 1000);
  
  pinMode(pinHeater, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT); // Relay
  pinMode(13, OUTPUT); // Relay

  dht.begin();
  
  digitalWrite(pinHeater, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  
}

void loop() {
  delay(1000);
  float celsius = dht.readTemperature();
  unsigned long stoppedSeconds = (millis() - lastStopped) / 1000;
  unsigned long runningSeconds = (millis() - lastStarted) / 1000;

  if (
      !isHeating && 
      celsius < tempThreshold &&
      stoppedSeconds > waitSeconds) {
    startHeater();
  }

  if (isHeating && runningSeconds > heatSeconds) {
    stopHeater();
  }

}


void startHeater() {
  digitalWrite(pinHeater, HIGH);
  isHeating = true;
  lastStarted = millis();
  delay(1000);
}

void stopHeater() {
  digitalWrite(pinHeater, LOW);
  isHeating = false;
  lastStopped = millis();
  lastStarted = millis();
  delay(1000);
}
