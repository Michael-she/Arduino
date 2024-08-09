#include <SoftwareSerial.h>

// Define the pins for the HC12 module
#define HC12_TX_PIN 14
#define HC12_RX_PIN 15

// Create a SoftwareSerial object
SoftwareSerial hc12(HC12_RX_PIN, HC12_TX_PIN);

const int green = 2;
const int blue = 3;
const int red = 4;



byte incomingByte;
String readBuffer = "";

void setup() {
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);

  digitalWrite(red, HIGH);
  delay(600);
  digitalWrite(red, LOW);
  delay(100);
    digitalWrite(blue, HIGH);
  delay(600);
  digitalWrite(blue, LOW);
  delay(100);
    digitalWrite(green, HIGH);
  delay(600);
  digitalWrite(green, LOW);
  delay(100);


  // Start the hardware serial communication at 9600 baud rate
  Serial.begin(115200);
  // Start the software serial communication at 9600 baud rate
  hc12.begin(9600);
  // Print a message to indicate setup is complete
  Serial.println("HC12 module initialized. Waiting for data...");

  // Test HC12 communication with AT command
  hc12.print("AT");
  delay(100);
  if (hc12.available()) {
    String response = hc12.readString();
    Serial.print("AT Command Response: ");
    Serial.println(response);
  } else {
    Serial.println("No response from HC12 module. Check wiring and baud rate.");
  }

  pinMode(17, INPUT);
}

void loop() {
 
  // Check if data is available from the HC12 module
   while (hc12.available()) {             // If HC-12 has data
    incomingByte = hc12.read();          // Store each icoming byte from HC-12
    readBuffer += char(incomingByte);    // Add each byte to ReadBuffer string variable
    delay(5);
  }

  if(readBuffer != ""){
    Serial.println(readBuffer);
    readBuffer="";
  }
  // Check if data is available from the Serial Monitor
  if (Serial.available()) {
    // Read the data and store it in a variable
    String sendData = Serial.readString();
    // Send the data to the HC12 module
    hc12.print(sendData);
    // Print a message to indicate data has been sent
    Serial.print("Sent to HC12: ");
    Serial.println(sendData);
  }
    
 
}
