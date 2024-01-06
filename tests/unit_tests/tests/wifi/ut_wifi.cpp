#include <gtest/gtest.h>

#include "esp_log.h"
#include "mock_esp_event.h"
#include "mock_esp_netif.h"
#include "mock_esp_wifi.h"
#include "mock_freertos.h"
#include "mock_http_server.h"
#include "wifi.h"

using namespace testing;

class UtWifi : public ::Test
{
   public:
    StrictMock<MockEspEvent> mock_esp_event;
    StrictMock<MockFreertos> mock_freertos;
    StrictMock<MockEspWifi> mock_esp_wifi;
    StrictMock<MockHttpServer> mock_http_server;
    StrictMock<MockEspNetif> mock_esp_netif;

    void expectedInitializationFunctionsInsideStartAP()
    {
        EXPECT_CALL(mock_esp_wifi, esp_netif_create_default_wifi_ap()).Times(1);
        EXPECT_CALL(mock_esp_wifi, esp_wifi_set_mode(_)).Times(1);
        EXPECT_CALL(mock_esp_wifi, esp_wifi_set_config(_, _)).Times(1);
        EXPECT_CALL(mock_esp_wifi, esp_wifi_start()).Times(1);
    }

    void expectedInitializationFunctionsInsideStartSTA()
    {
        EXPECT_CALL(mock_freertos, xEventGroupCreate())
            .WillOnce(Return((EventGroupHandle_t)1));
        EXPECT_CALL(mock_esp_wifi, esp_netif_create_default_wifi_sta())
            .Times(1);
        EXPECT_CALL(mock_http_server, getWifiSsid()).Times(1);
        EXPECT_CALL(mock_http_server, getWifiPswd()).Times(1);
        EXPECT_CALL(mock_esp_wifi, esp_wifi_set_mode(_)).Times(1);
        EXPECT_CALL(mock_esp_wifi, esp_wifi_set_config(_, _)).Times(1);
        EXPECT_CALL(mock_esp_wifi, esp_wifi_start()).Times(1);
    }
};

TEST_F(UtWifi, wifiObjectCreationCallsCorrectFunctions)
{
    InSequence s;

    EXPECT_CALL(mock_esp_netif, esp_netif_init()).Times(1);
    EXPECT_CALL(mock_esp_event, esp_event_loop_create_default()).Times(1);
    EXPECT_CALL(mock_esp_wifi, esp_wifi_init(_)).Times(1);
    EXPECT_CALL(mock_esp_wifi, esp_wifi_set_mode(_)).Times(1);
    EXPECT_CALL(mock_esp_event,
                esp_event_handler_instance_register(_, _, _, _, _))
        .Times(3);

    Wifi& wifi_fsm = Wifi::getInstance();
}

TEST_F(UtWifi, wifiStartAPFailsBecauseStartWebserverFails)
{
    InSequence s;

    expectedInitializationFunctionsInsideStartAP();

    EXPECT_CALL(mock_http_server, startWebServer()).WillOnce(Return(false));

    EXPECT_FALSE(Wifi::getInstance().startAP());
}

TEST_F(UtWifi, wifiStartAPSuccess)
{
    InSequence s;

    expectedInitializationFunctionsInsideStartAP();

    EXPECT_CALL(mock_http_server, startWebServer()).WillOnce(Return(true));

    EXPECT_TRUE(Wifi::getInstance().startAP());
}

TEST_F(UtWifi, wifiStartSTAFailsBecauseEventGroupIsNotCreated)
{
    InSequence s;

    EXPECT_CALL(mock_freertos, xEventGroupCreate()).WillOnce(Return(nullptr));

    EXPECT_FALSE(Wifi::getInstance().startSTA());
}

TEST_F(UtWifi, wifiStartSTAFailsBecauseWifiDoesntConnect)
{
    InSequence s;

    expectedInitializationFunctionsInsideStartSTA();

    EXPECT_CALL(mock_freertos, xEventGroupWaitBits(_, _, _, _, _))
        .WillOnce(Return(BIT1));

    EXPECT_FALSE(Wifi::getInstance().startSTA());
}

TEST_F(UtWifi, wifiStartSTASuccess)
{
    InSequence s;

    expectedInitializationFunctionsInsideStartSTA();

    EXPECT_CALL(mock_freertos, xEventGroupWaitBits(_, _, _, _, _))
        .WillOnce(Return(BIT0));

    EXPECT_TRUE(Wifi::getInstance().startSTA());
}

