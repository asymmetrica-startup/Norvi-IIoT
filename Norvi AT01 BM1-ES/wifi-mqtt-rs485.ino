#include <Adafruit_ADS1X15.h>
#include <Adafruit_NeoPixel.h>
#include <ModbusMaster.h>                             
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <UniversalTelegramBot.h>

#define RS485_RX_PIN 21
#define RS485_TX_PIN 22

#define PIN      25
#define NUMPIXELS 1
#define BOT_TOKEN "6910328263:AAFSaqnW6Gc9JXkonkqYAsemwAR591aEdOo"

ModbusMaster node;
Adafruit_ADS1115 ads1; 
Adafruit_NeoPixel pixels (NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const char* ssid = "FASTWEB-Ubx8JX";
const char* password = "TGqfj9GTK4";
const char* mqtt_server = "38.242.140.25";

WiFiClient espClient;
PubSubClient client(espClient);
UniversalTelegramBot bot(BOT_TOKEN, espClient);

void preTransmission() {
  digitalWrite(RS485_RX_PIN, 1);
  digitalWrite(RS485_TX_PIN, 1);
  delayMicroseconds(500);
}

void postTransmission() {
  digitalWrite(RS485_RX_PIN, 0);
  digitalWrite(RS485_TX_PIN, 0);
  delayMicroseconds(500);
}

void setup() {
  Serial.begin(115200);

  pixels.setBrightness(90);
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));
  pixels.show();

  // Connessione alla rete WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connessione in corso");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnesso alla rete WiFi");

  // Connessione al broker MQTT
  client.setServer(mqtt_server, 1883);
  while (!client.connected()) {
    Serial.println("Connessione al broker MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Connesso al broker MQTT");
    } else {
      Serial.print("Errore di connessione, riprovo in 5 secondi...");
      delay(5000);
    }
  }
  node.begin(1, Serial);                              
  node.preTransmission(preTransmission);              
  node.postTransmission(postTransmission);
}

void loop() {
   uint8_t result;

   result = node.readHoldingRegisters(0x0000, 3);

if (result == node.ku8MBSuccess) {
    int register1 = node.getResponseBuffer(0);
    int register2 = node.getResponseBuffer(2);
    int register3 = node.getResponseBuffer(3);
    int register4 = node.getResponseBuffer(4);

    // Costruisci il payload MQTT
    String payload = "{\"register1\":" + String(register1) + ", \"register2\":" + String(register2) + ", \"register3\":" + String(register3) + ", \"register4\":" + String(register4) + "}";

    // Pubblica il payload sul topic desiderato
    client.publish("norvi/BM1-ES/RS485/test", payload.c_str());
  }
  client.loop();
  delay(10000);
}