#include "mock_esp_netif.h"

esp_err_t esp_netif_init(void)
{
    return MockEspNetif::getInstance().esp_netif_init();
}