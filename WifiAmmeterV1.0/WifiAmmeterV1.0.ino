// Include the ESP32 library
#include <Arduino.h>

// Define the GPIO pin number for the analog input
const int analogPin = 36; // GPIO15 (D15)

void setup() {
  // Start the serial communication
  Serial.begin(115200);

  // Configure the analog pin as input
  pinMode(analogPin, INPUT);
}

void loop() {
  // Read the analog value
  int analogValue = analogRead(analogPin);

  // Convert the analog value to voltage
  float voltage = analogValue * (3.3 / 4095.0);

  // Print the value and voltage to the Serial Monitor
  Serial.print("Analog Value: ");
  Serial.print(analogValue);
  Serial.print(" | Voltage: ");
  Serial.println(voltage);

  // Wait for a bit before reading again
  delay(1000);
}
