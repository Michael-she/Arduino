// Basic demo for readings from Adafruit BNO08x
#include <Adafruit_BNO08x.h>

// For SPI mode, we need a CS pin
#define BNO08X_CS 10
#define BNO08X_INT 9

// For SPI mode, we also need a RESET 
//#define BNO08X_RESET 5
// but not for I2C or UART
#define BNO08X_RESET -1

Adafruit_BNO08x  bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;

int seg1[] = { 9, 3, 6, 7, 8, 12, 11, 10 };

int seg2[] = {17, 13, 14, 15, 16, 22, 21, 20};
int trueAngle = 0;
int trueAngleOffset = 0;
int angle;
int angleOld = 9999;
void setup(void) {

   
  Serial.begin(115200);
  delay(1000);
  initSegs();
Serial.println("FLASH");

for (int i = 0; i < 10; i++) {

    updateSeg1(i);
    updateSeg2(i);
    delay(100);
  }

  updateSeg1(0);
  updateSeg2(0);
bootAimation();

  
  Serial.println("Adafruit BNO08x test!");
delay(500);
bootAimation();
  // Try to initialize!
  if (!bno08x.begin_I2C()) {
  //if (!bno08x.begin_UART(&Serial1)) {  // Requires a device with > 300 byte UART buffer!
  //if (!bno08x.begin_SPI(BNO08X_CS, BNO08X_INT)) {
    Serial.println("Failed to find BNO08x chip");
    delay(500);
     watchdog_enable(100, 1);
    while (1) {
       delay(10); 
   }
  }
  Serial.println("BNO08x Found!");

  for (int n = 0; n < bno08x.prodIds.numEntries; n++) {
    Serial.print("Part ");
    Serial.print(bno08x.prodIds.entry[n].swPartNumber);
    Serial.print(": Version :");
    Serial.print(bno08x.prodIds.entry[n].swVersionMajor);
    Serial.print(".");
    Serial.print(bno08x.prodIds.entry[n].swVersionMinor);
    Serial.print(".");
    Serial.print(bno08x.prodIds.entry[n].swVersionPatch);
    Serial.print(" Build ");
    Serial.println(bno08x.prodIds.entry[n].swBuildNumber);
  }

  setReports();

  Serial.println("Reading events");
  delay(100);
  for (int i = 0; i < 10; i++) {

    updateSeg1(i);
    updateSeg2(i);
    delay(50);
  }

  updateSeg1(0);
  updateSeg2(0);
}

// Here is where you define the sensor outputs you want to receive
void setReports(void) {
  Serial.println("Setting desired reports");
  if (! bno08x.enableReport(SH2_GAME_ROTATION_VECTOR)) {
    Serial.println("Could not enable game vector");
  }
}


void loop() {
  delay(10);

  if (bno08x.wasReset()) {
    Serial.print("sensor was reset ");
    setReports();
  }
  
  if (! bno08x.getSensorEvent(&sensorValue)) {
    return;
  }
  
  switch (sensorValue.sensorId) {
    
    case SH2_GAME_ROTATION_VECTOR:
      Serial.print("Game Rotation Vector - r: ");
      Serial.print(sensorValue.un.gameRotationVector.real);
      Serial.print(" i: ");
      Serial.print(sensorValue.un.gameRotationVector.i);
      Serial.print(" j: ");
      Serial.print(sensorValue.un.gameRotationVector.j);
      Serial.print(" k: ");
      int angle = round(sensorValue.un.gameRotationVector.k*180);
      Serial.println(trueAngle);
     

       if(angleOld != angle){

     if(angleOld >= 175 && angle <= -175){

       trueAngleOffset += 360;
     }
     if(angleOld <= -175 && angle  >= 175){

       trueAngleOffset -= 360;
     }

     trueAngle = angle+trueAngleOffset;

    updateSeg1(abs((trueAngle%100)/10));
    updateSeg2(abs(trueAngle%10));
    angleOld = angle;
  }
      break;
  }

}





