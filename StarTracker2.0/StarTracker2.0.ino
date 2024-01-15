#include <Arduino.h>


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels



#include <HardwareSerial.h>

#define OLED_RESET     -1

#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



//Remote pins

const int joyX = 27;
const int joyY = 26;

const int btnTop = 19;
const int btnMid = 20;
const int btnBtm = 21;

const int modeSW = 22;

const int yawLimitPin = 18;

const int pitchLimitPin  = 15;

float stationLongitude = 0;
float stationLatitude = 0;
float altitude = 0;

float ISSLongitude = 0;
float ISSLatitude = 0;
float ISSAltitude = 0;



// Motor 1 (Roll) - Pins
const int motor1Pin1 = 6;
const int motor1Pin2 = 7;
const int motor1Pin3 = 8;
const int motor1Pin4 = 9;

// Motor 2 (Pitch) - Pins
const int motor2Pin1 = 10;
const int motor2Pin2 = 11;
const int motor2Pin3 = 12;
const int motor2Pin4 = 13;

const int stepperEnaPin = 14;

// Stepper motor steps per revolution
const int stepsPerRevolution = 512;  // Update this if microstepping
const float maxRotation = 1 * 360; // Maximum rotation angle in degrees

// Current position of motors
float currentPosRoll = 0;
float currentPosPitch = 90;

// Control sequence for 4-step motor
int motorSteps[4][4] = {
  {HIGH, LOW, LOW, LOW},
  {LOW, HIGH, LOW, LOW},
  {LOW, LOW, HIGH, LOW},
  {LOW, LOW, LOW, HIGH}
};

void setup() {

   if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Display initialisation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();

 display.setTextSize(4);
  display.setTextColor(WHITE);

display.setCursor(0, 0);
display.print("Boot");

display.display();
  // Initialize motor control pins as outputs
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor1Pin3, OUTPUT);
  pinMode(motor1Pin4, OUTPUT);

  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(motor2Pin3, OUTPUT);
  pinMode(motor2Pin4, OUTPUT);

pinMode(stepperEnaPin, OUTPUT);

pinMode(modeSW, INPUT);


pinMode(btnTop, INPUT);
pinMode(btnMid, INPUT);
pinMode(btnBtm, INPUT);


pinMode(joyX, INPUT);
pinMode(joyY, INPUT);

   delay(3000);
  // Start Serial communication at 9600 baud rate
  Serial.begin(115200);
  Serial.print("INIT ");
  Serial1.setFIFOSize(6400);
  Serial1.setPinout(16, 17);
Serial1.begin(9600);
 Serial.println("INIT ");




 


Serial.println("Display initialisation sucsess");

home();
}

void loop() {
  // Example usage
  

manaulControl();
 getISS();
  delay(1000); // Delay between sequences
}

void stepMotor(int motorPins[], int speed, float currentAngle, float targetAngle, float &currentPos) {
  float angleDifference = targetAngle - currentAngle;
  // Determine the shortest path and account for max rotation

  if (abs(angleDifference) > maxRotation) angleDifference = (angleDifference > 0) ? 0 : maxRotation;

  int direction = angleDifference > 0 ? 0 : 1;
  int stepsRequired = abs(angleDifference) / 360.0 * stepsPerRevolution;

  for (int steps = 0; steps < stepsRequired; steps++) {
    for (int index = 0; index < 4; index++) {
      for (int pin = 0; pin < 4; pin++) {
        digitalWrite(motorPins[pin], direction ? motorSteps[index][pin] : motorSteps[3 - index][pin]);
      }
      delayMicroseconds(speed);
    }
  }



  // Update current position
  currentPos = currentPos - angleDifference;
  
}


void home(){
  digitalWrite(stepperEnaPin, HIGH);
    display.clearDisplay();

 display.setTextSize(4);
  display.setTextColor(WHITE);

display.setCursor(0, 0);
display.print("HOME");

display.display();
   int motor1Pins[4] = {motor1Pin1, motor1Pin2, motor1Pin3, motor1Pin4};

  while(digitalRead(yawLimitPin)==LOW) {
    for (int index = 0; index < 4; index++) {
      for (int pin = 0; pin < 4; pin++) {
        digitalWrite(motor1Pins[pin], motorSteps[3-index][pin]);
      }
      delayMicroseconds(30000);
    }
  }
int motor2Pins[4] = {motor2Pin1, motor2Pin2, motor2Pin3, motor2Pin4};

while(digitalRead(pitchLimitPin)==LOW) {
    for (int index = 0; index < 4; index++) {
      for (int pin = 0; pin < 4; pin++) {
        digitalWrite(motor2Pins[pin], motorSteps[3-index][pin]);
      }
      delayMicroseconds(30000);
    }
  }
  digitalWrite(stepperEnaPin, LOW);
}

