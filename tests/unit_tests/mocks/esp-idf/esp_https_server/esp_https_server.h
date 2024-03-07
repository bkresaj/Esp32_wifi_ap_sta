#pragma once

#include "esp_err.h"
#include "esp_tls.h"
#include "mock_esp_http_server.h"

#define HTTPD_SSL_CONFIG_DEFAULT() {};

typedef struct esp_tls esp_tls_t;

typedef enum
{
    HTTPD_SSL_TRANSPORT_SECURE,   // SSL Enabled
    HTTPD_SSL_TRANSPORT_INSECURE  // SSL disabled
} httpd_ssl_transport_mode_t;

/**
 * @brief Indicates the state at which the user callback is executed,
 *        i.e at session creation or session close
 */
typedef enum
{
    HTTPD_SSL_USER_CB_SESS_CREATE,
    HTTPD_SSL_USER_CB_SESS_CLOSE
} httpd_ssl_user_cb_state_t;

/**
 * @brief Callback data struct, contains the ESP-TLS connection handle
 * and the connection state at which the callback is executed
 */
typedef struct esp_https_server_user_cb_arg
{
    httpd_ssl_user_cb_state_t user_cb_state; /*!< State of user callback */
    esp_tls_t *tls;                          /*!< ESP-TLS connection handle */
} esp_https_server_user_cb_arg_t;

/**
 * @brief Callback function prototype
 * Can be used to get connection or client information (SSL context)
 * E.g. Client certificate, Socket FD, Connection state, etc.
 *
 * @param user_cb Callback data struct
 */
typedef void esp_https_server_user_cb(esp_https_server_user_cb_arg_t *user_cb);

/**
 * HTTPS server config struct
 *
 * Please use HTTPD_SSL_CONFIG_DEFAULT() to initialize it.
 */
struct httpd_ssl_config
{
    /**
     * Underlying HTTPD server config
     *
     * Parameters like task stack size and priority can be adjusted here.
     */
    httpd_config_t httpd;

    /** Server certificate */
    const uint8_t *servercert;

    /** Server certificate byte length */
    size_t servercert_len;

    /** CA certificate ((CA used to sign clients, or client cert itself) */
    const uint8_t *cacert_pem;

    /** CA certificate byte length */
    size_t cacert_len;

    /** Private key */
    const uint8_t *prvtkey_pem;

    /** Private key byte length */
    size_t prvtkey_len;

    /** Transport Mode (default secure) */
    httpd_ssl_transport_mode_t transport_mode;

    /** Port used when transport mode is secure (default 443) */
    uint16_t port_secure;

    /** Port used when transport mode is insecure (default 80) */
    uint16_t port_insecure;

    /** Enable tls session tickets */
    bool session_tickets;

    /** Enable secure element for server session */
    bool use_secure_element;

    /** User callback for esp_https_server */
    esp_https_server_user_cb *user_cb;

    void *ssl_userdata; /*!< user data to add to the ssl context  */
    esp_tls_handshake_callback
        cert_select_cb; /*!< Certificate selection callback to use */
};

typedef struct httpd_ssl_config httpd_ssl_config_t;

/**
 * Create a SSL capable HTTP server (secure mode may be disabled in config)
 *
 * @param[in,out] config - server config, must not be const. Does not have to
 * stay valid after calling this function.
 * @param[out] handle - storage for the server handle, must be a valid pointer
 * @return success
 */
esp_err_t httpd_ssl_start(httpd_handle_t *handle, httpd_ssl_config_t *config);

/**
 * Stop the server. Blocks until the server is shut down.
 *
 * @param[in] handle
 * @return
 *    - ESP_OK: Server stopped successfully
 *    - ESP_ERR_INVALID_ARG: Invalid argument
 *    - ESP_FAIL: Failure to shut down server
 */
esp_err_t httpd_ssl_stop(httpd_handle_t handle);