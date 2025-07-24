#pragma once

#include "IComponent.h"
#include "../Observer_Pattern/IObserver.h"
#include "HouseState.h"

namespace segments {

  class Alarm : public IComponent, public IObserver<HouseState> {
  public:
    Alarm(int buzzerPin); // Declare constructor
    void MakeSound(int note, float duration);
    void Init() override;
    void Update() override;
    void onNotify(const HouseState& state) override;

  private:
    int relatedBuzzerPin;
  };
}