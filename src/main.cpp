#include <Arduino.h>
#include <TM1637Display.h>
#include <Adafruit_SSD1306.h>
#include <Segments/Alarm.h>
#include <Segments/Door.h>
#include <Segments/ServerLogic.h>
#include <Segments/MusicPLayer.h>
#include <Segments/Thermometer.h>
#include <Segments/Fan.h>
#include <Observer_Pattern/ISubject.h>

using namespace segments;

#define PirPin 2
bool isPirActive = false;

//---------------------class----------------

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
Fan fan(25);
Alarm alarmSystem(26);
Thermometer thermo(23, DHT22);
MusicPlayer musicPLayer(26);
ServerLogic serverLogic("Wokwi-GUEST", "", 8);
TM1637Display sevSeg = TM1637Display(27, 34);

IComponent* components[] = {&leftDoor, &rightDoor, &alarmSystem, &musicPLayer, &thermo, &fan, &serverLogic};
HouseState HouseState::Instance;// create a singletone instance of HouseState class

void setup() {
  Serial.begin(9600);
  for (auto c : components) c->Init();
  thermo.AddObserver(&alarmSystem);
  thermo.AddObserver(&fan);
}

void loop() {
  for (auto c : components) c->Update();
}
