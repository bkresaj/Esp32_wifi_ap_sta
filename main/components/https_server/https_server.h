#pragma once

#include <esp_https_server.h>
#include <spiffs.h>

#include <memory>

/**
 * @brief The HttpsServer class for managing an HTTP server.
 *
 * This class provides functionality for starting and stopping an HTTP server,
 * retrieving Wi-Fi SSID and password information, and clearing Wi-Fi SSID and
 * password.
 */
class HttpsServer
{
   public:
    /**
     * @brief Constructs an HttpsServer object with the specified Spiffs
     * instance.
     *
     * @param spiffs A pointer to the Spiffs instance used by the HTTP server.
     */
    explicit HttpsServer(Spiffs* spiffs) : spiffs_(spiffs)
    {
    }

    /**
     * @brief Starts the HTTP server.
     *
     * @return true if the server starts successfully, false otherwise.
     */
    bool startWebServer();

    /**
     * @brief Stops the HTTP server.
     *
     * @return true if the server stops successfully, false otherwise.
     */
    bool stopWebServer();

    /**
     * @brief Gets the Wi-Fi SSID.
     *
     * @return A string containing the Wi-Fi SSID, if available.
     */
    std::string getWifiSsid();

    /**
     * @brief Gets the Wi-Fi password.
     *
     * @return A string containing the Wi-Fi password, if available.
     */
    std::string getWifiPswd();

    /**
     * @brief Clears the Wi-Fi SSID and password.
     *
     * This function clears the stored Wi-Fi SSID and password information.
     */
    bool clearWifiSsidPswd();

   private:
    std::unique_ptr<Spiffs> spiffs_;
    static esp_err_t ssid_pswd_post_handler(httpd_req_t* req);
    static esp_err_t http_get_handler(httpd_req_t* req);
    httpd_handle_t server_{nullptr};
    std::string homepage_html_{};
    const std::string html_file_path_{"/spiffs/html_file"};
    static inline std::string ESP_WIFI_SSID_STA{};
    static inline std::string ESP_WIFI_PASS_STA{};
    static inline const std::string TAG_ = "Https server";
};
