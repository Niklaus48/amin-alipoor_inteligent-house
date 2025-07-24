#include <DHT.h>
#include "Thermometer.h"

namespace segments{

        //Constructor
        Thermometer::Thermometer(int relatedPin, uint8_t type) : dht(relatedPin, type){
            dht.begin();
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

        }

        void Thermometer::Update(){
            
        }
}