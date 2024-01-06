#pragma once

#include <gmock/gmock.h>

#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "singleton_template.h"

class MockEspWifi : public SingletonTemplate<MockEspWifi>
{
   public:
    MOCK_METHOD(esp_netif_t *, esp_netif_create_default_wifi_ap, (), ());
    MOCK_METHOD(esp_netif_t *, esp_netif_create_default_wifi_sta, (), ());
    MOCK_METHOD(esp_err_t, esp_wifi_set_mode, (wifi_mode_t mode), ());
    MOCK_METHOD(esp_err_t, esp_wifi_set_config,
                (wifi_interface_t interface, wifi_config_t *conf), ());
    MOCK_METHOD(esp_err_t, esp_wifi_start, (), ());
    MOCK_METHOD(esp_err_t, esp_wifi_stop, (), ());
    MOCK_METHOD(esp_err_t, esp_wifi_disconnect, (), ());
    MOCK_METHOD(esp_err_t, esp_wifi_init, (const wifi_init_config_t *config),
                ());
    MOCK_METHOD(esp_err_t, esp_wifi_connect, (), ());
};
