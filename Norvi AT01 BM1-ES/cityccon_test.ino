#include <Adafruit_ADS1X15.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define PIN      25
#define NUMPIXELS 1

const char* ssid = "FASTWEB-Ubx8JX";
const char* password = "TGqfj9GTK4";
const char* mqtt_server = "dashboard.cityconn.cloud";
const int mqtt_port = 30883;
const char* mqtt_user = "dbagroup";
const char* mqtt_password = "eCFckRQHrW2y6gL";

WiFiClient espClient;
PubSubClient client(espClient);

Adafruit_ADS1115 ads1; 
Adafruit_NeoPixel pixels (NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {

    pixels.setBrightness(90);

    Wire.begin(21,22);
    ads1.begin(0x48); 
    ads1.setGain(GAIN_ONE); 

 // Connessione alla rete WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connessione in corso");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnesso alla rete WiFi");
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.show();

  // Connessione al broker MQTT
  client.setServer(mqtt_server, mqtt_port);
  while (!client.connected()) {
    Serial.println("Connessione al broker MQTT...");
    if (client.connect("ESP32Client"), mqtt_user, mqtt_password) {
      Serial.println("Connesso al broker MQTT");
      pixels.setPixelColor(0, pixels.Color(0, 255, 0));
      pixels.show();
    } else {
      Serial.print("Errore di connessione, riprovo in 5 secondi...");
      delay(5000);
    }
  }
  client.publish("measures/@4c:75:25:48:bd:70/System/SN", "4c:75:25:48:bd:70");
  client.publish("measures/@4c:75:25:48:bd:70/System/EC", "Norvi BM1-ES");
  client.publish("measures/@4c:75:25:48:bd:70/System/uptime", "Online");
}

void loop() {

  client.publish("measures/@4c:75:25:48:bd:70/Voltage", "2.84");
  client.publish("measures/@4c:75:25:48:bd:70/Pressure", "3.91");
  client.publish("measures/@4c:75:25:48:bd:70/Temperature", "4.66");

  client.loop();
}