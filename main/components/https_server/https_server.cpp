#include "https_server.h"

#include <esp_log.h>

#include <cstring>

bool HttpsServer::startWebServer()
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

    httpd_ssl_config_t config = HTTPD_SSL_CONFIG_DEFAULT();
    const uint8_t servercert[] =
        "-----BEGIN CERTIFICATE-----\r\n"
        "MIIDPDCCAiSgAwIBAgIUDJ9umlf3+GRYGXBVsESaDn4xCEkwDQYJKoZIhvcNAQEL\r\n"
        "BQAwJTEjMCEGA1UEAwwaRVNQMzIgSFRUUFMgc2VydmVyIGV4YW1wbGUwHhcNMjQw\r\n"
        "MzA2MTYyMjE5WhcNMzQwMzA0MTYyMjE5WjAlMSMwIQYDVQQDDBpFU1AzMiBIVFRQ\r\n"
        "UyBzZXJ2ZXIgZXhhbXBsZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\r\n"
        "ALUNFJVZkyhNUuJEssCtpCHVWYgTcJbwqX+AXZWhZh7h+n/xe3GtpHvKu8cYlDfS\r\n"
        "bgdO3pif1Oi1kBisdIpghtZ92f/SN3pMyaJ01ytDSxT0qV84jdDJA/s0XixS3Oj2\r\n"
        "UPkHg+L41dbzrlKFuHzoef9DXFRiiHU+wtoAAvlUwYAZcEt4SKjt6MGa5EUesDVZ\r\n"
        "OQ/wQ6EEIStfI8ET5B4eB3XOzQgI4SYa3w46QJ2Z+RSXhEmjwJDdALvCWHB7Yxeo\r\n"
        "1eJtUXwexNBT/J+PzNxVcIkfzn8s4TUp+JzKXPflbn5Lgu9+sCTL4zHOz1vSGGNy\r\n"
        "uTg0NF9YamXI6qfrDWEtgEcCAwEAAaNkMGIwHQYDVR0OBBYEFORoOhFTzfENafDa\r\n"
        "b0QTRvUOgDIlMB8GA1UdIwQYMBaAFORoOhFTzfENafDab0QTRvUOgDIlMA8GA1Ud\r\n"
        "EwEB/wQFMAMBAf8wDwYDVR0RBAgwBocEwKgEATANBgkqhkiG9w0BAQsFAAOCAQEA\r\n"
        "ggEuoKHVpmgF2ONfq6LlUBEiv+tFxv+v0SJBazeBprWtyfzFux7KBCYIp/Lqs1zR\r\n"
        "E0zK83DHZiP2tqplpx/WOLFi/Bd9flXT+3EhqWNhe8M2/zTqbDGFiNvFo5MGt9Jt\r\n"
        "CDNpUF4cyhOeKjuuS7y0Xjdb+qPzuPjFiOzzTNbi2ESkOTB5TQ8E/3jl9xa8MIjL\r\n"
        "lmhrNaTgMChlRbnRIC0tx9wtjNjzIMZY4wTqdKu+Q0zOoGbPiad3OzqEQtUQbYXf\r\n"
        "6eRIzKttf24Jm1e/PSLCwvZ1HfTVJQ+g8s5LGL5kBtKlHsY83WobwVsMNOieVIBQ\r\n"
        "sM/Mxo1GZ3HE17S7Qdfo1Q==\r\n"
        "-----END CERTIFICATE-----\r\n";
    config.servercert = servercert;
    config.servercert_len = sizeof(servercert) / sizeof(servercert[0]);

    ESP_LOGI("********", "SERVER CERT LENGTH %d", config.servercert_len);

    const uint8_t prvtkey_pem[] =
        "-----BEGIN PRIVATE KEY-----\r\n"
        "MIIEvwIBADANBgkqhkiG9w0BAQEFAASCBKkwggSlAgEAAoIBAQC1DRSVWZMoTVLi\r\n"
        "RLLAraQh1VmIE3CW8Kl/gF2VoWYe4fp/8XtxraR7yrvHGJQ30m4HTt6Yn9TotZAY\r\n"
        "rHSKYIbWfdn/0jd6TMmidNcrQ0sU9KlfOI3QyQP7NF4sUtzo9lD5B4Pi+NXW865S\r\n"
        "hbh86Hn/Q1xUYoh1PsLaAAL5VMGAGXBLeEio7ejBmuRFHrA1WTkP8EOhBCErXyPB\r\n"
        "E+QeHgd1zs0ICOEmGt8OOkCdmfkUl4RJo8CQ3QC7wlhwe2MXqNXibVF8HsTQU/yf\r\n"
        "j8zcVXCJH85/LOE1Kficylz35W5+S4LvfrAky+Mxzs9b0hhjcrk4NDRfWGplyOqn\r\n"
        "6w1hLYBHAgMBAAECggEAFxCUh7ZCngbDwVmZIBy+5SSF5GT9me0M2tZilbZbCSIm\r\n"
        "xHfiDwhdBPw90c5ay4W+SKYOi4kJGaBPvCT0L9C+PMMJq6J/ONBhe5XA95z6CrBk\r\n"
        "h2HwByIkpLL2MFVaGT+P7zm63oZuz5R5p7+rUhZXvGI7d/CZi740GouX1WWrxQLB\r\n"
        "wfVI2x4UV4gTTlkHC2mAw8tAHkJyRGW7lzfA/1cLOXj6Ea1pcM/Zph9UUwIdSLKi\r\n"
        "N7nhpRQS0NPPzGojvMthj+M7DqjMVG9EbXo7aZMp/UJBklRNXNhehzoULKyl07cK\r\n"
        "ptZidg5col/36Kktl0u+m3/+/tI+FhGjkvBdlTgaKQKBgQDSKz4/gNafQ365kd59\r\n"
        "ERGgLpHc0JxtC68M99b2K/nGecOSu3lpEWqTA+Big7M+uLmC1oi8BVXAZ0eEN+BP\r\n"
        "M4FThOKgNEJR6sIeKW9QERekryRQ8jWGqr16ZWF/RtqmXUBmVOrp6oU9hDWzaJOl\r\n"
        "4BIndeFAdY0SqzZsyqxdeWkZ6wKBgQDciFL4T4ZVAL/gynuM0MzJ/gmfdmTOIVf3\r\n"
        "PG0IOHrTt/93qj/1PNjw8YPkWI/KjekjgSVW755vbi+v4RjpTaETFV5yy1Co0x+Y\r\n"
        "OMgu6t4FPQuAWofQP70P77PSuNX3PvlQ+pR4bHnFVGnZjABJePEVgfnVMfdYk+pZ\r\n"
        "qjl9O7MgFQKBgQCJAq/d7jv8L6AZkcKdfdcjubhVX5nabNd9eMSmTxzSCuc4bQdF\r\n"
        "O5liphBNqNXdecdWSE1zZXb1q16wD00n1Mfmjqa/EdJ6XLSXvSNX6wyiy5q9Omky\r\n"
        "lXRYqmbY9nPsKLam/4z1WXHrHJl1kCNQ67ZqCysBPqDtHrioS6f6d//QaQKBgQCd\r\n"
        "kCj3kJoBkY+GJCUx7tziVMa7T/Ol8tzOoDWmcfQP4J59+P3JUbgOLgB13XCe39V6\r\n"
        "sILlIj6ojqkzmDgcOIKlB9d7XRg8BtvD/o3IN7Z5N6xIAGaYu3FxOWnZXIZDKJMo\r\n"
        "8F+zyo0WF8Go1QY/eFGHdF3znMeEWepONfjtKQJKIQKBgQCHqiuijgunrXHzpZQf\r\n"
        "S6osFMvOUAzrLRC4h+ckJJ0U2HgXp0GFSRkCcu+7tD9ckv+LS623bKzeVE0Spyy9\r\n"
        "QJKwvnPkW2RSCrhRItyDzrMlvFiuHKF9W4OwXcWP4D9Z5qVUlazI11elV71CwFii\r\n"
        "LUf0oe5FYoorQ29bEex+IdD/Sg==\r\n"
        "-----END PRIVATE KEY-----\r\n";
    config.prvtkey_pem = prvtkey_pem;
    config.prvtkey_len = sizeof(prvtkey_pem) / sizeof(prvtkey_pem[0]);

    ESP_LOGI("********", "PRIV KEY LENGTH %d", config.prvtkey_len);

    // Start the httpd ssl server
    if (httpd_ssl_start(&server_, &config) == ESP_OK)
    {
        ESP_LOGI(TAG_.data(), "Registering URI handlers");
        httpd_register_uri_handler(server_, &homepage);
        httpd_register_uri_handler(server_, &ssid_pswd);
        return true;
    }

    ESP_LOGI(TAG_.data(), "Error starting server!");
    return false;
}

bool HttpsServer::stopWebServer()
{
    return httpd_ssl_stop(server_) == ESP_OK ? true : false;
}

std::string HttpsServer::getWifiSsid()
{
    return ESP_WIFI_SSID_STA;
}

std::string HttpsServer::getWifiPswd()
{
    return ESP_WIFI_PASS_STA;
}

bool HttpsServer::clearWifiSsidPswd()
{
    ESP_WIFI_SSID_STA.clear();
    ESP_WIFI_PASS_STA.clear();
    return true;
}

/* An HTTP POST handler */
esp_err_t HttpsServer::ssid_pswd_post_handler(httpd_req_t* req)
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
esp_err_t HttpsServer::http_get_handler(httpd_req_t* req)
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