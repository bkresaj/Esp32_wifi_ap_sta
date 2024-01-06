#pragma once

#include "wifi.h"

class MockWifi : public SingletonTemplate<MockWifi>
{
   public:
    MOCK_METHOD(bool, startAP, (), ());
    MOCK_METHOD(bool, startSTA, (), ());
    MOCK_METHOD(bool, stopAP, (), ());
    MOCK_METHOD(bool, stopSTA, (), ());
    MOCK_METHOD(std::string, getEspWifiSsidSta, (), ());
    MOCK_METHOD(bool, getTimeDate, (char* time_buffer, size_t size), ());
};