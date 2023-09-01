#include <lipkg.h>




String receivedData = ""; // Buffer to hold incoming data packets

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Configure Serial2 pins
  Serial2.setTX(8);
  Serial2.setRX(9);

  // Initialize other required hardware
  pinMode(25, OUTPUT);
  digitalWrite(25, HIGH);

  // Initialize Serial2
  Serial2.begin(230400);
}

void loop() {
  if (Serial2.available()) {
    char receivedChar = Serial2.read();

    // Check if the character is a delimiter (newline character in this case)
    if (receivedChar == '\n') {
      // A full packet is received; time to process
      //Serial.print("Received: ");
      for (unsigned int i = 0; i < receivedData.length(); ++i) {
        Serial.print(receivedData[i], HEX);
      }
      Serial.println();
      
      // Clear the receivedData string for the next packet
      receivedData = "";
    } else {
      // Append the received character to the string buffer
      receivedData += receivedChar;
    }
  }
}
