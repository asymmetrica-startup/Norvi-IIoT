#include <TinyGsmClient.h>
#include <NB-IoT-Library.h>
#include <PubSubClient.h>

#define RX0 4
#define TX0 5

const char apn[] = "iot.1nce.net";  
const char user[] = ""; 
const char pass[] = "";  

const char* mqtt_server = "38.242.140.25";

TinyGsm modem(SerialAT);
NB nb;

NBClient espClient;
PubSubClient client(espClient);

void setup() {
Serial.begin(9600);
SerialAT.begin(9600, SERIAL_8N1, RX_pin, TX_pin);  // Sostituisci RX_pin e TX_pin con i pin del tuo ESP32
pinMode(19, OUTPUT);
digitalWrite(19, HIGH);
}
  // Inizializza il modulo NB-IoT
  nb.begin(modem);

  // Connessione al servizio NB-IoT
  if (!nb.connect(apn, user, pass)) {
    Serial.println("Connessione fallita");
    return;
  }

  Serial.println("Connessione NB-IoT stabilita!");
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

void loop() {
 if (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
  }
  // read from port 0, send to port 1:
  if (Serial.available()) {
    int inByte = Serial.read();
    Serial2.write(inByte);
  }
    // Il tuo codice può andare qui
  
  // Esempio di pubblicazione su un topic MQTT
  client.publish("norvi/BM1-ES/test", "Ciao dal tuo Norvi BM1-ES!");

  // Mantieni la connessione al broker MQTT
  client.loop();
}
