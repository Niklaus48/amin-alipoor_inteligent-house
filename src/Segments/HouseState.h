#pragma once

namespace segments {

  class HouseState{
  public:
    float Temperature;
    float Humidity;
    bool IsMainDoorOpen;
    bool IaDoorPretectorActive;
    bool IsFanActive;
    bool IsLightOn;
    double Time;
  };
}