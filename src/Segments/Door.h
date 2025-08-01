#pragma once

#include <ESP32Servo.h>
#include "IComponent.h"
#include "../Observer_Pattern/IObserver.h"
#include "HouseState.h"

namespace segments {
  class Door : public IComponent,public IObserver<HouseState> {
  public:
    Door(int pin, int open, int close); // Declare constructor
    void SetOpen(bool open);
    void Init() override;
    void Update() override;
    void onNotify(const HouseState& state) override;

  private:
    int openDegree;
    int closeDegree;
    int relatedServoPin;
    bool isDoorOpen;
    Servo relatedServo;

  };
}
