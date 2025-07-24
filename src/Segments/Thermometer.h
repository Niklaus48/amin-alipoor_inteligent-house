#pragma once
#include <DHT.h>
#include "IComponent.h"
#include "HouseState.h"
#include "../Observer_Pattern/ISubject.h"


namespace segments {
  class Thermometer : public IComponent, public ISubject<HouseState> {
  public:
    Thermometer(int relatedPin, uint8_t type); // Declare constructor
    String GetTemperatureAsString();
    String GetHumidityAsString();
    float GetHumidity();
    float GetTemperature();
    void Init() override;
    void Update() override;

  private:
    DHT dht;
  };
}
