#pragma once

#include <singleton_template.h>

#include "esp_http_server.h"

class MockEspHttpServer : public SingletonTemplate<MockEspHttpServer>
{
   public:
    MOCK_METHOD(esp_err_t, httpd_stop, (httpd_handle_t handle), ());
    MOCK_METHOD(int, httpd_req_recv,
                (httpd_req_t * r, char *buf, size_t buf_len), ());
    MOCK_METHOD(esp_err_t, httpd_resp_send_chunk,
                (httpd_req_t * r, const char *buf, ssize_t buf_len), ());
    MOCK_METHOD(esp_err_t, httpd_resp_send,
                (httpd_req_t * r, const char *buf, ssize_t buf_len), ());
    MOCK_METHOD(size_t, httpd_req_get_hdr_value_len,
                (httpd_req_t * r, const char *field), ());
    MOCK_METHOD(esp_err_t, httpd_start,
                (httpd_handle_t * handle, const httpd_config_t *config), ());
    MOCK_METHOD(esp_err_t, httpd_register_uri_handler,
                (httpd_handle_t handle, const httpd_uri_t *uri_handler), ());
};
