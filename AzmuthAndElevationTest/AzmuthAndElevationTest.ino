#include <cmath> // Include the cmath library for mathematical functions

// Function to convert GPS coordinates to Cartesian coordinates
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

void setup() {
    Serial.begin(9600); // Start the serial communication



    delay(5000);
     Serial.print("Inint: ");
     Serial.print("Inint: ");
     Serial.print("Inint: ");
     Serial.print("Inint: ");
    double observerLatitude = -34.056295050573176;
    double observerLongitude = 18.849344674375637;
    double observerAltitude = 256;



    double satelliteLatitude = -33.463262088252;
    double satelliteLongitude = 119.12813713941;
    double satelliteAltitude = 428211;

    double azimuth, elevation;

    calculateAzimuthElevation(observerLatitude, observerLongitude, observerAltitude,
                              satelliteLatitude, satelliteLongitude, satelliteAltitude,
                              azimuth, elevation);

    Serial.print("Azimuth: ");
    Serial.println(azimuth);
    Serial.print("Elevation: ");
    Serial.println(elevation);
}

void loop() {
    // Empty loop as the calculation is done once in setup
}
