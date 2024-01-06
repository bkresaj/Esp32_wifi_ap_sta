#pragma once

#include <gmock/gmock.h>
#include <singleton_template.h>

#include <string>

class Wifi
{
   public:
    static Wifi& getInstance();
    bool startAP();
    bool startSTA();
    bool stopAP();
    bool stopSTA();
    std::string getEspWifiSsidSta();
    bool getTimeDate(char* time_buffer, size_t size);
};
