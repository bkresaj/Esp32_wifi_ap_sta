#pragma once

#include "esp_netif_types.h"

esp_netif_t* esp_netif_create_default_wifi_ap(void);
esp_netif_t* esp_netif_create_default_wifi_sta(void);