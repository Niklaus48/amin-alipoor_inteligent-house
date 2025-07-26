#pragma once

namespace segments {

  class HouseState{
  public:
    static HouseState Instance;

    //--temp and hum
    float Temperature;
    float Humidity;
    const float minDesireTemp = 10;
    const float maxDesireTemp = 30;

    bool IsMainDoorOpen;
    bool IaDoorPretectorActive;
    bool IsFanActive;
    bool IsLightOn;
    double Time;
  };
}