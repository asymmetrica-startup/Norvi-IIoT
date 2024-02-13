#include <ModbusMaster.h>                             //Library for using ModbusMaster
#define FC 4                                          //RE & DE are connected with the GPIO4 (for flow control) of the Norvi ESP32 Device.
ModbusMaster node;                                    //object node for class ModbusMaster

void preTransmission()                                //Function for setting stste of Flow control pin of RS-485
{
  digitalWrite(FC, 1);
}

void postTransmission()
{
  digitalWrite(FC, 0);
}
void setup()
{
  pinMode(FC, OUTPUT);
  digitalWrite(FC, 0);
  Serial.begin(9600);                                //Baud Rate as 9600
  pinMode(18, INPUT);                                //Initialize the inputs and outputs
  pinMode(39, INPUT);
  pinMode(34, INPUT);
  pinMode(35, INPUT);
  pinMode(19, INPUT);
  pinMode(21, INPUT);
  pinMode(22, INPUT);
  pinMode(23, INPUT);
  node.begin(1, Serial);                              //Slave ID as 1
  node.preTransmission(preTransmission);              //Callback for configuring RS-485 Transreceiver correctly
  node.postTransmission(postTransmission);
}

void loop()
{
    node.writeSingleRegister(0x40000, digitalRead(18));             //Writes value to 0x40000 holding register
                                                               
    node.writeSingleRegister(0x40001,digitalRead(39) );             //Writes value to 0x40001 holding register

    node.writeSingleRegister(0x40002, digitalRead(34));             //Writes value to 0x40002 holding register
  
    node.writeSingleRegister(0x40003, digitalRead(35));             //Writes value to 0x40003 holding register
  
    node.writeSingleRegister(0x40004, digitalRead(19));             //Writes value to 0x40004 holding register

    node.writeSingleRegister(0x40005, digitalRead(21));

    node.writeSingleRegister(0x40006, digitalRead(22));

    node.writeSingleRegister(0x40007, digitalRead(23));

    delay(1000);
}