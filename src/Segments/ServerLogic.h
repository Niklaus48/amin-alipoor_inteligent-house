#pragma once

#include "IComponent.h"
#include "../Observer_Pattern/IObserver.h"
#include "HouseState.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <uri/UriBraces.h>

namespace segments {

  class ServerLogic : public IComponent, public IObserver<HouseState> {
  public:

    //Constructor
    ServerLogic(const char* ssid, const char *pass, int32_t channel);
    void sendHtml(const char* Html);
    void onNotify(const HouseState& state) override;
    void Init() override;
    void Update() override;

  private:
    WebServer server;
    const char* SSID;
    const char* Password;
    int32_t channel;
  };
}