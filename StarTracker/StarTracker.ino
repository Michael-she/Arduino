#include <HardwareSerial.h>

float stationLongitude = 0;
float stationLatitude = 0;
float altitude = 0;

float ISSLongitude = 0;
float ISSLatitude = 0;
float ISSAltitude = 0;


void setup() {
  
  // Start Serial communication at 9600 baud rate
  Serial.begin(9600);
  Serial1.begin(9600);

  delay(3000);
  Serial.println("INIT ");
  

 Serial1.println("INIT ");
 delay(1000);
  Serial.println("INIT ");
   Serial.println("INIT ");
}

void loop(){

getISS();
delay(100);

}

void getISS() {
  // Send the string to the Serial port
  Serial1.println("||<<||<<GET,https://api.wheretheiss.at/v1/satellites/25544>>||>>||");
Serial.println("Sending serial");
  // Wait for a response for a maximum of 5 seconds
  unsigned long startTime = millis();
  while (!Serial1.available() && millis() - startTime < 20000) {
    // Do nothing until data is available or 5 seconds have passed
  }

  // Check if data is available to read
  if (Serial1.available()) {
    // Read the response and print it
    
    String response = Serial1.readString();
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
    double satelliteAltitude = issAltStr.toDouble();

    double azimuth, elevation;

    calculateAzimuthElevation(observerLatitude, observerLongitude, observerAltitude,
                              satelliteLatitude, satelliteLongitude, satelliteAltitude,
                              azimuth, elevation);

    Serial.print("Azimuth: ");
    Serial.println(azimuth);
    Serial.print("Elevation: ");
    Serial.println(elevation);

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
    double R = 6378137.0; // Earth's radius at sea level in meters

    // Calculate Cartesian coordinates
    X = (R + alt) * cos(latRad) * cos(lonRad);
    Y = (R + alt) * cos(latRad) * sin(lonRad);
    Z = (R + alt) * sin(latRad);
}

// Function to calculate the azimuth and elevation
void calculateAzimuthElevation(double observerLat, double observerLon, double observerAlt,
                               double satelliteLat, double satelliteLon, double satelliteAlt,
                               double &azimuth, double &elevation) {
    double observerX, observerY, observerZ;
    double satelliteX, satelliteY, satelliteZ;

    // Convert observer and satellite coordinates to Cartesian
    gpsToCartesian(observerLat, observerLon, observerAlt, observerX, observerY, observerZ);
    gpsToCartesian(satelliteLat, satelliteLon, satelliteAlt, satelliteX, satelliteY, satelliteZ);

    // Calculate relative position
    double relativeX = satelliteX - observerX;
    double relativeY = satelliteY - observerY;
    double relativeZ = satelliteZ - observerZ;

    // Calculate elevation
    elevation = atan2(relativeZ, sqrt(relativeX * relativeX + relativeY * relativeY)) * 180.0 / M_PI;

    // Calculate azimuth
    azimuth = atan2(relativeY, relativeX) * 180.0 / M_PI;
    if (azimuth < 0) {
        azimuth += 360.0; // Normalize to [0, 360] degrees
    }
}
