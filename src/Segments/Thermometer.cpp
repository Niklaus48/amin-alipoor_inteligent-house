#include <DHT.h>
#include "Thermometer.h"

namespace segments{
    
    //Constructor
    Thermometer::Thermometer(int relatedPin, uint8_t type) : dht(relatedPin, type){            
    }

    String Thermometer::GetTemperatureAsString() {
        return "Temperature: " + String(GetTemperature()) + "C";
    }

    String Thermometer::GetHumidityAsString() {
        return "Humidity: " + String(GetHumidity()) + "%";
    }

    float Thermometer::GetHumidity(){
        return dht.readHumidity();
    }

    float Thermometer::GetTemperature(){
        return dht.readTemperature();
    }

    void Thermometer::Init(){
        dht.begin();
        Serial.println("Thermometer(DHT) Initialized Successfully");
    }

    void Thermometer::Update(){
        if (millis() - updateDelay >= 1000){
            HouseState::Instance.Temperature = dht.readTemperature();
            HouseState::Instance.Humidity = dht.readHumidity();

            NotifyObservers(HouseState::Instance);
            Serial.println("Humidity and Temperature Data has been brodcasted to observers");
            updateDelay = millis();
        }
    }
}