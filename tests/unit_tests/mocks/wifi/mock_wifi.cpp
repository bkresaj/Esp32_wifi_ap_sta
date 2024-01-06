#include "mock_wifi.h"

Wifi& Wifi::getInstance()
{
    static Wifi wifi;
    return wifi;
}

bool Wifi::startAP()
{
    return MockWifi::getInstance().startAP();
}

bool Wifi::startSTA()
{
    return MockWifi::getInstance().startSTA();
}

bool Wifi::stopAP()
{
    return MockWifi::getInstance().stopAP();
}

bool Wifi::stopSTA()
{
    return MockWifi::getInstance().stopSTA();
}

std::string Wifi::getEspWifiSsidSta()
{
    return MockWifi::getInstance().getEspWifiSsidSta();
}

bool Wifi::getTimeDate(char* time_buffer, size_t size)
{
    return MockWifi::getInstance().getTimeDate(time_buffer, size);
}