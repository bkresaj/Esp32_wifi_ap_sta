#pragma once

#include "http_server.h"

class MockHttpServer : public SingletonTemplate<MockHttpServer>
{
   public:
    MOCK_METHOD(bool, startWebServer, (), ());
    MOCK_METHOD(bool, stopWebServer, (), ());
    MOCK_METHOD(std::string, getWifiSsid, (), ());
    MOCK_METHOD(std::string, getWifiPswd, (), ());
    MOCK_METHOD(bool, clearWifiSsidPswd, (), ());
};