#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <uri/UriBraces.h>
#include <TM1637Display.h>
#include <Adafruit_SSD1306.h>
#include <Segments/Alarm.h>
#include <Segments/Door.h>
#include <Segments/MusicPLayer.h>
#include <Segments/Thermometer.h>
#include <Observer_Pattern/ISubject.h>

using namespace segments;

#define PirPin 2
bool isPirActive = false;

//---------------------class----------------
class ServerLogic{

  public:
    WebServer server;

    //Constructor
    ServerLogic(const char* ssid, const char *pass, int32_t channel): server(80) {

      WiFi.begin(ssid, pass, channel);
      while (WiFi.status() != WL_CONNECTED) {
        delay(100);
      }

      //config routes here

      server.begin();
    }

    void sendHtml() {
      //server.send(200, "text/html", response);
    }
};
struct Pin{
  int i;
  int j;
};


//--------------------object-------------------
Pin logo[] = {
  {0,10},
  {0,11},
  {0,12},
  {0,13},
  {0,14},
  {7,15},
  {7,16},
  {7,17},
  {7,18},
  {1,14},
  {2,14},
  {3,14},
  {4,14},
  {5,14},
  {6,14},
  {7,14},
  {4,15},
  {4,16},
  {4,17},
  {4,18},
  {3,18},
  {2,18},
  {1,18},
  {0,18},
  {4,14},
  {4,13},
  {4,12},
  {4,11},
  {4,10},
  {5,10},
  {6,10},
  {7,10},
};
Door leftDoor(33, 180, 90);
Door rightDoor(18, -90, 90);
Alarm alarmSystem(35);
Thermometer thermo(23, DHT22);
MusicPlayer musicPLayer(1);
ServerLogic serverLogic("X^2 + 1 = 0", "javabe haghighi nadarad", 8);
TM1637Display sevSeg = TM1637Display(31, 33);

IComponent* components[] = { &leftDoor, &rightDoor, &alarmSystem, &musicPLayer, &thermo};


void setup() {
  Serial.begin(9600);

  ISubject<HouseState>& subject = thermo; // reference to base interface
 
  // WiFi.begin(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL);
  // Serial.println("trying to connect to wifi");
  // //Wait for connection
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(100);
  //   Serial.print(".");
  // }
  // Serial.println("Successfully Connected to server!");

  // //server.on("/", sendHtml);

  // server.begin();
  // Serial.println("HTTP server started");
  // Serial.println("Please open this url to access server => http://localhost:8180");
  // myServo.write(180);
  for (auto c : components) c->Init();
}

void loop() {
  serverLogic.server.handleClient();
  for (auto c : components) c->Update();
}
