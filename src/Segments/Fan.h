#pragma once

#include <ESP32Servo.h>
#include "IComponent.h"
#include "../Observer_Pattern/IObserver.h"
#include "HouseState.h"

namespace segments {
  class Fan : public IComponent,public IObserver<HouseState> {
  public:
    Fan(int pin); // Declare constructor
    void Spin();
    void Init() override;
    void Update() override;
    void onNotify(const HouseState& state) override;

  private:
    int relatedServoPin;
    bool isActive;
    Servo relatedServo;

  };
}
