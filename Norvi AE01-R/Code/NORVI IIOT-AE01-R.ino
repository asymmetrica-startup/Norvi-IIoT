/*
 * RS485
 * All Output Turn ON Series
 * All input status serial print
 
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "FS.h"

#define ANALOG_PIN_0 32

#define INPUT1 18
#define INPUT2 39
#define INPUT3 34
#define INPUT4 35
#define INPUT5 19
#define INPUT6 21
#define INPUT7 22
#define INPUT8 23

#define OUTPUT1 26
#define OUTPUT2 27
#define OUTPUT3 14
#define OUTPUT4 12
#define OUTPUT5 13
#define OUTPUT6 15
#define OUTPUT7 2
#define OUTPUT8 33

#define RS485_RXD 3
#define RS485_TXD 5
#define RS485_FC 4

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int analog_value = 0;

int readSwitch(){
  analog_value = analogRead(ANALOG_PIN_0);

 
  return analog_value                                                                                                ; //Read analog
}

// ================================================ SETUP ================================================
void setup() {
 
  Serial.begin(9600);
  Serial.println("Hello");
 
 pinMode(RS485_FC, OUTPUT); 
 digitalWrite(RS485_FC, HIGH);

 Serial1.begin(9600, SERIAL_8N1,RS485_RXD,RS485_TXD);
  digitalWrite(RS485_FC, HIGH);   // RS-485
  
  pinMode(OUTPUT1, OUTPUT);
  pinMode(OUTPUT2, OUTPUT);
  pinMode(OUTPUT3, OUTPUT);
  pinMode(OUTPUT4, OUTPUT);
  pinMode(OUTPUT5, OUTPUT);
  pinMode(OUTPUT6, OUTPUT);
  pinMode(OUTPUT7, OUTPUT);
  pinMode(OUTPUT8, OUTPUT);

  pinMode(INPUT1, INPUT);
  pinMode(INPUT2, INPUT);
  pinMode(INPUT3, INPUT);
  pinMode(INPUT4, INPUT);
  pinMode(INPUT5, INPUT);
  pinMode(INPUT6, INPUT);
  pinMode(INPUT7, INPUT);
  pinMode(INPUT8, INPUT);

  
  Wire.begin(16,17);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();

  adcAttachPin(32);

 

  
}


void loop() {
 
  Serial.println(""); 
  Serial.print(digitalRead(INPUT1));Serial.print(digitalRead(INPUT2));Serial.print(digitalRead(INPUT3));Serial.println(digitalRead(INPUT4));Serial.print(digitalRead(INPUT5));Serial.print(digitalRead(INPUT6));Serial.print(digitalRead(INPUT7));Serial.println(digitalRead(INPUT8));
  Serial.println(""); 

 
  Serial.print("Push button  ");Serial.println(readSwitch());
  Serial.println(""); 

  
  digitalWrite(OUTPUT1, HIGH);
  digitalWrite(OUTPUT2, LOW);
  digitalWrite(OUTPUT3, LOW);
  digitalWrite(OUTPUT4, LOW);
  digitalWrite(OUTPUT5, LOW);
  digitalWrite(OUTPUT6, LOW);
  digitalWrite(OUTPUT7, LOW);
  digitalWrite(OUTPUT8, LOW);
  delay(200);
  digitalWrite(OUTPUT1, LOW);
  digitalWrite(OUTPUT2, HIGH);
  digitalWrite(OUTPUT3, LOW);
  digitalWrite(OUTPUT4, LOW);
  digitalWrite(OUTPUT5, LOW);
  digitalWrite(OUTPUT6, LOW);
  digitalWrite(OUTPUT7, LOW);
  digitalWrite(OUTPUT8, LOW);
  delay(200);
  digitalWrite(OUTPUT1, LOW);
  digitalWrite(OUTPUT2, LOW);
  digitalWrite(OUTPUT3, HIGH);
  digitalWrite(OUTPUT4, LOW);
  digitalWrite(OUTPUT5, LOW);
  digitalWrite(OUTPUT6, LOW);
  digitalWrite(OUTPUT7, LOW);
  digitalWrite(OUTPUT8, LOW);
  delay(200);
  digitalWrite(OUTPUT1, LOW);
  digitalWrite(OUTPUT2, LOW);
  digitalWrite(OUTPUT3, LOW);
  digitalWrite(OUTPUT4, HIGH);
  digitalWrite(OUTPUT5, LOW);
  digitalWrite(OUTPUT6, LOW);
  digitalWrite(OUTPUT7, LOW);
  digitalWrite(OUTPUT8, LOW);
  delay(200);
  digitalWrite(OUTPUT1, LOW);
  digitalWrite(OUTPUT2, LOW);
  digitalWrite(OUTPUT3, LOW);
  digitalWrite(OUTPUT4, LOW);
  digitalWrite(OUTPUT5, HIGH);
  digitalWrite(OUTPUT6, LOW);
  digitalWrite(OUTPUT7, LOW);
  digitalWrite(OUTPUT8, LOW);
  delay(200);
  digitalWrite(OUTPUT1, LOW);
  digitalWrite(OUTPUT2, LOW);
  digitalWrite(OUTPUT3, LOW);
  digitalWrite(OUTPUT4, LOW);
  digitalWrite(OUTPUT5, LOW);
  digitalWrite(OUTPUT6, HIGH);
  digitalWrite(OUTPUT7, LOW);
  digitalWrite(OUTPUT8, LOW);
  delay(200);
  digitalWrite(OUTPUT1, LOW);
  digitalWrite(OUTPUT2, LOW);
  digitalWrite(OUTPUT3, LOW);
  digitalWrite(OUTPUT4, LOW);
  digitalWrite(OUTPUT5, LOW);
  digitalWrite(OUTPUT6, LOW);
  digitalWrite(OUTPUT7, HIGH);
  digitalWrite(OUTPUT8, LOW);
  delay(200);
  digitalWrite(OUTPUT1, LOW);
  digitalWrite(OUTPUT2, LOW);
  digitalWrite(OUTPUT3, LOW);
  digitalWrite(OUTPUT4, LOW);
  digitalWrite(OUTPUT5, LOW);
  digitalWrite(OUTPUT6, LOW);
  digitalWrite(OUTPUT7, LOW);
  digitalWrite(OUTPUT8, HIGH);
  delay(200);
  
digitalWrite(RS485_FC, HIGH);                    // Make FLOW CONTROL pin HIGH
  delay(500);
  Serial1.println(F("RS485 01 SUCCESS"));    // Send RS485 SUCCESS serially
  delay(500);                                // Wait for transmission of data
  digitalWrite(RS485_FC, LOW) ;                    // Receiving mode ON

                                             // Serial1.flush() ;
  delay(1000);     
  
  while (Serial1.available()) {  // Check if data is available
    char c = Serial1.read();     // Read data from RS485
    Serial.write(c);             // Print data on serial monitor
  }
 
}