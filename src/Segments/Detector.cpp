#include "Detector.h"
#include <Arduino.h>

namespace segments{
    //Constructor
    Detector::Detector(int priPin){
        relatedPirPin = priPin;
    }

    bool Detector::CheckForPrecense() {
        return digitalRead(relatedPirPin);
    }

    void Detector::Init(){
        pinMode(relatedPirPin, INPUT);
        Serial.print("Detector(Pir) Initialized on pin ");
        Serial.print(relatedPirPin);
        Serial.println();
    }

    void Detector::Update(){
        bool precense = digitalRead(relatedPirPin);
        if (precense != HouseState::Instance.IsAnyPrecense){
            HouseState::Instance.IsAnyPrecense = precense;
            NotifyObservers(HouseState::Instance);
        }
    }
}