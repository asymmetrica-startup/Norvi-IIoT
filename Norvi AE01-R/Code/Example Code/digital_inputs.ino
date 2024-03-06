#define INPUT1 18

void setup() {
  Serial.begin(115200);
  Serial.println("Device Starting");

  pinMode(INPUT1, INPUT);
}

void loop() {
  Serial.print(digitalRead(INPUT1));

  Serial.println(""); 
  delay(500);
}