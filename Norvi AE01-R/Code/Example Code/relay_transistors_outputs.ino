#define OUTPUT1 14

void setup() {
  Serial.begin(115200);
  Serial.println("Device Starting");

  pinMode(OUTPUT1 , OUTPUT);
}

void loop() {
  digitalWrite(OUTPUT1, HIGH);
  delay(500);
  digitalWrite(OUTPUT1, LOW);
  delay(500);

  Serial.println(""); 
  delay(500);
}