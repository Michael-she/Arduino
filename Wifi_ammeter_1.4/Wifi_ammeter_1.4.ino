#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>
const int analogPins [] = {36, 39};
const int numCurrentSensors = 2;

const int red = 12;
const int green = 14;
const int blue = 27;
const int resistor = 122;
const int transformerMultiplier = 2000;

const char* ssid = "3VG WiFi";
const char* password = "3vangogh@*";

int sendInterval = 10;

const char* serverName = "https://automationhub.vercel.app/api/CurrentSensor"; // Replace with your server's URL

String id = ""+WiFi.macAddress();



void setup() {

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(red, HIGH);
    delay(500);
    digitalWrite(red, LOW);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(id);
for(int i = 0; i<10; i++){
  delay(50);
    digitalWrite(green, HIGH);
    delay(50);
    digitalWrite(green, LOW);
}
for(int i = 0; i<numCurrentSensors; i++){
   pinMode(analogPins[i], INPUT);
}
}

void loop() {
 
   
    

  float maxAmps[numCurrentSensors] ;
  float minAmps[numCurrentSensors] ;
  double ampsSum[numCurrentSensors];

  digitalWrite(green, HIGH);
    for(int i = 0; i<sendInterval*10; i++){
      for(int j = 0; j<numCurrentSensors; j++){
      float currentAmps  = getAmps(analogPins[j] );
      if(currentAmps >maxAmps[j] ){
        maxAmps[j] = currentAmps;
      }

      if(currentAmps <minAmps[j]  || minAmps[j] ==0){
        minAmps[j]  = currentAmps;
      }
    ampsSum[j]  +=currentAmps ;
      }
      delay(100);


    }
digitalWrite(green, LOW);
for(int j = 0; j<numCurrentSensors; j++){
  String toSend = id + ":"+j+",";
    float aveAmps = ampsSum[j]/(sendInterval*10);

    float power = aveAmps*220*sendInterval/3600;

    toSend+=toAmpString(aveAmps)+","+toAmpString(maxAmps[j])+","+toAmpString(minAmps[j])+","+toAmpString(power);
 Serial.println(toSend);
    sendPackets(toSend);
}
   

 
}

void sendPackets(String sendData){

   if(WiFi.status() == WL_CONNECTED) {
     digitalWrite(blue, HIGH);
    HTTPClient https;
    
    https.begin(serverName); // HTTPS begin
    https.addHeader("Content-Type", "text/plain");

   
    int httpResponseCode = https.POST(sendData); // Your POST data here
digitalWrite(blue, LOW);
    if(httpResponseCode > 0) {
      String response = https.getString(); //Get the response to the request
      Serial.println(httpResponseCode);   //Print return code
      Serial.println(response);           //Print request answer

      for(int i = 0; i<10; i++){
  delay(50);
    digitalWrite(green, HIGH);
    digitalWrite(blue, LOW);
    delay(50);
      digitalWrite(blue, HIGH);
    digitalWrite(green, LOW);
}
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
      for(int i = 0; i<10; i++){
  delay(50);
    digitalWrite(red, HIGH);
    digitalWrite(blue, LOW);
    delay(50);
      digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
}
    }

    https.end(); //End HTTP
  } else {

    digitalWrite(red, HIGH);
    delay(1000);
    digitalWrite(red, LOW);
    Serial.println("Error in WiFi connection");
  }

}



float getAmps(int inPin){

int analogValue = analogRead(inPin);

  // Convert the analog value to voltage
  float voltage = analogValue * (3.1 / 4095.0);

  // Print the value and voltage to the Serial Monitor
  //Serial.print("Analog Value: ");
 // Serial.print(analogValue);
 // Serial.print(" | Voltage: ");
 if(voltage!=0){
   voltage+=0.13;
 }
  Serial.print(voltage);
  Serial.print("  ");
  float amps = (voltage/resistor) *transformerMultiplier*2;
 Serial.println(amps);

  // Wait for a bit before reading again


return amps;

}

String toAmpString(float amps){


  String ampString = String(amps, 2);

  return ampString;
}
