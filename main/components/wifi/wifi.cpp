#include "wifi.h"

#include "esp_event.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "esp_netif.h"

Wifi::Wifi()
{
    static Spiffs spiffs;
    static HttpServer http_server(&spiffs);
    http_server_ = &http_server;

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg;
    cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip));
}

Wifi& Wifi::getInstance()
{
    static Wifi instance;
    return instance;
}

bool Wifi::startAP()
{
    if (!m_apNetif_)
    {
        m_apNetif_ = esp_netif_create_default_wifi_ap();
    }

    wifi_config_t wifi_config = {
        .ap = {.ssid = {},
               .password = {},
               .ssid_len = wifi_config_.esp_wifi_ap_ssid.size(),
               .channel = wifi_config_.esp_wifi_channel,
               .authmode = wifi_config_.esp_wifi_scan_auth_mode_threshold,
               .max_connection = wifi_config_.max_sta_conn}};

    if (wifi_config_.esp_wifi_ap_ssid.size() > 0)
    {
        for (int i = 0; i < wifi_config_.esp_wifi_ap_ssid.size(); i++)
        {
            wifi_config.ap.ssid[i] =
                static_cast<uint8_t>(wifi_config_.esp_wifi_ap_ssid[i]);
        };
    }

    if (wifi_config_.esp_wifi_ap_pass.size() > 0)
    {
        for (int j = 0; j < wifi_config_.esp_wifi_ap_pass.size(); j++)
        {
            wifi_config.ap.password[j] =
                static_cast<uint8_t>(wifi_config_.esp_wifi_ap_pass[j]);
        };
    }

    ESP_LOGI(TAG_.data(), "Password: %s", wifi_config.ap.password);

    if (wifi_config_.esp_wifi_ap_pass.size() == 0)
    {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG_.data(), "wifi_init_softap finished.");

    if (http_server_->startWebServer())
    {
        ESP_LOGI(TAG_.data(), "Webserver started!");
        ESP_LOGI(TAG_.data(),
                 "IP address for accesing webpage after connecting is : "
                 "192.168.4.1");
        return true;
    }
    else
    {
        ESP_LOGE(TAG_.data(), "Error starting webserver!");
        return false;
    }
}

bool Wifi::startSTA()
{
    s_wifi_event_group = xEventGroupCreate();

    if (s_wifi_event_group == nullptr)
    {
        return false;
    }

    if (!m_staNetif_)
    {
        m_staNetif_ = esp_netif_create_default_wifi_sta();
    }

    wifi_config_t wifi_config = {0};

    std::string ESP_WIFI_SSID_STA{http_server_->getWifiSsid()};
    std::string ESP_WIFI_PASS_STA{http_server_->getWifiPswd()};

    if (ESP_WIFI_SSID_STA.size() > 0)
    {
        for (int i = 0; i < ESP_WIFI_SSID_STA.size(); i++)
        {
            wifi_config.sta.ssid[i] =
                static_cast<uint8_t>(ESP_WIFI_SSID_STA[i]);
        };
    }

    if (ESP_WIFI_PASS_STA.size() > 0)
    {
        for (int j = 0; j < ESP_WIFI_PASS_STA.size(); j++)
        {
            wifi_config.sta.password[j] =
                static_cast<uint8_t>(ESP_WIFI_PASS_STA[j]);
        };
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG_.data(), "wifi_init_sta finished.");

    /* Waiting until either the connection is established (wifi_connected_bit)
     * or connection failed for the maximum number of re-tries (wifi_fail_bit).
     * The bits are set by event_handler() (see above) */

    EventBits_t bits = xEventGroupWaitBits(
        s_wifi_event_group,
        wifi_config_.wifi_connected_bit | wifi_config_.wifi_fail_bit, pdFALSE,
        pdFALSE, portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we
     * can test which event actually happened. */
    if (bits & wifi_config_.wifi_connected_bit)
    {
        ESP_LOGI(TAG_.data(), "connected to ap SSID:%s password:%s",
                 ESP_WIFI_SSID_STA.c_str(), ESP_WIFI_PASS_STA.c_str());
        return true;
    }
    else if (bits & wifi_config_.wifi_fail_bit)
    {
        ESP_LOGI(TAG_.data(), "Failed to connect to SSID:%s, password:%s",
                 ESP_WIFI_SSID_STA.c_str(), ESP_WIFI_PASS_STA.c_str());
    }
    else
    {
        ESP_LOGE(TAG_.data(), "UNEXPECTED EVENT");
    }

    return false;
}

bool Wifi::stopAP()
{
    if (!http_server_->stopWebServer())
    {
        return false;
    }
    if (esp_wifi_stop() != ESP_OK)
    {
        return false;
    }
    if (esp_wifi_set_mode(WIFI_MODE_NULL) != ESP_OK)
    {
        return false;
    }
    return true;
}

bool Wifi::stopSTA()
{
    http_server_->clearWifiSsidPswd();
    if (esp_wifi_disconnect() != ESP_OK)
    {
        return false;
    }
    if (esp_wifi_stop() != ESP_OK)
    {
        return false;
    }
    if (esp_wifi_set_mode(WIFI_MODE_NULL) != ESP_OK)
    {
        return false;
    }
    vEventGroupDelete(s_wifi_event_group);
    return true;
}

std::string Wifi::getWifiSsidSta()
{
    return http_server_->getWifiSsid();
}

// cppcheck-suppress unusedFunction
std::string Wifi::getWifiPasswordSta()
{
    return http_server_->getWifiPswd();
}

bool Wifi::getWifiStaStatus()
{
    return wifi_sta_connected_;
}

void Wifi::wifi_event_handler(void* arg, esp_event_base_t event_base,
                              int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED)
    {
        wifi_event_ap_staconnected_t* event =
            static_cast<wifi_event_ap_staconnected_t*>(event_data);
        ESP_LOGI(TAG_.data(), "station " MACSTR " join, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
    else if (event_id == WIFI_EVENT_AP_STADISCONNECTED)
    {
        wifi_event_ap_stadisconnected_t* event =
            static_cast<wifi_event_ap_stadisconnected_t*>(event_data);
        ESP_LOGI(TAG_.data(), "station " MACSTR " leave, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
}

void Wifi::event_handler(void* arg, esp_event_base_t event_base,
                         int32_t event_id, void* event_data)
{
    static int s_retry_num = 0;
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT &&
             event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (s_retry_num < wifi_config_.esp_maximum_retry)
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG_.data(), "retry to connect to the AP");
        }
        else
        {
            xEventGroupSetBits(s_wifi_event_group, wifi_config_.wifi_fail_bit);
            wifi_sta_connected_ = false;
        }
        ESP_LOGI(TAG_.data(), "connect to the AP fail");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t* event = static_cast<ip_event_got_ip_t*>(event_data);
        ESP_LOGI(TAG_.data(), "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, wifi_config_.wifi_connected_bit);
        wifi_sta_connected_ = true;
    }
}
