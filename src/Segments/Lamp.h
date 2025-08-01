#pragma once

#include "IComponent.h"
#include "../Observer_Pattern/IObserver.h"
#include "HouseState.h"

namespace segments {
  class Lamp : public IComponent, public IObserver<HouseState> {
  public:
    Lamp(int ledPin); // Declare constructor
    void SetActive(bool active);
    void Init() override;
    void Update() override;
    void onNotify(const HouseState& state) override;

  private:
    int relatedLedPin;
  };
}