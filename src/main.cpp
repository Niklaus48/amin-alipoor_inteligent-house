#include <Arduino.h>
#include <Segments/Alarm.h>
#include <Segments/Door.h>
#include <Segments/ServerLogic.h>
#include <Segments/MusicPLayer.h>
#include <Segments/Thermometer.h>
#include <Segments/Fan.h>
#include <Segments/Lamp.h>
#include <Segments/Detector.h>
#include <Segments/Clock.h>
#include <Segments/Screen.h>
#include <Observer_Pattern/ISubject.h>

using namespace segments;


Door leftDoor(33, 90, 0);
Door rightDoor(18, 90, 0);
Fan fan(25);
Lamp lamp(4);
Detector detector(5);
Alarm alarmSystem(26);
Thermometer thermo(23, DHT22);
ServerLogic serverLogic("Wokwi-GUEST", "", 8);
segments::Clock myClock((uint8_t)27, (uint8_t)14, 0.25);
Screen screen(128, 64);


IComponent* components[] = {&leftDoor, &rightDoor, &alarmSystem, &thermo, &fan, &lamp, &detector, &myClock, &screen, &serverLogic};
HouseState HouseState::Instance;// create a singletone instance of HouseState class

void SensorTask(void* param) {
  while (true) {
    thermo.Update();
    detector.Update();
    vTaskDelay(50 / portTICK_PERIOD_MS);  // 20Hz
  }
}

void ViewTask(void* param) {
  while (true) {
    lamp.Update();
    fan.Update();
    alarmSystem.Update();
    leftDoor.Update();
    rightDoor.Update();
    screen.Update();
    vTaskDelay(50 / portTICK_PERIOD_MS); // 20Hz
  }
}

void ServerTask(void* param) {
  while (true) {
    serverLogic.Update();
    myClock.Update();
    vTaskDelay(50 / portTICK_PERIOD_MS); // 20Hz
  }
}

void setup() {
  Serial.begin(9600);
  
  for (auto c : components) c->Init();
  
  thermo.AddObserver(&alarmSystem);
  thermo.AddObserver(&fan);
  thermo.AddObserver(&screen);

  detector.AddObserver(&lamp);
  detector.AddObserver(&screen);

  myClock.AddObserver(&lamp);
  myClock.AddObserver(&screen);

  serverLogic.AddObserver(&fan);
  serverLogic.AddObserver(&leftDoor);
  serverLogic.AddObserver(&rightDoor);
  serverLogic.AddObserver(&lamp);
  serverLogic.AddObserver(&screen);

  xTaskCreatePinnedToCore(SensorTask, "SensorTask", 2048, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(ViewTask, "ViewTask", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(ServerTask, "ServerTask", 2048, NULL, 1, NULL, 1);
}

void loop() {
  //for (auto c : components) c->Update();
}
