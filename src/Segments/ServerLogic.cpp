#include "ServerLogic.h"
#include <Arduino.h>

namespace segments{

    //Constructor
    ServerLogic::ServerLogic(const char* ssid, const char *pass, int32_t channel){
        SSID = ssid;
        Password = pass;
        this->channel = channel;
    } 

    void ServerLogic::sendHtml(const char* Html) {
        server.send(200,"",Html);
    }

    void ServerLogic::Init(){
        WiFi.begin(SSID, Password, channel);
        Serial.println("Attempt to connect to the wifi with SSId: ");
        Serial.print(SSID);
        Serial.print(" and Password: ");
        Serial.print(Password);
        Serial.println();

        Serial.println("Connecting");
        while (WiFi.status() != WL_CONNECTED){
            delay(100);
            Serial.print(".");
        }
        Serial.println();
        Serial.println("Successfully Connected to wifi!");


        //config routes here
        server.on("/", HTTP_GET, [this]() {
        //sendHtml();
        });

      server.begin();
      Serial.println("HTTP server started");
      Serial.println("Please open this url to access server => http://localhost:8180");
    }

    void ServerLogic::Update(){
        server.handleClient();
    }

    void ServerLogic::onNotify(const HouseState& state){
        
    }
}