void stepRoll(int speed, float targetAngle) {
  int motor1Pins[4] = {motor1Pin1, motor1Pin2, motor1Pin3, motor1Pin4};
  stepMotor(motor1Pins, speed, currentPosRoll, targetAngle, currentPosRoll);


  if(currentPosRoll>370){

int tA = currentPosRoll+360;
stepMotor(motor1Pins, 3000, currentPosRoll, tA, currentPosRoll);


  }
  if(currentPosRoll<-10){

int tA = currentPosRoll-360;
stepMotor(motor1Pins, 3000, currentPosRoll, tA, currentPosRoll);


  }
}

void stepPitch(int speed, float targetAngle) {
  int motor2Pins[4] = {motor2Pin1, motor2Pin2, motor2Pin3, motor2Pin4};
  stepMotor(motor2Pins, speed, currentPosPitch, targetAngle, currentPosPitch);
}




void getISS() {
  // Send the string to the Serial port
  Serial1.println("||<<||<<GET,https://api.wheretheiss.at/v1/satellites/25544>>||>>||");

  // Wait for a response for a maximum of 5 seconds
  unsigned long startTime = millis();
  while (!Serial1.available() && millis() - startTime < 20000) {
  
  }

  // Check if data is available to read
  if (Serial1.available()) {
    // Read the response and print it
    
    String response = readSerialData();
  //  Serial.print("Response: ");
    Serial.println(response);

  if(response.indexOf("ERROR")== -1){

     double observerLatitude = -34.056295050573176;
    double observerLongitude = 18.849344674375637;
    double observerAltitude = 256;


    String issLatStr = response.substring(response.indexOf("latitude")+10, response.indexOf("longitude")-2);
Serial.println(issLatStr);
    String issLongStr = response.substring(response.indexOf("longitude")+11, response.indexOf("altitude")-2);
Serial.println(issLongStr);
  String issAltStr = response.substring(response.indexOf("altitude")+10, response.indexOf("velocity")-2);
Serial.println(issAltStr);
    double satelliteLatitude = issLatStr.toDouble();
    double satelliteLongitude =  issLongStr.toDouble();
    double satelliteAltitude = issAltStr.toDouble()*1000;

    double azimuth, elevation;

    calculateAzimuthElevation(observerLatitude, observerLongitude, observerAltitude,
                              satelliteLatitude, satelliteLongitude, satelliteAltitude,
                              azimuth, elevation);

    Serial.print("Azimuth: ");
    Serial.println(azimuth);
    Serial.print("Elevation: ");
    Serial.println(elevation);
    


  display.clearDisplay();

 display.setTextSize(1);
  display.setTextColor(WHITE);

display.setCursor(0, 0);
display.print("Azimuth: ");

display.print(azimuth);

display.setCursor(0, 8);
display.print("Elevation: ");

display.print(elevation);

display.setCursor(0, 16);
display.print("Yaw: ");

display.print(currentPosRoll);

display.setCursor(0, 24);
display.print("Pitch: ");

display.print(currentPosPitch);

display.display();

if(elevation>0){
digitalWrite(stepperEnaPin, HIGH);
    delay(300);
 stepRoll(10000, azimuth); // Move Roll motor to 90 degrees at speed 1000 microseconds
  stepPitch(10000, elevation); // Move Pitch motor to 45 degrees at speed 1000 microseconds
  delay(100);
  digitalWrite(stepperEnaPin, LOW);
  }}else{
    Serial.println("Error Detected");
     readSerialData();

  }

  

  } else {
    // If no response in 5 seconds
    Serial.println("No response in 5 seconds.");
  }

  // Wait for 5 seconds before sending the string again
 delay(1000);
}








void gpsToCartesian(double lat, double lon, double alt, double &X, double &Y, double &Z) {
    double latRad = lat * M_PI / 180.0; // Convert latitude to radians
    double lonRad = lon * M_PI / 180.0; // Convert longitude to radians


    Serial.print("LRAD: ");
    Serial.println(latRad);
    Serial.print("LONRAD: ");
    Serial.println(lonRad);
    double R = 6378137.0; // Earth's radius at sea level in meters

    // Calculate Cartesian coordinates
    X = (R + alt) * cos(latRad) * cos(lonRad);
   
    Y = (R + alt) * cos(latRad) * sin(lonRad);
    Z = (R + alt) * sin(latRad);

      Serial.print("X: ");
    Serial.print(X);
          Serial.print("Y: ");
    Serial.print(Y);
    Serial.print("   Z: ");
    Serial.println(Z);
}

