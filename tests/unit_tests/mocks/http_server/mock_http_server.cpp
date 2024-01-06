#include "mock_http_server.h"

HttpServer::HttpServer(Spiffs* spiffs)
{
}

bool HttpServer::startWebServer()
{
    return MockHttpServer::getInstance().startWebServer();
}

bool HttpServer::stopWebServer()
{
    return MockHttpServer::getInstance().stopWebServer();
}

std::string HttpServer::getWifiSsid()
{
    return MockHttpServer::getInstance().getWifiSsid();
}

std::string HttpServer::getWifiPswd()
{
    return MockHttpServer::getInstance().getWifiPswd();
}

bool HttpServer::clearWifiSsidPswd()
{
    return MockHttpServer::getInstance().clearWifiSsidPswd();
}
