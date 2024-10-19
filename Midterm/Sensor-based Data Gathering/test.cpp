#include <WiFi.h>
#include "time.h"
#include "DHT.h"
#include "ThingSpeak.h"

WiFiClient client;
const char *ssid = "pc-server";
const char *password = "password";


#define DHTPIN 26  
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);

float humidityReadings[10];
float temperatureReadings[10];
int readingIndex = 0;


void setup() {
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  dht.begin();  

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");;
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);

}

void loop() {
    digitalWrite(2, LOW);
    if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi disconnected! Reconnecting...");
    WiFi.reconnect();
    return;
  }

  static unsigned long lastReadingTime = 0;
  if (millis() - lastReadingTime >= 60000 ) { // time in between reads
    lastReadingTime = millis();
    
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    humidityReadings[readingIndex] = humidity;
    temperatureReadings[readingIndex] = temperature;
    readingIndex++;
    // you can add code for printing per minute temp & humid here
    Serial.print(temperature);
    Serial.print(", \t");
    Serial.print(humidity);
    Serial.print(", \t");
    Serial.println();
  }

  if (readingIndex == 10) {
    float avgHumidity = 0.0;
    float avgTemperature = 0.0;
    for (int i = 0; i < 10; i++) {
      avgHumidity += humidityReadings[i];
      avgTemperature += temperatureReadings[i];
    }
    avgHumidity /= 10;
    avgTemperature /= 10; 
    ThingSpeak.setField(1, avgHumidity);
    ThingSpeak.setField(2, avgTemperature);
    int x = ThingSpeak.writeFields(channel, API);
    if (x == 200) {
      digitalWrite(2, HIGH);
    }

    readingIndex = 0;
  }
  delay(1000);
  
}
