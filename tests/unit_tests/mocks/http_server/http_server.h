#pragma once

#include <gmock/gmock.h>
#include <mock_spiffs.h>
#include <singleton_template.h>

class HttpServer
{
   public:
    HttpServer(Spiffs* spiffs);
    bool startWebServer();
    bool stopWebServer();
    std::string getWifiSsid();
    std::string getWifiPswd();
    bool clearWifiSsidPswd();
};
