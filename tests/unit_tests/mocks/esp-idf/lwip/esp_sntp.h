#pragma once

#ifndef SNTP_OPMODE_POLL
#define SNTP_OPMODE_POLL ESP_SNTP_OPMODE_POLL
#else
#warning "Defined!"
#endif /* SNTP_OPMODE_POLL */

typedef uint8_t u8_t;

typedef enum
{
    ESP_SNTP_OPMODE_POLL,
    ESP_SNTP_OPMODE_LISTENONLY,
} esp_sntp_operatingmode_t;

void esp_sntp_setoperatingmode(esp_sntp_operatingmode_t operating_mode);
void esp_sntp_setservername(u8_t idx, const char *server);
void esp_sntp_init(void);
