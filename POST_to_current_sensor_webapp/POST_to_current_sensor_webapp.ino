#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>
const int analogPin = 36;

const int resistor = 44;
const int transformerMultiplier = 1000;

const char* ssid = "3VG WiFi";
const char* password = "3vangogh@*";

int sendInterval = 10;

const char* serverName = "https://current-sensor-webapp-michael-she.vercel.app/inputCSV"; // Replace with your server's URL

String id = "Wifi1";



void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

   pinMode(analogPin, INPUT);
}

void loop() {
 
   String toSend = id + ",";
    

  float maxAmps = 0;
  float minAmps = 999999;
  double ampsSum = 0;
    for(int i = 0; i<sendInterval*10; i++){

      float currentAmps = getAmps(analogPin);
      if(currentAmps>maxAmps){
        maxAmps = currentAmps;
      }

      if(currentAmps<minAmps){
        minAmps = currentAmps;
      }
    ampsSum +=currentAmps;
      delay(100);


    }

    float aveAmps = ampsSum/(sendInterval);

    float power = aveAmps*220*sendInterval;

    toSend+=toAmpString(aveAmps)+","+toAmpString(maxAmps)+","+toAmpString(minAmps)+","+toAmpString(power);

    sendPackets(toSend);

 
}

void sendPackets(String sendData){

   if(WiFi.status() == WL_CONNECTED) {
    HTTPClient https;
    
    https.begin(serverName); // HTTPS begin
    https.addHeader("Content-Type", "text/plain");

   
    int httpResponseCode = https.POST(sendData); // Your POST data here

    if(httpResponseCode > 0) {
      String response = https.getString(); //Get the response to the request
      Serial.println(httpResponseCode);   //Print return code
      Serial.println(response);           //Print request answer
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    https.end(); //End HTTP
  } else {
    Serial.println("Error in WiFi connection");
  }

}
String generateRandomNumberString() {
  // Generate a random integer between 0 and 2000
  int randInt = random(2001); // 2001 because we need to include 2000 in our range

  // Convert to float and scale down to get two decimal places
  float randFloat = randInt / 100.0;

  // Convert float to String with two decimal places
  String randomNumberString = String(randFloat, 2);

  return randomNumberString;
}


float getAmps(int inPin){

int analogValue = analogRead(inPin);

  // Convert the analog value to voltage
  float voltage = analogValue * (4 / 4095.0);

  // Print the value and voltage to the Serial Monitor
  //Serial.print("Analog Value: ");
 // Serial.print(analogValue);
 // Serial.print(" | Voltage: ");
  Serial.println(voltage);

  float amps = (voltage/resistor) *transformerMultiplier/2;


  // Wait for a bit before reading again


return amps;

}

String toAmpString(float amps){


  String ampString = String(amps, 2);

  return ampString;
}
