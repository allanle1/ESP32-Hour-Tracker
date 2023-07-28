#include <Arduino.h>
#include <WiFi.h>
#include "Network.h"

Network *network;

const int voltagePin = 36; // Pin to measure voltage
const int threshold = 1;   // Voltage threshold for device activation

volatile bool isDeviceActive = false;
volatile unsigned long startTime = 0;
volatile unsigned long totalOperatingTime = 0;

void IRAM_ATTR handleInterrupt()
{
  isDeviceActive = digitalRead(voltagePin) > threshold;

  if (isDeviceActive)
  {
    startTime = millis();
  }
  else
  {
    totalOperatingTime += millis() - startTime;
  }
}

void initNetwork()
{
  network = new Network();
  network->initWiFi();
}

void setup()
{
  pinMode(voltagePin, INPUT);
  attachInterrupt(digitalPinToInterrupt(voltagePin), handleInterrupt, CHANGE);

  Serial.begin(921600);
  initNetwork();
  // WiFi.begin("ASUS", "iamnotfat");

  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(1000);
  //   Serial.println("Connecting to WiFi...");
  // }

  // Serial.println("Connected to WiFi");
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    // unsigned long operatingHours = totalOperatingTime / 3600000;       // Convert milliseconds to hours
    unsigned long operatingMinutes = (totalOperatingTime / 3600); // Convert remaining milliseconds to minutes

    Serial.print("Operating Time: ");
    // Serial.print(operatingHours);
    // Serial.print(" hours, ");
    Serial.print(operatingMinutes);
    Serial.println(" seconds");
  }
  delay(10000);
  network->firestoreDataUpdate(totalOperatingTime);
}