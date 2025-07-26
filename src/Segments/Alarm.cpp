#include "Alarm.h"
#include <Arduino.h>

namespace segments{

    //Constructor
    Alarm::Alarm(int buzzerPin){
        relatedBuzzerPin = buzzerPin;   
    } 

    void Alarm::MakeSound(int note, float duration) {
        tone(relatedBuzzerPin, note, duration * 1000);
    }

    void Alarm::Init(){
        pinMode(relatedBuzzerPin, OUTPUT);
        Serial.print("Alarm(Buzzer) Initialized on pin ");
        Serial.print(relatedBuzzerPin);
        Serial.println();
    }

    void Alarm::Update(){
        
    }

    void Alarm::onNotify(const HouseState& state){
        if (state.Temperature >= 50){
            Serial.println("High temperature detected!!!");
            MakeSound(440, 0.5);
        }
    }
}

