#pragma once

#include "esp_netif_ip_addr.h"

struct esp_netif_obj;

typedef struct esp_netif_obj esp_netif_t;

typedef enum
{
    IP_EVENT_STA_GOT_IP,       /*!< station got IP from connected AP */
    IP_EVENT_STA_LOST_IP,      /*!< station lost IP and the IP is reset to 0 */
    IP_EVENT_AP_STAIPASSIGNED, /*!< soft-AP assign an IP to a connected station
                                */
    IP_EVENT_GOT_IP6,     /*!< station or ap or ethernet interface v6IP addr is
                             preferred */
    IP_EVENT_ETH_GOT_IP,  /*!< ethernet got IP from connected AP */
    IP_EVENT_ETH_LOST_IP, /*!< ethernet lost IP and the IP is reset to 0 */
    IP_EVENT_PPP_GOT_IP,  /*!< PPP interface got IP */
    IP_EVENT_PPP_LOST_IP, /*!< PPP interface lost IP */
} ip_event_t;

typedef struct
{
    esp_ip4_addr_t ip;      /**< Interface IPV4 address */
    esp_ip4_addr_t netmask; /**< Interface IPV4 netmask */
    esp_ip4_addr_t gw;      /**< Interface IPV4 gateway address */
} esp_netif_ip_info_t;

typedef struct
{
    esp_netif_t *esp_netif; /*!< Pointer to corresponding esp-netif object */
    esp_netif_ip_info_t ip_info; /*!< IP address, netmask, gatway IP address */
    bool ip_changed; /*!< Whether the assigned IP has changed or not */
} ip_event_got_ip_t;
