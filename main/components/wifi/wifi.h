#ifndef WIFI_H_
#define WIFI_H_

#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "http_server.h"

struct WifiConfig
{
    const std::string_view esp_wifi_ap_ssid;
    const std::string_view esp_wifi_ap_pass;
    const uint8_t esp_wifi_channel;
    const uint8_t max_sta_conn;
    const uint8_t esp_maximum_retry;
    const int wifi_connected_bit;
    const int wifi_fail_bit;
    const wifi_auth_mode_t esp_wifi_scan_auth_mode_threshold;
};

/**
 * @class Wifi
 * @brief State machine for Wifi modes
 */
class Wifi
{
   public:
    static Wifi& getInstance();

    ~Wifi() = default;
    /**
     * Function for starting access point mode.
     *
     * @return Bool returning true on success or false on failure.
     */
    bool startAP();

    /**
     * Function for starting station mode.
     *
     * @return Bool returning true on success or false on failure.
     */
    bool startSTA();

    /**
     * Function for stoping access point mode.
     *
     * @return Bool returning true on success or false on failure.
     */
    bool stopAP();

    /**
     * Function for stoping station mode.
     *
     * @return Bool returning true on success or false on failure.
     */
    bool stopSTA();

    /**
     * Function for getting ssid of current network in station Wifi mode.
     *
     * @return String returning ssid.
     */
    std::string getWifiSsidSta();

    std::string getWifiPasswordSta();

    bool getWifiStaStatus();

   private:
    Wifi();

    static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                   int32_t event_id, void* event_data);
    static void event_handler(void* arg, esp_event_base_t event_base,
                              int32_t event_id, void* event_data);

    static constexpr WifiConfig wifi_config_{
        .esp_wifi_ap_ssid = "Esp32test",
        .esp_wifi_ap_pass = "esp32test",
        .esp_wifi_channel = 1,
        .max_sta_conn = 5,
        .esp_maximum_retry = 3,
        .wifi_connected_bit = BIT0,
        .wifi_fail_bit = BIT1,
        .esp_wifi_scan_auth_mode_threshold = WIFI_AUTH_WPA2_PSK};

    HttpServer* http_server_{nullptr};
    esp_netif_t* m_apNetif_{nullptr};
    esp_netif_t* m_staNetif_{nullptr};
    static inline EventGroupHandle_t s_wifi_event_group{};
    static inline bool wifi_sta_connected_{false};

    static inline const std::string TAG_ = "Wifi";
};

#endif  // WIFI_FSM_H