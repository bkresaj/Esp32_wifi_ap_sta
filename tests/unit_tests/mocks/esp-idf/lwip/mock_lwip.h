#pragma once

#include <gmock/gmock.h>

#include "esp_sntp.h"
#include "lwip/ip_addr.h"
#include "singleton_template.h"

class MockLwip : public SingletonTemplate<MockLwip>
{
   public:
    MOCK_METHOD(void, esp_sntp_setoperatingmode,
                (esp_sntp_operatingmode_t operating_mode), ());
    MOCK_METHOD(void, esp_sntp_setservername, (u8_t idx, const char *server),
                ());
    MOCK_METHOD(void, esp_sntp_init, (), ());
};
