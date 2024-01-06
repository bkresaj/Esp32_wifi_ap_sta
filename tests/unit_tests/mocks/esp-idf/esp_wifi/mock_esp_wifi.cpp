#include "mock_esp_wifi.h"

esp_netif_t *esp_netif_create_default_wifi_ap(void)
{
    return MockEspWifi::getInstance().esp_netif_create_default_wifi_ap();
}

esp_netif_t *esp_netif_create_default_wifi_sta(void)
{
    return MockEspWifi::getInstance().esp_netif_create_default_wifi_sta();
}

esp_err_t esp_wifi_set_mode(wifi_mode_t mode)
{
    return MockEspWifi::getInstance().esp_wifi_set_mode(mode);
}

esp_err_t esp_wifi_set_config(wifi_interface_t interface, wifi_config_t *conf)
{
    return MockEspWifi::getInstance().esp_wifi_set_config(interface, conf);
}

esp_err_t esp_wifi_start()
{
    return MockEspWifi::getInstance().esp_wifi_start();
}

esp_err_t esp_wifi_stop(void)
{
    return MockEspWifi::getInstance().esp_wifi_stop();
}

esp_err_t esp_wifi_disconnect(void)
{
    return MockEspWifi::getInstance().esp_wifi_disconnect();
}

esp_err_t esp_wifi_init(const wifi_init_config_t *config)
{
    return MockEspWifi::getInstance().esp_wifi_init(config);
}

esp_err_t esp_wifi_connect(void)
{
    return MockEspWifi::getInstance().esp_wifi_connect();
}