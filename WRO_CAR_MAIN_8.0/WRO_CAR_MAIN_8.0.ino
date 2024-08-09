

#include <Wire.h>



#include <Arduino.h>

#include <TFLI2C.h>  // TFLuna-I2C Library v.0.1.1
TFLI2C tflI2C;

int blocks[4][6];
int currentSection = 0;


int cameraContents[3][5];
int numShots = 2;

int8_t front = 0x61;
int8_t left = 0x62;
int8_t right = 0x60;

int midline = 50;
int hugWallDist = 24;

boolean turnRight = false;

int targetAngle = 0;
int trueAngle = 0;
int angleAjustment = 90;

float wallThreshHold = 2;

int lane = 0; //Left or right lane. 0 is left and 1 is right
int currentLane = 0;

int turningCircle = 10;



void setup() {
  // put your setup code here, to run once:
  pinMode(25, OUTPUT);
  pinMode(6, INPUT);
  digitalWrite(25, HIGH);
  Serial.begin(115200);
  Wire1.setSDA(2);
  Wire1.setSCL(3);
  Wire1.begin();


  Wire.setSDA(4);
  Wire.setSCL(5);
  Wire.begin();


  for (int i = 0 ; i <4; i++){
    for (int j = 0 ; j<6; j++){

      blocks[i][j] = 0;

    }


  }

  setAngle(0);
  setMotor(0);
  if (getAngle() == 0) {
    Serial.println("Gyro Self Check");
  }

  while(!checkMotor()){
    delay(1000);
  }

  //sampleCommands( 0x60);
}

void loop() {

 
  waitForButton();
   updateCurrentLane();
  lane = currentLane;
 //targetAngle = 0;
 setAngle(targetAngle);
 turnRight = false;
 
 while(true){
  midBlockHandling();
  setMotor(7);

  transferLanesFrontSensor();
  waitForWall();
  waitForWallToBeGone();
  setMotor(7);
  leftTurn();
  setMotor(7);
  waitForWall();
  setMotor(0);

 }
 
 

// getCamera();
// printCamera();
// delay(100);
}

bool checkMotor(){

  Wire1.requestFrom(13, 1);
  int motorReturn = Wire1.read();

  if (motorReturn == 2){
    Serial.println("Good motor");
    return true;
  }else{
    Serial.println("No Motor Response");
    return false;
  }


}

void waitForButton(){
  delay(15000);
    setAngle(90);
    delay(500);
    setAngle(0);
    setMotor(1);
    delay(2000);
    setMotor(0);
    setAngle(-90);
    delay(500);
    setAngle(0);
    delay(500);

  while(digitalRead(6) == LOW){

  Serial.print(getAngle());
  delay(10);

  }
  Serial.println("");
  Serial.println("Button Pressed");
  delay(1000);

}

void getCamera(){

  Wire1.requestFrom(14, 30);

  for (int i = 0 ; i<2; i++){
    for (int j = 0; j<5; j++){

      cameraContents[i][j] = recieveInt();

    }
  }

  }

  char getNearestBlockColour(){

  if (cameraContents[0][0] == 82 || cameraContents[0][0] == 71){

  return (char)cameraContents[0][0];

  }

  return (char)cameraContents[1][0];

}

void handleMagenta(){
  if (cameraContents[0][0] == 77){
    blocks [currentSection][3] = 77;
    blocks [currentSection][4] = 77;
    blocks [currentSection][5] = 77;

    for (int i = 0; i<5; i++){
    cameraContents[0][i] = cameraContents[1][i];
    }
  }else if (cameraContents[1][0] == 77 || cameraContents[2][0] == 77 ){

    blocks [currentSection][3] = 77;
    blocks [currentSection][4] = 77;
    blocks [currentSection][5] = 77;

  }



}
void rightTurn(){




}
void leftTurn(){
    if (lane == 0){
      targetAngle;
    setAngle(targetAngle+90);
    //waitForAngle(targetAngle+20);

    setMotor(0);
    delay(1000);
    getCamera();
    }else {
      setAngle(targetAngle+90);
      waitForAngle(targetAngle+60);
      setMotor(0);
      delay(500);
      getCamera();
    }

    if (cameraContents[0][0] == 71){
      Serial.println("GREEN");
      lane = 0;
      currentLane = 0;
      blocks[currentSection][0] = 71;

    }else if (cameraContents[0][0] == 82){
      blocks[currentSection][0] = 82;
      Serial.println("RED");
      printCamera();
      setAngle(targetAngle-60);
      setMotor(7);
      delay(1500);
      lane = 1;
      currentLane = 1;
      setAngle(targetAngle);
      waitForFrontWall(hugWallDist);
      

    }else{
    Serial.println("NIKS");
    setAngle(targetAngle);

    setMotor(7);


    delay(1380);
    lane = 3; 
    currentLane = 3;

  }


  targetAngle+=90;
  setAngle(targetAngle);


  setMotor(7);

  //waitForBlockToPass();
  waitForTargetAngle();

  

  setMotor(0);

}

