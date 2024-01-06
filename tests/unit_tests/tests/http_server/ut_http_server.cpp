#include <gtest/gtest.h>
#include <http_server.h>

#include "esp_log.h"
#include "mock_esp_http_server.h"
#include "mock_spiffs.h"

using namespace testing;

class UtHttpServer : public ::Test
{
   public:
    StrictMock<MockSpiffs> mock_spiffs;
    StrictMock<MockEspHttpServer> mock_esp_http_server;
};

TEST_F(UtHttpServer, startWebServerFailsWhenSpiffsPointerIsNullptr)
{
    HttpServer http_server(nullptr);

    EXPECT_FALSE(http_server.startWebServer());
}

TEST_F(UtHttpServer,
       startWebServerFailsWhenSpiffsGetFileSizeReadsIncorrectLength)
{
    Spiffs* spiffs = new Spiffs();
    HttpServer http_server(spiffs);

    EXPECT_CALL(mock_spiffs, getFileSize(_)).WillOnce(Return(0));

    EXPECT_FALSE(http_server.startWebServer());
}

TEST_F(UtHttpServer, startWebServerFailsWhenHttpdStartFails)
{
    Spiffs* spiffs = new Spiffs();
    HttpServer http_server(spiffs);

    InSequence s;

    EXPECT_CALL(mock_spiffs, getFileSize(_)).WillOnce(Return(1));

    EXPECT_CALL(mock_spiffs, readFile(_, _)).Times(1);

    EXPECT_CALL(mock_esp_http_server, httpd_start(_, _))
        .WillOnce(Return(ESP_FAIL));

    EXPECT_FALSE(http_server.startWebServer());
}

TEST_F(UtHttpServer, startWebServerSuccess)
{
    Spiffs* spiffs = new Spiffs();
    HttpServer http_server(spiffs);

    InSequence s;

    EXPECT_CALL(mock_spiffs, getFileSize(_)).WillOnce(Return(1));

    EXPECT_CALL(mock_spiffs, readFile(_, _)).Times(1);

    EXPECT_CALL(mock_esp_http_server, httpd_start(_, _))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mock_esp_http_server, httpd_register_uri_handler(_, _))
        .Times(2);

    EXPECT_TRUE(http_server.startWebServer());
}

TEST_F(UtHttpServer, stopWebServerFailsWhenHttpdStopFails)
{
    Spiffs* spiffs = new Spiffs();
    HttpServer http_server(spiffs);

    EXPECT_CALL(mock_esp_http_server, httpd_stop(_)).WillOnce(Return(ESP_FAIL));

    EXPECT_FALSE(http_server.stopWebServer());
}

TEST_F(UtHttpServer, stopWebServerSuccess)
{
    Spiffs* spiffs = new Spiffs();
    HttpServer http_server(spiffs);

    EXPECT_CALL(mock_esp_http_server, httpd_stop(_)).WillOnce(Return(ESP_OK));

    EXPECT_TRUE(http_server.stopWebServer());
}
