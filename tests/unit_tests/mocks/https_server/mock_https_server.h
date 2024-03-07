#pragma once

#include "https_server.h"

class MockHttpsServer : public SingletonTemplate<MockHttpsServer>
{
   public:
    MOCK_METHOD(bool, startWebServer, (), ());
    MOCK_METHOD(bool, stopWebServer, (), ());
    MOCK_METHOD(std::string, getWifiSsid, (), ());
    MOCK_METHOD(std::string, getWifiPswd, (), ());
    MOCK_METHOD(bool, clearWifiSsidPswd, (), ());
};