void midBlockHandling(){



  getCamera();

  if (cameraContents[0][0] == 71){
  Serial.println("GREEN");
  lane = 0;
  
  blocks[currentSection][3] = 71;

}else if (cameraContents[0][0] == 82){
  Serial.println("RED");
  lane = 1;
  
  blocks[currentSection][3] = 82;

}else{

  lane = 3;
  currentLane = 3;

}
}

void transferLanesFrontSensor(){
 
 //This currently works:

 if (lane != currentLane){
    Serial.print(currentLane);
    Serial.print(" -> ");
    Serial.println(lane);

    int8_t sensor = 0;
    
    int ajustment = 0;
    if(lane == 1){

      ajustment = targetAngle-angleAjustment;

      sensor = left;
      

    }else{
    ajustment = targetAngle+angleAjustment;
    sensor = right;
    
    }

    // Serial.println(getAngle());
    // Serial.println(getAngle());
    // Serial.println(getAngle());
    // Serial.println(getAngle());

    // Serial.println(getAngle());
    // Serial.println(getAngle());

    setAngle(ajustment);
   
    
  waitForFrontWall(hugWallDist);
  setAngle(targetAngle);
  waitForTargetAngle();
  currentLane = lane;
  }

}

void waitForFrontWall(int targetDistance){

  int frontDist = getDistance(front);
    while(frontDist>targetDistance | frontDist == -1 | frontDist == 0){
  //while (getDistance(front) >10 || getDistance(front)==-1){
    frontDist = getDistance(front);
    Serial.print("Distance to Side ");
    Serial.println(frontDist);
    delay(10);
  }
  Serial.println(getDistance(front));

}



void waitForBlockToPass(){


  int8_t sensor = 0;
  if(lane = 0){
  sensor = right;

  }else{
    sensor = left;
  }


  int maxDist = getDistance(sensor);

  int frontDist = getDistance(front);
  bool blockEncountered = false;
  if(lane==3){
  blockEncountered = true;
  }

  while(!blockEncountered){

  int currentDist = getDistance(sensor);

      if (currentDist>maxDist){
        maxDist = currentDist;
      }

      if (currentDist < maxDist-10){
        while(currentDist < maxDist-10){
          currentDist = getDistance(sensor);
          blockEncountered = true;
          delay(10);
        }
      }

  }




}

int countBlockPositions(){

  int8_t sensor = 0;
  if(turnRight){
  sensor = right;

  }else{
    sensor = left;
  }

  int numBlocks = 0;
  int maxDist = getDistance(sensor);

  int frontDist = getDistance(front);

  while(frontDist>25 || frontDist == -1){

  frontDist = getDistance(front);

  int currentDist = getDistance(sensor);

      if (currentDist>maxDist){
        maxDist = currentDist;
      }

      if (currentDist < maxDist-50){
        while(currentDist < maxDist-50){
          currentDist = getDistance(sensor);
          delay(10);
        }
  numBlocks++;
      }

  Serial.println(numBlocks);
  }

  return numBlocks;
}



void waitForWallToBeGone(){
    int8_t sensor = 0;
    if (turnRight){
    sensor = right;

    }else{
      sensor = left;
    }

    int sensorDist = getDistance(sensor);


    bool wall = true;

    int counter = 0;
    while(wall){
      delay(10);
      sensorDist = getDistance(sensor);

      if(getDistance(sensor)>150){
        counter++;
      }else{
        counter = 0;
      }

      if (counter == numShots){
        wall = false;
      }
}


}

