#include <SoftwareSerial.h>

// Define the pins for the HC12 module
#define HC12_TX_PIN 14
#define HC12_RX_PIN 15

SoftwareSerial hc12(HC12_RX_PIN, HC12_TX_PIN);



byte incomingByte;
String readBuffer = "";

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
          

void setup() {
  // put your setup code here, to run once:
 myservo.attach(2, 500, 2500);  // attaches the servo on pin 9 to the servo object

  myservo.write(5);
    hc12.begin(1200);
  pinMode(21, OUTPUT);

  for(int i = 0; i<10; i++){

    digitalWrite(21,HIGH);
    delay(50);
    digitalWrite(21,LOW);
    delay(50);


  }
   pinMode(12, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (digitalRead(12) == HIGH){
    myservo.write(90);
    delay(5000);
    myservo.write(5);


  }
  // ==== Storing the incoming data into a String variable
  while (hc12.available()) {             // If HC-12 has data
    incomingByte = hc12.read();          // Store each icoming byte from HC-12
    readBuffer += char(incomingByte);    // Add each byte to ReadBuffer string variable
   // delay(100);
  }
//  delay(500);
  // ==== Sending data from one HC-12 to another via the Serial Monitor
  while (Serial.available()) {
    hc12.write(Serial.read());
  }

  if(readBuffer != ""){
  String response = readBuffer;
  readBuffer = "";

    Serial.print("AT Command Response: ");
    Serial.println(response);
    if(response=="H"){
          digitalWrite(21,HIGH);
    
    
        hc12.print("b");
       
        digitalWrite(21,LOW);

    }else if (response == "f"){
      myservo.write(180);
      digitalWrite(21,HIGH);
      delay(5000);
      myservo.write(90);
    
  } 
  }
}
