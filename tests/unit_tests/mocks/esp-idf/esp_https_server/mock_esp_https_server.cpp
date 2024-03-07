#include "mock_esp_https_server.h"

esp_err_t httpd_ssl_start(httpd_handle_t *handle, httpd_ssl_config_t *config)
{
    return MockEspHttpsServer::getInstance().httpd_ssl_start(handle, config);
}

esp_err_t httpd_ssl_stop(httpd_handle_t handle)
{
    return MockEspHttpsServer::getInstance().httpd_ssl_stop(handle);
}
