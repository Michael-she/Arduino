// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI Peripheral device
// Refer to the "Wire Peripheral Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>


#include <TFLI2C.h>  // TFLuna-I2C Library v.0.1.1
TFLI2C tflI2C;

void setup()
{
 // put your setup code here, to run once:

  Serial.begin(115200);
  Wire1.setSDA(2);
  Wire1.setSCL(3);
  Wire1.begin();


  Wire.setSDA(4);
  Wire.setSCL(5);
  Wire.begin();


  pinMode(2, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0 ; i<8; i++){
 setMotor(i);
 delay(2000);
  }
  setMotor(0);
 delay(2000);
 for(int i = 0; i>-8; i--){
   setMotor(i);
 delay(2000);
 }
 
}
void setMotor(byte x)
{
  Wire1.beginTransmission(4); // transmit to device #4
  Wire1.write("x is hohohehe");        // sends five bytes
  Wire1.write(x);              // sends one byte  
  Wire1.endTransmission();    // stop transmitting

  x++;
  digitalWrite(25, LOW);
  delay(500);
  
  digitalWrite(25, HIGH);
  delay(500);
}
