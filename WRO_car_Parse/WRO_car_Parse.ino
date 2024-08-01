#include <Wire.h>

// Buffer to store the received data
const int bufferSize = 31;
byte dataBuffer[bufferSize];

void setup() {
  // Initialize I2C communication as a slave with address 13
  Wire.begin(13);
  
  // Register the receive event function
  Wire.onReceive(receiveEvent);
  
  // Initialize the Serial Monitor
  Serial.begin(9600);
  while (!Serial); // Wait for the Serial Monitor to open
  Serial.println("I2C Slave Receiver Ready");
}

void loop() {
  // Nothing to do in the loop
}

// Function to be called when data is received
void receiveEvent(int bytes) {
  int index = 0;
  
  // Read the received bytes into the buffer
  while (Wire.available() > 0 && index < bufferSize) {
    dataBuffer[index] = Wire.read();
    index++;
  }

  // Print the received data to the Serial Monitor
  Serial.println("Received data:");
 
  for (int i = 1; i < bufferSize; i += 10) {
    if (dataBuffer[i] != 0xFF || dataBuffer[i + 1] != 0xFF) {
      int color = (dataBuffer[i] << 8) | (dataBuffer[i + 1]);
      int16_t x = (dataBuffer[i + 2] << 8) | (dataBuffer[i + 3]);
      int16_t y = (dataBuffer[i + 4] << 8) | (dataBuffer[i + 5]);
      int16_t width = (dataBuffer[i + 6] << 8) | (dataBuffer[i + 7]);
      int16_t height = (dataBuffer[i + 8] << 8) | (dataBuffer[i + 9]);
      
      Serial.print("Color: "); Serial.print(color);
      Serial.print(", X: "); Serial.print(x);
      Serial.print(", Y: "); Serial.print(y);
      Serial.print(", Width: "); Serial.print(width);
      Serial.print(", Height: "); Serial.print(height);
      Serial.println();
    }
  }
}
