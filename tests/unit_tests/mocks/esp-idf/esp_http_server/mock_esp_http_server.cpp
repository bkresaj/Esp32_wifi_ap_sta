#include "mock_esp_http_server.h"

esp_err_t httpd_stop(httpd_handle_t handle)
{
    return MockEspHttpServer::getInstance().httpd_stop(handle);
}

int httpd_req_recv(httpd_req_t *r, char *buf, size_t buf_len)
{
    return MockEspHttpServer::getInstance().httpd_req_recv(r, buf, buf_len);
}

esp_err_t httpd_resp_send_chunk(httpd_req_t *r, const char *buf,
                                ssize_t buf_len)
{
    return MockEspHttpServer::getInstance().httpd_resp_send_chunk(r, buf,
                                                                  buf_len);
}

esp_err_t httpd_resp_send(httpd_req_t *r, const char *buf, ssize_t buf_len)
{
    return MockEspHttpServer::getInstance().httpd_resp_send(r, buf, buf_len);
}

size_t httpd_req_get_hdr_value_len(httpd_req_t *r, const char *field)
{
    return MockEspHttpServer::getInstance().httpd_req_get_hdr_value_len(r,
                                                                        field);
}

esp_err_t httpd_start(httpd_handle_t *handle, const httpd_config_t *config)
{
    return MockEspHttpServer::getInstance().httpd_start(handle, config);
}

esp_err_t httpd_register_uri_handler(httpd_handle_t handle,
                                     const httpd_uri_t *uri_handler)
{
    return MockEspHttpServer::getInstance().httpd_register_uri_handler(
        handle, uri_handler);
}