void bootAimation() {

  for (int i = 0; i < 8; i++) {
    Serial.println(seg1[i]);
    digitalWrite(seg1[i], LOW);
    delay(50);
    digitalWrite(seg1[i], HIGH);
  }

  for (int i = 0; i < 8; i++) {
    Serial.println(seg2[i]);
    digitalWrite(seg2[i], LOW);
    delay(50);
    digitalWrite(seg2[i], HIGH);
  }
}



void updateSeg1(char displayNum) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(seg1[i], HIGH);
  }

  if (displayNum == 0) {
    digitalWrite(seg1[0], LOW);
    digitalWrite(seg1[1], LOW);
    digitalWrite(seg1[7], LOW);
    digitalWrite(seg1[3], LOW);
    digitalWrite(seg1[4], LOW);
    digitalWrite(seg1[6], LOW);
  } else if (displayNum == 1) {
    digitalWrite(seg1[6], LOW);
    digitalWrite(seg1[7], LOW);
  } else if (displayNum == 2) {
    digitalWrite(seg1[0], LOW);
    digitalWrite(seg1[7], LOW);
    digitalWrite(seg1[2], LOW);
    digitalWrite(seg1[3], LOW);
    digitalWrite(seg1[4], LOW);
  } else if (displayNum == 3) {
    digitalWrite(seg1[0], LOW);
    digitalWrite(seg1[7], LOW);
    digitalWrite(seg1[2], LOW);
    digitalWrite(seg1[6], LOW);
    digitalWrite(seg1[4], LOW);
  } else if (displayNum == 4) {
    digitalWrite(seg1[1], LOW);
    digitalWrite(seg1[7], LOW);
    digitalWrite(seg1[2], LOW);
    digitalWrite(seg1[6], LOW);
  } else if (displayNum == 5) {
    digitalWrite(seg1[0], LOW);
    digitalWrite(seg1[1], LOW);
    digitalWrite(seg1[2], LOW);
    digitalWrite(seg1[6], LOW);
    digitalWrite(seg1[4], LOW);
  } else if (displayNum == 6) {
    digitalWrite(seg1[0], LOW);
    digitalWrite(seg1[1], LOW);
    digitalWrite(seg1[2], LOW);
    digitalWrite(seg1[3], LOW);
    digitalWrite(seg1[6], LOW);
    digitalWrite(seg1[4], LOW);
  } else if (displayNum == 7) {
    digitalWrite(seg1[0], LOW);
    digitalWrite(seg1[7], LOW);
    digitalWrite(seg1[6], LOW);
  } else if (displayNum == 8) {
    digitalWrite(seg1[0], LOW);
    digitalWrite(seg1[1], LOW);
    digitalWrite(seg1[2], LOW);
    digitalWrite(seg1[3], LOW);
    digitalWrite(seg1[4], LOW);
    digitalWrite(seg1[6], LOW);
    digitalWrite(seg1[7], LOW);
  } else if (displayNum == 9) {
    digitalWrite(seg1[0], LOW);
    digitalWrite(seg1[1], LOW);
    digitalWrite(seg1[2], LOW);
    digitalWrite(seg1[7], LOW);
    digitalWrite(seg1[6], LOW);
  } else if (displayNum == 'C') {
    digitalWrite(seg1[0], LOW);
    digitalWrite(seg1[1], LOW);
    digitalWrite(seg1[3], LOW);
    digitalWrite(seg1[4], LOW);
 
 } else if (displayNum == 'F') {
    digitalWrite(seg1[0], LOW);
    digitalWrite(seg1[1], LOW);
    digitalWrite(seg1[3], LOW);
    digitalWrite(seg1[2], LOW);
 } else if (displayNum == '.') {
    digitalWrite(seg1[5], LOW);
  }  
}



