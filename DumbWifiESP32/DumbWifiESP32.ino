#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>

const int red = 12;
const int green = 14;
const int blue = 27;

const char* ssid = "3VG WiFi";
const char* password = "3vangogh@*";

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;     // whether the string is complete
String finalInputString = "";
String serverName ="";


  bool firstDelimiterFound = false;
  bool endDelimiterFound = false;


void setup() {
  // put your setup code here, to run once:
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

Serial.begin(9600);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(red, HIGH);
    delay(500);
    digitalWrite(red, LOW);
    Serial.println("CONN WAIT");
  }
  Serial.println("CONN SUCSESS");
 
for(int i = 0; i<10; i++){
  delay(50);
    digitalWrite(green, HIGH);
    delay(50);
    digitalWrite(green, LOW);
}
   
}

void loop() {
  // print the string when a newline arrives:
digitalWrite(blue, LOW);
 digitalWrite(red, LOW);
 
  while (Serial.available()) {
    //  Serial.println("Serial");
     digitalWrite(red, LOW);
    // get the new byte:
    char inChar = (char)Serial.read(); 
   
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is the closing delimiter, set stringComplete to true:
    digitalWrite(blue, HIGH);

    if(firstDelimiterFound == true && endDelimiterFound == false){
      finalInputString += inChar;
    }


    if(finalInputString.endsWith(">>||>>||")){
     
      finalInputString = finalInputString.substring(0, finalInputString.length()-8);
      endDelimiterFound = true;
      stringComplete = true;

    }


    if(inputString.indexOf("||<<||<<") != -1){

      
      firstDelimiterFound = true;

    }

    
  


  }


  if (stringComplete) {
    digitalWrite(red, LOW);
    digitalWrite(blue, HIGH);
    


    String method = finalInputString.substring(0, finalInputString.indexOf(","));
    finalInputString = finalInputString.substring(finalInputString.indexOf(",")+1);
  //Serial.println(method);
 

  if(method == "GET"){

    serverName = finalInputString;
  getRequest();


  }

   if(method == "POST"){

    serverName = finalInputString.substring(0, finalInputString.indexOf(","));
    finalInputString = finalInputString.substring(finalInputString.indexOf(",")+1);
  sendPackets(finalInputString);


  }

    // clear the string:
    inputString = "";
finalInputString="";
  firstDelimiterFound = false;
    stringComplete = false;
    endDelimiterFound = false;
  }
}






void getRequest(){
digitalWrite(blue, HIGH);

 if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    // Specify the URL
    http.begin(serverName);
    // Make the request
    int httpCode = http.GET();

    // Check is request was successful
    if(httpCode > 0){
      if(httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          Serial.println(payload);
          digitalWrite(blue, LOW);
          digitalWrite(green, HIGH);
          delay(500);
          digitalWrite(green, LOW);
      }
    }
    else {
      digitalWrite(blue, LOW);
      Serial.println("ERROR");
      Serial.println(httpCode);
      digitalWrite(red, HIGH);
          delay(500);
          digitalWrite(red, LOW);
    }
    http.end(); //Free resources
  }
  else {
    Serial.println("WiFi not connected");
    digitalWrite(red, HIGH);
          delay(5000);
          digitalWrite(red, LOW);
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
      Serial.print("ERROR: ");
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