#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT22 
int counter = 0;
float tempvals[10]; 
float temphumid[10];

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  //to add location column
  //Serial.println("Humidity, \tTemp_C, \tLocation");
  Serial.println("Humidity, \tTemp_C");

  
  
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(60000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(h);
  Serial.print(", \t");
  Serial.print(t);
  counter++;
  if (counter == 10) {
    for (int i=0; i<10; i++) {
      // add code for averaging here
    }
  counter = 0;
  }

  //Code added is for Location
  //Serial.print(", \t");
  //Serial.print("Study_Area");
  Serial.println();

}