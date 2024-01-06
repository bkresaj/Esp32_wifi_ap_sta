#pragma once

#include <cstdint>

#define IPSTR "%d.%d.%d.%d"
#define IP2STR(ipaddr) 1

struct esp_ip4_addr
{
    uint32_t addr; /*!< IPv4 address */
};

typedef struct esp_ip4_addr esp_ip4_addr_t;