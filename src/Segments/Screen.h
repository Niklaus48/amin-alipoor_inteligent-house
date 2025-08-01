#pragma once

#include "IComponent.h"
#include "../Observer_Pattern/IObserver.h"
#include "HouseState.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

namespace segments {
  class Screen : public IComponent, public IObserver<HouseState> {
  public:
    Screen(uint8_t width, uint8_t height); // Declare constructor
    void Init() override;
    void Update() override;
    void onNotify(const HouseState& state) override;

  private:
    Adafruit_SSD1306 oled;
  };
}