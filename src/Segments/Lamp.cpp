#include <Arduino.h>
#include "Lamp.h"

namespace segments{
        
    //Constructor
    Lamp::Lamp(int ledPin){
        relatedLedPin = ledPin;
    }

    void Lamp::SetActive(bool active) {
        digitalWrite(relatedLedPin, active? HIGH: LOW);
    }

    void Lamp::Init(){
        pinMode(relatedLedPin, OUTPUT);
        Serial.print("Lamp(LED) Initialized on pin ");
        Serial.print(relatedLedPin);
        Serial.println();
    }

    void Lamp::Update(){
    }

    void Lamp::onNotify(const HouseState& state){

        if (state.IsLightOn){
            SetActive(true);
        }
        else
        {
            if (state.IsAutoLightActive){
                if (state.IsAnyPrecense){
                    SetActive(true);
                }
                else if (state.Time.minute >= 16){
                    SetActive(true);
                }
                else{
                    SetActive(false);
                }
            }
            else{
                SetActive(false);
            }
        }
        Serial.println("Recieve state from subjects in {Lamp}");
    }
}