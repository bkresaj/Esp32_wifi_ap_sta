#include "http_server.h"

#include <esp_log.h>

#include <cstring>

bool HttpServer::startWebServer()
{
    if (spiffs_ == nullptr)
    {
        ESP_LOGE(TAG_.data(), "Spiffs pointer is nullptr");
        return false;
    }

    const httpd_uri_t ssid_pswd = {.uri = "/",
                                   .method = HTTP_POST,
                                   .handler = ssid_pswd_post_handler,
                                   .user_ctx = NULL};

    size_t size_of_html_file = spiffs_->getFileSize(html_file_path_);

    if (size_of_html_file > 0)
    {
        homepage_html_.resize(size_of_html_file);
        spiffs_->readFile(html_file_path_, homepage_html_);
    }
    else
    {
        ESP_LOGE(TAG_.data(), "Reading html file failed");
        return false;
    }

    const httpd_uri_t homepage = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = http_get_handler,
        /* Let's pass response string in user
         * context to demonstrate it's usage */
        .user_ctx = reinterpret_cast<void*>(homepage_html_.data())};

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG_.data(), "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server_, &config) == ESP_OK)
    {
        ESP_LOGI(TAG_.data(), "Registering URI handlers");
        httpd_register_uri_handler(server_, &homepage);
        httpd_register_uri_handler(server_, &ssid_pswd);
        return true;
    }

    ESP_LOGI(TAG_.data(), "Error starting server!");
    return false;
}

bool HttpServer::stopWebServer()
{
    return httpd_stop(server_) == ESP_OK ? true : false;
}

std::string HttpServer::getWifiSsid()
{
    return ESP_WIFI_SSID_STA;
}

std::string HttpServer::getWifiPswd()
{
    return ESP_WIFI_PASS_STA;
}

bool HttpServer::clearWifiSsidPswd()
{
    ESP_WIFI_SSID_STA.clear();
    ESP_WIFI_PASS_STA.clear();
    return true;
}

/* An HTTP POST handler */
esp_err_t HttpServer::ssid_pswd_post_handler(httpd_req_t* req)
{
    char buf[150] = {0};
    // cppcheck-suppress variableScope
    int ret, remaining = req->content_len;
    while (remaining > 0)
    {
        /* Read the data for the request */
        if ((ret = httpd_req_recv(
                 req, buf,
                 (remaining < sizeof(buf) ? remaining : sizeof(buf)))) <= 0)
        {
            if (ret == HTTPD_SOCK_ERR_TIMEOUT)
            {
                /* Retry receiving if timeout occurred */
                continue;
            }
            return ESP_FAIL;
        }

        /* Send back the same data */
        httpd_resp_send_chunk(req, buf, ret);
        remaining -= ret;

        /* Log data received */
        ESP_LOGI(TAG_.data(), "=========== RECEIVED DATA ==========");
        ESP_LOGI(TAG_.data(), "%.*s", ret, buf);
        ESP_LOGI(TAG_.data(), "====================================");
    }

    const char* credentials = std::strstr(buf, "credentials");
    if (credentials != nullptr)
    {
        // Add +11 for credentials and +6 to get ssid
        const char* position = credentials + 11 + 5;

        // Extract the first variable until ","
        while (*position != ',' && *position != '\r')
        {
            ESP_WIFI_SSID_STA += *position;
            ++position;
        }

        // Skip the ","
        if (*position == ',')
        {
            ++position;
        }

        // Extract the next variable after "," and before "\r"
        while (*position != '\r')
        {
            ESP_WIFI_PASS_STA += *position;
            ++position;
        }
    }

    // End response
    httpd_resp_send_chunk(req, NULL, 0);

    return ESP_OK;
}

/* An HTTP GET handler */
esp_err_t HttpServer::http_get_handler(httpd_req_t* req)
{
    /* Get header value string length and allocate memory for length + 1,
     * extra byte for null termination */
    ESP_LOGI(TAG_.data(), "URI : %s", req->uri);

    const char* resp_str = reinterpret_cast<const char*>(req->user_ctx);
    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);

    /* After sending the HTTP response the old HTTP request
     * headers are lost. Check if HTTP request headers can be read now. */
    if (httpd_req_get_hdr_value_len(req, "Host") == 0)
    {
        ESP_LOGI(TAG_.data(), "Request headers lost");
    }
    return ESP_OK;
}