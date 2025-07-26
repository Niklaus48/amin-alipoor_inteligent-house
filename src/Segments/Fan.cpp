#include <ESP32Servo.h>
#include "Fan.h"

namespace segments{
        
    //Constructor
    Fan::Fan(int pin){
        relatedServo = Servo();
        relatedServoPin = pin;
    }

    void Fan::Spin() {

    }

    void Fan::Init(){
        relatedServo.attach(relatedServoPin);
        Serial.print("Fan(Servo) Initialized on pin ");
        Serial.print(relatedServoPin);
        Serial.println();
    }

    void Fan::Update(){
        static unsigned long turnDelay = 0;
        static unsigned int turnCount = 0;
        if (millis() - turnDelay >= 300){

            if (isActive){
                if (turnCount % 2 == 1) relatedServo.write(0);
                else relatedServo.write(180);

                turnDelay = millis();
                turnCount++;
            }
            else{
                relatedServo.write(90);
            }
        }
    }

    void Fan::onNotify(const HouseState& state){
        isActive = state.Temperature > state.maxDesireTemp;
    }
}