#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <UniversalTelegramBot.h>

#define BOT_TOKEN "6910328263:AAFSaqnW6Gc9JXkonkqYAsemwAR591aEdOo"

const char* ssid = "Taranto OnAir";
const char* password = "Alex260981";
const char* mqtt_server = "38.242.140.25";

WiFiClient espClient;
PubSubClient client(espClient);
UniversalTelegramBot bot(BOT_TOKEN, espClient);

void setup() {
  Serial.begin(115200);

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
}

void loop() {
  // Esempio di pubblicazione su un topic MQTT
  client.publish("norvi/BM1-ES/test", "Ciao dal tuo Norvi BM1-ES!");
  // Mantieni la connessione al broker MQTT
  client.loop();
  delay(10000);
}