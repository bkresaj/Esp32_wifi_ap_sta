#pragma once

#include <singleton_template.h>

#include "esp_https_server.h"

class MockEspHttpsServer : public SingletonTemplate<MockEspHttpsServer>
{
   public:
    MOCK_METHOD(esp_err_t, httpd_ssl_start,
                (httpd_handle_t * handle, httpd_ssl_config_t *config), ());
    MOCK_METHOD(esp_err_t, httpd_ssl_stop, (httpd_handle_t handle), ());
};
