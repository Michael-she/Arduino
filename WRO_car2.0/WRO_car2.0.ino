#include <Wire.h>


void setup() {
  // put your setup code here, to run once:
pinMode(25, OUTPUT);
digitalWrite(25, HIGH);
  Serial.begin(115200);
 Wire1.setSDA(18);
  Wire1.setSCL(19);
  Wire1.begin();


  
}

void loop() {
  // put your main code here, to run repeatedly:
for(int i = -10; i<=10; i++){
setMotor(i);
Serial.println(i);
delay(1000);

}
}


void setMotor(int8_t motorNum) {
  Serial.println("Transmitting Wire");
  Wire1.beginTransmission(13);


  byte motorState = motorNum;
  Wire1.write(motorState);


  Wire1.endTransmission();
}

void sendWireInt(int sendInt) {
  short int split = sendInt;
  byte b1 = split >> 8;
  byte b2 = split;
  Wire1.write(b1);
  Wire1.write(b2);
}

int recieveWireInt() {
  byte b1 = Wire1.read();
  byte b2 = Wire1.read();

  short int returnInt = b1;

  returnInt = returnInt << 8;
  returnInt += b2;

  return returnInt;
}