#include "Alarm.h"
#include <Arduino.h>

namespace segments{

    //Constructor
    Alarm::Alarm(int buzzerPin){
        this->relatedBuzzerPin = buzzerPin;
        pinMode(buzzerPin, OUTPUT);
    } 

    void Alarm::MakeSound(int note, float duration) {
        tone(relatedBuzzerPin, note, duration * 1000);
    }

    void Alarm::Init(){

    }

    void Alarm::Update(){
        
    }

    void Alarm::onNotify(const HouseState& state){
        
    }
}

