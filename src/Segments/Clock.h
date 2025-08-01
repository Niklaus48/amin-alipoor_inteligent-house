#pragma once
#include "IComponent.h"
#include "HouseState.h"
#include <TM1637Display.h>
#include "../Observer_Pattern/ISubject.h"


namespace segments {
  class Clock : public IComponent, public ISubject<HouseState> {
  public:
    Clock(int clkPin, int dioPin, float startTimeAsHour); // Declare constructor
    DateTime GetCurrentTime();
    void Init() override;
    void Update() override;

  private:
    TM1637Display sevSeg;
    unsigned long startTime;// as second
  };
}