void updateSeg2(char displayNum) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(seg2[i], HIGH);
  }

  if (displayNum == 0) {
    digitalWrite(seg2[0], LOW);
    digitalWrite(seg2[1], LOW);
    digitalWrite(seg2[7], LOW);
    digitalWrite(seg2[3], LOW);
    digitalWrite(seg2[4], LOW);
    digitalWrite(seg2[6], LOW);
  } else if (displayNum == 1) {
    digitalWrite(seg2[6], LOW);
    digitalWrite(seg2[7], LOW);
  } else if (displayNum == 2) {
    digitalWrite(seg2[0], LOW);
    digitalWrite(seg2[7], LOW);
    digitalWrite(seg2[2], LOW);
    digitalWrite(seg2[3], LOW);
    digitalWrite(seg2[4], LOW);
  } else if (displayNum == 3) {
    digitalWrite(seg2[0], LOW);
    digitalWrite(seg2[7], LOW);
    digitalWrite(seg2[2], LOW);
    digitalWrite(seg2[6], LOW);
    digitalWrite(seg2[4], LOW);
  } else if (displayNum == 4) {
    digitalWrite(seg2[1], LOW);
    digitalWrite(seg2[7], LOW);
    digitalWrite(seg2[2], LOW);
    digitalWrite(seg2[6], LOW);
  } else if (displayNum == 5) {
    digitalWrite(seg2[0], LOW);
    digitalWrite(seg2[1], LOW);
    digitalWrite(seg2[2], LOW);
    digitalWrite(seg2[6], LOW);
    digitalWrite(seg2[4], LOW);
  } else if (displayNum == 6) {
    digitalWrite(seg2[0], LOW);
    digitalWrite(seg2[1], LOW);
    digitalWrite(seg2[2], LOW);
    digitalWrite(seg2[3], LOW);
    digitalWrite(seg2[6], LOW);
    digitalWrite(seg2[4], LOW);
  } else if (displayNum == 7) {
    digitalWrite(seg2[0], LOW);
    digitalWrite(seg2[7], LOW);
    digitalWrite(seg2[6], LOW);
  } else if (displayNum == 8) {
    digitalWrite(seg2[0], LOW);
    digitalWrite(seg2[1], LOW);
    digitalWrite(seg2[2], LOW);
    digitalWrite(seg2[3], LOW);
    digitalWrite(seg2[4], LOW);
    digitalWrite(seg2[6], LOW);
    digitalWrite(seg2[7], LOW);
  } else if (displayNum == 9) {
    digitalWrite(seg2[0], LOW);
    digitalWrite(seg2[1], LOW);
    digitalWrite(seg2[2], LOW);
    digitalWrite(seg2[7], LOW);
    digitalWrite(seg2[6], LOW);
  } else if (displayNum == 'C') {
    digitalWrite(seg2[0], LOW);
    digitalWrite(seg2[1], LOW);
    digitalWrite(seg2[3], LOW);
    digitalWrite(seg2[4], LOW);
 
 } else if (displayNum == 'F') {
    digitalWrite(seg2[0], LOW);
    digitalWrite(seg2[1], LOW);
    digitalWrite(seg2[3], LOW);
    digitalWrite(seg2[2], LOW);
 } else if (displayNum == '.') {
    digitalWrite(seg2[5], LOW);
  }  
}


void initSegs(){

    pinMode(seg1[0], OUTPUT);
  pinMode(seg1[1], OUTPUT);
  pinMode(seg1[2], OUTPUT);
  pinMode(seg1[3], OUTPUT);
  pinMode(seg1[4], OUTPUT);
  pinMode(seg1[5], OUTPUT);
  pinMode(seg1[6], OUTPUT);
  pinMode(seg1[7], OUTPUT);


    pinMode(seg2[0], OUTPUT);
  pinMode(seg2[1], OUTPUT);
  pinMode(seg2[2], OUTPUT);
  pinMode(seg2[3], OUTPUT);
  pinMode(seg2[4], OUTPUT);
  pinMode(seg2[5], OUTPUT);
  pinMode(seg2[6], OUTPUT);
  pinMode(seg2[7], OUTPUT);

   for (int i = 0; i < 8; i++) {
    Serial.println(i);

    digitalWrite(seg1[i], HIGH);
  }
   for (int i = 0; i < 8; i++) {
    Serial.println(i);

    digitalWrite(seg2[i], HIGH);
  }
}