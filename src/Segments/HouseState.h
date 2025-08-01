#pragma once

#include <ArduinoJson.h>

namespace segments {

  struct DateTime
  {
    long second = 0;
    long minute = 0;
    long hour = 0;

    static DateTime ToDateTimeSecond(unsigned long seconds){
        DateTime dt = DateTime();
        dt.second = seconds % 60;
        seconds /= 60;
        dt.minute = seconds % 60;
        seconds /= 60;
        dt.hour = seconds % 24;

        return dt;
    }
    static unsigned long DateTimeToSecond(DateTime dt){
        return (dt.hour * 3600) + (dt.minute * 60) + dt.second;
    }
  };


  class HouseState{
  public:
    static HouseState Instance;// singletone instance

    //--temp and hum
    float Temperature;
    float Humidity;
    const float minDesireTemp = 10;
    const float maxDesireTemp = 30;
    //--Door
    bool IsMainDoorOpen;
    bool IsDoorPretectorActive;
    //--Cooling system
    bool IsFanActive;
    bool IsAutoFanActive;
    //--Lamps
    bool IsLightOn;
    bool IsAutoLightActive;
    //Clock
    DateTime Time;
    //Detector
    bool IsAnyPrecense;


    String ToJson() const{
    StaticJsonDocument<200> doc;

    doc["temp"] = this->Temperature;
    doc["hum"] = this->Humidity;
    doc["tempThreshold"] = this->maxDesireTemp;
    doc["isMainDoorOpen"] = this->IsMainDoorOpen;
    doc["IsDoorPretectorActive"] = this->IsDoorPretectorActive;
    doc["IsFanActive"] = this->IsFanActive;
    doc["IsAutoFanActive"] = this->IsAutoFanActive;
    doc["IsLightOn"] = this->IsLightOn;
    doc["IsAutoLightActive"] = this->IsAutoLightActive;
    doc["IsAnyPrecense"] = this->IsAnyPrecense;
    doc["hour"] = this->Time.hour;
    doc["min"] = this->Time.minute;
    doc["sec"] = this->Time.second;
  
    String output;
    serializeJson(doc, output);

    return output;
    }
  };
}