// #include <Arduino.h>
// #include <WiFi.h>
// #include <WiFiClient.h>
// #include <WebServer.h>
// #include <uri/UriBraces.h>
#include <ESP32Servo.h>

#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""
// Defining the WiFi channel speeds up the connection:
#define WIFI_CHANNEL 6

//WebServer server(80);
Servo myServo;

void sendHtml() {
  //server.send(200, "text/html", response);
}

void setup(void) {
  // Serial.begin(9600);
  myServo.attach(18);
  myServo.write(180);

  
  // WiFi.begin(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL);
  // Serial.print("Connecting to WiFi with SSID: ");
  // Serial.print(WIFI_SSID);
  // // Wait for connection
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(100);
  //   Serial.print(".");
  // }
  // Serial.println("Successfully Connected to server!");

  // Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());

  // server.on("/", sendHtml);

  // server.begin();
  // Serial.println("HTTP server started");
  // Serial.println("Please open this url to access server => http://localhost:8180");
  // myServo.write(180);
}

void loop() {
  
}
