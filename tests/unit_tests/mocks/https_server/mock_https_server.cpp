#include "mock_https_server.h"

HttpsServer::HttpsServer(Spiffs* spiffs)
{
}

bool HttpsServer::startWebServer()
{
    return MockHttpsServer::getInstance().startWebServer();
}

bool HttpsServer::stopWebServer()
{
    return MockHttpsServer::getInstance().stopWebServer();
}

std::string HttpsServer::getWifiSsid()
{
    return MockHttpsServer::getInstance().getWifiSsid();
}

std::string HttpsServer::getWifiPswd()
{
    return MockHttpsServer::getInstance().getWifiPswd();
}

bool HttpsServer::clearWifiSsidPswd()
{
    return MockHttpsServer::getInstance().clearWifiSsidPswd();
}
