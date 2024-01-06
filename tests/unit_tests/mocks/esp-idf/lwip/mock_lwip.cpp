#include "mock_lwip.h"

void esp_sntp_setoperatingmode(esp_sntp_operatingmode_t operating_mode)
{
    MockLwip::getInstance().esp_sntp_setoperatingmode(operating_mode);
}

void esp_sntp_setservername(u8_t idx, const char *server)
{
    MockLwip::getInstance().esp_sntp_setservername(idx, server);
}

void esp_sntp_init(void)
{
    MockLwip::getInstance().esp_sntp_init();
}