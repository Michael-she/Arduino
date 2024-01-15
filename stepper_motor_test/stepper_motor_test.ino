#include <Arduino.h>


#include <HardwareSerial.h>

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

// Stepper motor steps per revolution
const int stepsPerRevolution = 512;  // Update this if microstepping
const float maxRotation = 1.5 * 360; // Maximum rotation angle in degrees

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
  // Initialize motor control pins as outputs
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor1Pin3, OUTPUT);
  pinMode(motor1Pin4, OUTPUT);

  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(motor2Pin3, OUTPUT);
  pinMode(motor2Pin4, OUTPUT);

   delay(3000);
  // Start Serial communication at 9600 baud rate
  Serial.begin(115200);
  Serial.print("INIT ");
  Serial1.setFIFOSize(6400);
  Serial1.setPinout(16, 17);
Serial1.begin(9600);
 Serial.print("INIT ");
}

void loop() {
  // Example usage
  
 getISS();
  delay(1000); // Delay between sequences
}

void stepMotor(int motorPins[], int speed, float currentAngle, float targetAngle, float &currentPos) {
  float angleDifference = targetAngle - currentAngle;
  // Determine the shortest path and account for max rotation
  if (angleDifference > 180) angleDifference -= 360;
  if (angleDifference < -180) angleDifference += 360;
  if (abs(angleDifference) > maxRotation) angleDifference = (angleDifference > 0) ? -maxRotation : maxRotation;

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
  currentPos = fmod(currentPos + angleDifference, 360.0);
  if (currentPos < 0) currentPos += 360.0;
}

void stepRoll(int speed, float targetAngle) {
  int motor1Pins[4] = {motor1Pin1, motor1Pin2, motor1Pin3, motor1Pin4};
  stepMotor(motor1Pins, speed, currentPosRoll, targetAngle, currentPosRoll);
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
 stepRoll(10000, -azimuth); // Move Roll motor to 90 degrees at speed 1000 microseconds
  stepPitch(10000, elevation); // Move Pitch motor to 45 degrees at speed 1000 microseconds
  }else{
    Serial.println("Error Detected");
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

