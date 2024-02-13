#define RS485_FC 4

void setup() {
  Serial.begin(115200);
  Serial.println("Device Starting");

  pinMode(RS485_FC, OUTPUT);
}

void loop() {
  digitalWrite(RS485_FC, HIGH);   // Turns on Transmitter Mode
  Serial.println("RS-485 Sending"); 
  
  delay(500);
}