TEST_F(UtWifi, wifiStopAPFailsWhenStopWebServerFails)
{
    InSequence s;

    EXPECT_CALL(mock_http_server, stopWebServer()).WillOnce(Return(false));
    EXPECT_CALL(mock_esp_wifi, esp_wifi_stop()).Times(0);
    EXPECT_CALL(mock_esp_wifi, esp_wifi_set_mode(_)).Times(0);

    EXPECT_FALSE(Wifi::getInstance().stopAP());
}

TEST_F(UtWifi, wifiStopAPFailsWhenEspWifiStopFails)
{
    InSequence s;

    EXPECT_CALL(mock_http_server, stopWebServer()).WillOnce(Return(true));
    EXPECT_CALL(mock_esp_wifi, esp_wifi_stop()).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(mock_esp_wifi, esp_wifi_set_mode(_)).Times(0);

    EXPECT_FALSE(Wifi::getInstance().stopAP());
}

TEST_F(UtWifi, wifiStopAPFailsWhenEspWifiSetModeFails)
{
    InSequence s;

    EXPECT_CALL(mock_http_server, stopWebServer()).WillOnce(Return(true));
    EXPECT_CALL(mock_esp_wifi, esp_wifi_stop()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mock_esp_wifi, esp_wifi_set_mode(_)).WillOnce(Return(ESP_FAIL));

    EXPECT_FALSE(Wifi::getInstance().stopAP());
}

TEST_F(UtWifi, wifiStopAPSuccess)
{
    InSequence s;

    EXPECT_CALL(mock_http_server, stopWebServer()).WillOnce(Return(true));
    EXPECT_CALL(mock_esp_wifi, esp_wifi_stop()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mock_esp_wifi, esp_wifi_set_mode(_)).WillOnce(Return(ESP_OK));

    EXPECT_TRUE(Wifi::getInstance().stopAP());
}

TEST_F(UtWifi, wifiStopSTAFailsWhenEspWifiDisconnectFails)
{
    InSequence s;

    EXPECT_CALL(mock_http_server, clearWifiSsidPswd()).Times(1);
    EXPECT_CALL(mock_esp_wifi, esp_wifi_disconnect())
        .WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(mock_esp_wifi, esp_wifi_stop()).Times(0);
    EXPECT_CALL(mock_esp_wifi, esp_wifi_set_mode(WIFI_MODE_NULL)).Times(0);
    EXPECT_CALL(mock_freertos, vEventGroupDelete(_)).Times(0);

    EXPECT_FALSE(Wifi::getInstance().stopSTA());
}

TEST_F(UtWifi, wifiStopSTAFailsWhenEspWifiStopFails)
{
    InSequence s;

    EXPECT_CALL(mock_http_server, clearWifiSsidPswd()).Times(1);
    EXPECT_CALL(mock_esp_wifi, esp_wifi_disconnect()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mock_esp_wifi, esp_wifi_stop()).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(mock_esp_wifi, esp_wifi_set_mode(WIFI_MODE_NULL)).Times(0);
    EXPECT_CALL(mock_freertos, vEventGroupDelete(_)).Times(0);

    EXPECT_FALSE(Wifi::getInstance().stopSTA());
}

TEST_F(UtWifi, wifiStopSTAFailsWhenEspWifiSetModeFails)
{
    InSequence s;

    EXPECT_CALL(mock_http_server, clearWifiSsidPswd()).Times(1);
    EXPECT_CALL(mock_esp_wifi, esp_wifi_disconnect()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mock_esp_wifi, esp_wifi_stop()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mock_esp_wifi, esp_wifi_set_mode(WIFI_MODE_NULL))
        .WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(mock_freertos, vEventGroupDelete(_)).Times(0);

    EXPECT_FALSE(Wifi::getInstance().stopSTA());
}

TEST_F(UtWifi, wifiStopSTASuccess)
{
    InSequence s;

    EXPECT_CALL(mock_http_server, clearWifiSsidPswd()).Times(1);
    EXPECT_CALL(mock_esp_wifi, esp_wifi_disconnect()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mock_esp_wifi, esp_wifi_stop()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mock_esp_wifi, esp_wifi_set_mode(WIFI_MODE_NULL))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mock_freertos, vEventGroupDelete(_)).Times(1);

    EXPECT_TRUE(Wifi::getInstance().stopSTA());
}