void printCamera(){
 Serial.println("------------------");
  for (int i = 0 ; i<3; i++){
      Serial.print("Colour: ");
      if (cameraContents[i][0] == 82){
        Serial.println("Red");
      }
      if (cameraContents[i][0] == 71){
        Serial.println("Green");
      }
      if (cameraContents[i][0] == 77){
        Serial.println("Magenta");
      }

      Serial.print("\tX: ");
      Serial.println(cameraContents[i][1]);

      Serial.print("\tY: ");
      Serial.println(cameraContents[i][2]);

      Serial.print("\tWidth: ");
      Serial.println(cameraContents[i][3]);

      Serial.print("\tHeight: ");
      Serial.println(cameraContents[i][4]);

      



  }

}


void waitForTargetAngle() {
  setAngle(targetAngle);
 Serial.print("Waiting for angle...   ");
    Serial.print(getAngle());
    Serial.print(" / ");
    Serial.println(targetAngle);

  while (getAngle() > targetAngle + 2 || getAngle() < targetAngle - 2) {
    delay(10);

    Serial.print("Waiting for angle...   ");
    Serial.print(getAngle());
    Serial.print(" / ");
    Serial.println(targetAngle);
  }
}

void waitForAngle(int angle) {
  

  while (getAngle() > angle + 2 || getAngle() < angle - 2) {
    delay(10);

    Serial.print("Waiting for angle...   ");
    Serial.print(getAngle());
    Serial.print(" / ");
    Serial.println(angle);
  }
}


void waitForWall(){


  int rightCounter = 0;
  int leftCounter = 0;
  bool wallFound = false;
  while(!wallFound){

    int frontDist = getDistance(front);

 if (turnRight) {
      int rightDist = getDistance(right);

      Serial.println(rightDist);


      if (rightDist < 100) {
        rightCounter++;
      } else {
        rightCounter = 0;
        
      }
      if (rightCounter == numShots*20 && frontDist < 210) {
        wallFound = true;
      }




    } else {
      int leftDist = getDistance(left);


      Serial.println(leftDist);

      if (leftDist < 100) {
        leftCounter++;
      } else {
        leftCounter = 0;

      }



      if (leftCounter == numShots*20 && frontDist < 210) {
        wallFound = true;
      
      }
    }

  }
  Serial.println("Wall spotted");
}
void setAngle(int targetAngleLocal) {
  getAngle();
   Serial.print("Setting Angle: ");
    Serial.println(targetAngleLocal);

  Wire1.beginTransmission(12);
  sendInt(targetAngleLocal);
  Wire1.endTransmission();
}

int getAngle() {
  Wire1.requestFrom(12, 2);
  trueAngle = recieveInt();
  return trueAngle;
}

int getDistance(int8_t tfAddr) {
  int16_t tfDist = 0;
  if (tflI2C.getData(tfDist, tfAddr))  // If read okay...
  {
    // Serial.print("Dist: ");
    //Serial.println(tfDist);          // print the data...
    return tfDist;
  }
  return -1;
}




void setMotor(int8_t motorNum) {
  Serial.print("Transmitting Wire  ");
  Serial.println(motorNum);
  Wire1.beginTransmission(13);


  int16_t motorState = motorNum;
 sendInt(motorState);


     byte error = Wire1.endTransmission();
    Serial.println(error);


}

void sendInt(int sendInt) {
  int16_t split = sendInt;
 // Serial.println(split, BIN);
  byte b1 = split >> 8;
  byte b2 = split;
  // Serial.print(b1, BIN);
  // Serial.print(" | ");
  // Serial.println(b2, BIN);
  Wire1.write(b1);
  Wire1.write(b2);
}

int recieveInt() {
  byte b1 = Wire1.read();
  byte b2 = Wire1.read();

  short int returnInt = b1;

  returnInt = returnInt << 8;
  returnInt += b2;

  return returnInt;
}


void updateCurrentLane(){

if(getDistance(left)>getDistance(right)){
  currentLane = 1;
}else{
  currentLane = 0;
}


}