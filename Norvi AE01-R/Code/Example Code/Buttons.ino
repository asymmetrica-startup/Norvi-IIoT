#define buttonPin 32
int  buttonState = 0;

void setup() {
 
  Serial.begin(9600);                             
  pinMode(buttonPin,INPUT);
}
void loop() { 
  Serial.print("Button: ");
  buttonState = analogRead(buttonPin);
  delay(50);
  Serial.print(analogRead(buttonPin));
  Serial.print("\tAnalog: ");
  delay(1000);
}