void calculateAzimuthElevation(double observerLat, double observerLon, double observerAlt,
                               double satelliteLat, double satelliteLon, double satelliteAlt,
                               double &azimuth, double &elevation) {
    double observerX, observerY, observerZ;
    double satelliteX, satelliteY, satelliteZ;

    // Convert observer and satellite coordinates to Cartesian
    gpsToCartesian(observerLat, observerLon, observerAlt, observerX, observerY, observerZ);
    gpsToCartesian(satelliteLat, satelliteLon, satelliteAlt, satelliteX, satelliteY, satelliteZ);

    // Calculate relative position in a local tangent plane
    double deltaX = satelliteX - observerX;
    double deltaY = satelliteY - observerY;
    double deltaZ = satelliteZ - observerZ;

    // Convert observer latitude and longitude to radians
    double observerLatRad = observerLat * M_PI / 180.0;
    double observerLonRad = observerLon * M_PI / 180.0;

    // Calculate the rotation matrix for the observer's local tangent plane
    double rotMatrix[3][3] = {
        {-sin(observerLonRad), cos(observerLonRad), 0},
        {-sin(observerLatRad) * cos(observerLonRad), -sin(observerLatRad) * sin(observerLonRad), cos(observerLatRad)},
        {cos(observerLatRad) * cos(observerLonRad), cos(observerLatRad) * sin(observerLonRad), sin(observerLatRad)}
    };

    // Rotate the relative position into the observer's local tangent plane
    double localX = rotMatrix[0][0] * deltaX + rotMatrix[0][1] * deltaY + rotMatrix[0][2] * deltaZ;
    double localY = rotMatrix[1][0] * deltaX + rotMatrix[1][1] * deltaY + rotMatrix[1][2] * deltaZ;
    double localZ = rotMatrix[2][0] * deltaX + rotMatrix[2][1] * deltaY + rotMatrix[2][2] * deltaZ;

    // Calculate elevation
    elevation = atan2(localZ, sqrt(localX * localX + localY * localY)) * 180.0 / M_PI;

    // Calculate azimuth
    azimuth =360 -( atan2(localY, localX) * 180.0 / M_PI -90);
    if (azimuth < 0) {
        azimuth += 360.0; // Normalize to [0, 360] degrees


    }
     if (azimuth > 360) {
       azimuth -= 360.0; // Normalize to [0, 360] degrees
       
    }
}


String readSerialData() {
  String receivedString = "";  // String to hold incoming data
  char incomingChar;

  // Wait for data to start coming in
  while (Serial1.available() == 0) {}

  // Read characters into our string until we get a newline
  while (Serial1.available() > 0) {
    incomingChar = Serial1.read();  // Read a single character

    // Check for end of transmission (you can define your own end-of-transmission character)
    if (incomingChar == '\n') {
      break;  // Exit the loop once we receive the newline character
    }

    receivedString += incomingChar;  // Add the character to our string
    delay(10);  // Short delay to allow buffer to fill
  }

  return receivedString;  // Return the complete string
}



void manaulControl(){



bool switchState = digitalRead(modeSW);

  if(switchState){

  display.clearDisplay();
  display.setTextSize(4);
    display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("MAN MODE");

  display.display();
 delay(1000);
    digitalWrite(stepperEnaPin, HIGH);

  

 
  while(switchState){


  int joyXReading = analogRead(joyX);

  int joyYReading = analogRead(joyY);
  switchState = digitalRead(modeSW);

  display.clearDisplay();
  display.setTextSize(1);
    display.setTextColor(WHITE);
  display.setCursor(0, 0);

  display.print("Yaw: ");

  display.print(currentPosRoll);

  display.setCursor(0, 8);
  display.print("Pitch: ");

  display.print(currentPosPitch);

  display.setCursor(0, 16);
  display.print("INPUT: ");

  display.print(joyXReading);
  display.setCursor(0, 24);
  display.print(" ");
  display.print(joyYReading);
  display.display();


  if(joyXReading>600){
    int speed=30000;
 if(joyXReading>800){
  speed=10000;
 }
if(joyXReading>9000){
speed=6000;

 }
 if(joyXReading>1000){
speed=3000;

 }



    
    stepRoll(speed, currentPosRoll-1);
    
  }

   if(joyXReading<400){
 int speed=30000;

    if(joyXReading<300){
  speed=10000;
 }

  if(joyXReading<200){
  speed=6000;
 }

 if(joyXReading<100){
speed=3000;

 }

   
    stepRoll(speed, currentPosRoll+1);
    
  }

   if(joyYReading>600){
    int speed=30000;

     if(joyYReading>800){
  speed=10000;
 }
if(joyYReading>9000){
speed=6000;

 }
 if(joyYReading>1000){
speed=3000;

 }
    stepPitch(speed, currentPosPitch+1);
    
  }

   if(joyYReading<400){
    int speed=30000;

       if(joyYReading<300){
  speed=10000;
 }

  if(joyYReading<200){
  speed=6000;
 }

 if(joyYReading<100){
speed=3000;

 }
    stepPitch(speed, currentPosPitch-1);
    
  }
  }



  delay(100);
    digitalWrite(stepperEnaPin, LOW);

  display.clearDisplay();
  display.setTextSize(4);
    display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("AUTO");

  display.display();




}







  }
