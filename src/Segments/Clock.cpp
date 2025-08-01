#include <TM1637Display.h>
#include <Arduino.h>
#include "Clock.h"

namespace segments{

    //Constructor
    Clock::Clock(int clkPin, int dioPin, float startTimeAsHour) : sevSeg(clkPin, dioPin){
        startTime = startTimeAsHour * 3600;
    }

    DateTime Clock::GetCurrentTime(){
        unsigned long currentSecond = (millis() / 1000) + startTime;
        return DateTime::ToDateTimeSecond(currentSecond);
    }

    void Clock::Init(){
        sevSeg.setBrightness(15);
        Serial.println("Clock(Seven Segment) Initialized Successfully");
        Serial.println();
    }

    void Clock::Update(){
        if (millis() - updateDelay >= 1000){

            DateTime currentTime = GetCurrentTime();
            HouseState::Instance.Time = currentTime;
            NotifyObservers(HouseState::Instance);

            int displayTimeValue = ((currentTime.minute) * 100) + currentTime.second;
            sevSeg.showNumberDecEx(displayTimeValue, 0b11100000, true);

            updateDelay = millis();
        }
    }
}