#pragma once

#include <gmock/gmock.h>

#include "esp_netif.h"
#include "esp_netif_ip_addr.h"
#include "esp_netif_types.h"
#include "singleton_template.h"

class MockEspNetif : public SingletonTemplate<MockEspNetif>
{
   public:
    MOCK_METHOD(esp_err_t, esp_netif_init, (), ());
};
