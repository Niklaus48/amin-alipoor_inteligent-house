#include "Alarm.h"
#include <Arduino.h>

namespace segments{

    //Constructor
    Alarm::Alarm(int buzzerPin){
        relatedBuzzerPin = buzzerPin;   
    } 

    void Alarm::MakeSound(int freq, unsigned long duration) {
        if (freq == 0){
            noTone(relatedBuzzerPin);
            return;
        }

        tone(relatedBuzzerPin, freq , duration);
    }

    void Alarm::Init(){
        pinMode(relatedBuzzerPin, OUTPUT);
        setToneChannel(2);
        Serial.print("Alarm(Buzzer) Initialized on pin ");
        Serial.print(relatedBuzzerPin);
        Serial.println();
    }

    void Alarm::Update(){
        
        if (millis() - updateDelay >= 1000){
            if (isAlarmActive){            
                MakeSound(440, 250);
                updateDelay = millis();
            }       
        }
    }

    void Alarm::onNotify(const HouseState& state){
        isAlarmActive = state.Temperature >= 70;
        Serial.println("Recieve state from subjects in {Alarm}");
    }
}

