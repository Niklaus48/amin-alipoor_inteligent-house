#include <ESP32Servo.h>
#include "Door.h"

namespace segments{
        
    //Constructor
    Door::Door(int pin, int open, int close):openDegree(open), closeDegree(close) {

        openDegree = open;
        closeDegree = close;
        relatedServo = Servo();
        relatedServo.attach(pin);
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

    }

    void Door::Update(){
        
    }

    void Door::onNotify(const HouseState& state){
        
    }
}