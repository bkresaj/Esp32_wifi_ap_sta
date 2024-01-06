#include "ntp_server.h"

#include "esp_log.h"
#include "esp_sntp.h"

static bool ntp_server_initialized = false;

bool initNtpServer()
{
    if (!ntp_server_initialized)
    {
        ntp_server_initialized = true;
        esp_sntp_setoperatingmode((esp_sntp_operatingmode_t)SNTP_OPMODE_POLL);
        esp_sntp_setservername(0, "pool.ntp.org");

        esp_sntp_init();

        ESP_LOGI("NTP_SERVER", "Ntp server properly initialized");
        return true;
    }
    else
    {
        return false;
    }
}

bool getTimeDate(char* time_buffer, size_t size)
{
    initNtpServer();

    if (ntp_server_initialized)
    {
        time_t now;
        struct tm timeinfo;

        setenv("TZ", "GMT-1", 1);
        tzset();

        time(&now);
        localtime_r(&now, &timeinfo);

        if (timeinfo.tm_year != 70)
        {
            strftime(time_buffer, size, "%T %d-%m-%Y ", &timeinfo);
            return true;
        }
        return false;
    }
    else
    {
        ESP_LOGE("NTP_SERVER", "Ntp server not initialized");
        return false;
    }
}
