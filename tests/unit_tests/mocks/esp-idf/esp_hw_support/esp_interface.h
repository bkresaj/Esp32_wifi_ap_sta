#pragma once

typedef enum
{
    ESP_IF_WIFI_STA = 0, /**< Station interface */
    ESP_IF_WIFI_AP,      /**< Soft-AP interface */
    ESP_IF_WIFI_NAN,     /**< NAN interface */
    ESP_IF_ETH,          /**< Ethernet interface */
    ESP_IF_MAX
} esp_interface_t;