#include <ModbusMaster.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define MAX485_RE_NEG 33
#define MAX485_DE     32

ModbusMaster node;

const char* ssid = "FASTWEB-Ubx8JX";
const char* password = "TGqfj9GTK4";
const char* broker = "38.242.140.25";
const int porta_broker = 1883;
const char* nome_client = "NorviESP32";

WiFiClient espClient;
PubSubClient client(espClient);

void preTransmission() {
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
  delayMicroseconds(500);
}

void postTransmission() {
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
  delayMicroseconds(500);
}

void setup() {
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);

  Serial.begin(9600);

  // Configurazione WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connessione WiFi in corso...");
  }
  Serial.println("Connesso a WiFi");

  // Configurazione MQTT
  client.setServer(broker, porta_broker);

  node.begin(1, Serial);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }

  client.loop();

  uint8_t result;
  uint16_t data[32];

  result = node.readHoldingRegisters(0x0000, 10);

  if (result == node.ku8MBSuccess) {
    publishToMQTT(data, 10);
  }

  delay(1000);
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.println("Connessione a MQTT...");
    if (client.connect(nome_client)) {
      Serial.println("Connesso a MQTT");
    } else {
      Serial.print("Riprova in 5 secondi...");
      delay(5000);
    }
  }
}

void publishToMQTT(uint16_t data[], int length) {
  // Creazione di una stringa per contenere i dati
  String payload = "";

  // Concatenazione dei dati nella stringa payload
  for (int i = 0; i < length; i++) {
    payload += String(data[i]);
    if (i < length - 1) {
      payload += ",";
    }
  }

  // Converte la stringa in un array di caratteri (const char*)
  const char* payloadChar = payload.c_str();

  // Pubblicazione dei dati sul topic "norvi"
  if (client.publish("norvi", payloadChar)) {
    Serial.println("Dati pubblicati su MQTT");
  } else {
    Serial.println("Errore durante la pubblicazione dei dati su MQTT");
  }
}

