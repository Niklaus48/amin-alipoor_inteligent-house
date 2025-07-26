#include <ESP32Servo.h>
#include "Door.h"

namespace segments{
        
    //Constructor
    Door::Door(int pin, int open, int close){
        openDegree = open;
        closeDegree = close;
        relatedServo = Servo();
        relatedServoPin = pin;
    }

    void Door::SetOpen(bool open) {
        if (open) {
            relatedServo.write(openDegree);
        }
        else {
            relatedServo.write(closeDegree);
        }
    }

    void Door::Init(){
        relatedServo.attach(relatedServoPin);
        Serial.print("Door(Servo) Initialized on pin ");
        Serial.print(relatedServoPin);
        Serial.println();
    }

    void Door::Update(){
        
    }

    void Door::onNotify(const HouseState& state){
        
    }
}