#pragma once

#include "IComponent.h"
#include "../Observer_Pattern/ISubject.h"
#include "HouseState.h"

namespace segments {
  class Detector : public IComponent, public ISubject<HouseState> {
  public:
    Detector(int pirPin); // Declare constructor
    bool CheckForPrecense();
    void Init() override;
    void Update() override;

  private:
    int relatedPirPin;
  };
}