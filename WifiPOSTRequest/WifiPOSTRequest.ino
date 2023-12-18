/*
    This sketch establishes a TCP connection to a "quote of the day" service.
    It sends a "hello" message, and then prints received data.
*/

#include <WiFi.h>

#include <HTTPClient.h>

#ifndef STASSID
#define STASSID "3VG WiFi"
#define STAPSK "3vangogh@*"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

const char* host = "djxmmx.net";
const uint16_t port = 17;

WiFiMulti multi;

void setup() {
  Serial.begin(115200);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  multi.addAP(ssid, password);

  if (multi.run() != WL_CONNECTED) {
    Serial.println("Unable to connect to network, rebooting in 10 seconds...");
    delay(10000);
    rp2040.reboot();
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
 if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Specify the URL for the POST request
    http.begin("https://current-sensor-webapp-michael-she.vercel.app/inputCSV"); // Replace with your server URL

    // Specify the content type of the request
    http.addHeader("Content-Type", "text/plain");

    // Send the POST request
    int httpResponseCode = http.POST("Arduino,6.42"); // Replace with your POST data

    // If the response is successful, print the response
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    // End the HTTP connection
    http.end();
  } else {
    Serial.println("WiFi not connected");
  }

  delay(10000); // Send a POST request every 10 seconds
}
