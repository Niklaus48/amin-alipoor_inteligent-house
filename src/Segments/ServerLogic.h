#pragma once

#include "IComponent.h"
#include "../Observer_Pattern/IObserver.h"
#include "../Observer_Pattern/ISubject.h"
#include "HouseState.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <uri/UriBraces.h>

namespace segments {

  class ServerLogic : public IComponent, public ISubject<HouseState>{
  public:

    //Constructor
    ServerLogic(const char* ssid, const char *pass, int32_t channel);
    void sendStaticHtml(const HouseState& state);
    void Init() override;
    void Update() override;

  private:
    WebServer server;
    const char* SSID;
    const char* Password;
    int32_t channel;
  };
}