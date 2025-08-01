#include <ESP32Servo.h>
#include "Fan.h"

namespace segments{

    //Constructor
    Fan::Fan(int pin){
        relatedServo = Servo();
        relatedServoPin = pin;
    }

    void Fan::Spin() {
        relatedServo.write(turnCount % 2 == 1 ? 0 : 180);
        turnCount++;
    }

    void Fan::Init(){
        relatedServo.attach(relatedServoPin);
        Serial.print("Fan(Servo) Initialized on pin ");
        Serial.print(relatedServoPin);
        Serial.println();
    }

    void Fan::Update(){
        if (millis() - updateDelay >= 800){
            
            if (isActive){
                Spin();
                updateDelay = millis();
            }
            else{
                relatedServo.write(90);
            }
        }
    }

    void Fan::onNotify(const HouseState& state){

        isActive = state.IsFanActive;

        if (state.IsAutoFanActive){
            isActive = state.Temperature > state.maxDesireTemp;
        }

        Serial.println("Recieve state from subjects in {Fan}");
    }
}