#pragma once

#include <gmock/gmock.h>

#include <cstddef>

#include "esp_err.h"
#include "mock_esp_netif.h"
#include "mock_freertos.h"

#define HTTPD_MAX_URI_LEN 100
#define HTTPD_SOCK_ERR_FAIL -1
#define HTTPD_SOCK_ERR_INVALID -2
#define HTTPD_SOCK_ERR_TIMEOUT -3
#define HTTPD_RESP_USE_STRLEN -1

#define HTTPD_DEFAULT_CONFIG() {};

typedef void *httpd_handle_t;
typedef void (*httpd_free_ctx_fn_t)(void *ctx);

typedef struct httpd_req
{
    httpd_handle_t handle;
    int method;
    const char uri[HTTPD_MAX_URI_LEN + 1];
    size_t content_len;
    void *aux;
    void *user_ctx;
    void *sess_ctx;
    httpd_free_ctx_fn_t free_ctx;
    bool ignore_sess_ctx_changes;
} httpd_req_t;

enum http_method
{
    YES,
    NO
};
typedef enum http_method httpd_method_t;

#define HTTP_POST (httpd_method_t)1
#define HTTP_GET (httpd_method_t)1

typedef struct httpd_uri
{
    const char *uri;       /*!< The URI to handle */
    httpd_method_t method; /*!< Method supported by the URI */

    /**
     * Handler to call for supported request method. This must
     * return ESP_OK, or else the underlying socket will be closed.
     */
    esp_err_t (*handler)(httpd_req_t *r);

    /**
     * Pointer to user context data which will be available to handler
     */
    void *user_ctx;

#ifdef CONFIG_HTTPD_WS_SUPPORT
    /**
     * Flag for indicating a WebSocket endpoint.
     * If this flag is true, then method must be HTTP_GET. Otherwise the
     * handshake will not be handled.
     */
    bool is_websocket;

    /**
     * Flag indicating that control frames (PING, PONG, CLOSE) are also passed
     * to the handler This is used if a custom processing of the control frames
     * is needed
     */
    bool handle_ws_control_frames;

    /**
     * Pointer to subprotocol supported by URI
     */
    const char *supported_subprotocol;
#endif
} httpd_uri_t;

typedef void (*httpd_free_ctx_fn_t)(void *ctx);
typedef esp_err_t (*httpd_open_func_t)(httpd_handle_t hd, int sockfd);
typedef void (*httpd_close_func_t)(httpd_handle_t hd, int sockfd);
typedef bool (*httpd_uri_match_func_t)(const char *reference_uri,
                                       const char *uri_to_match,
                                       size_t match_upto);

typedef struct httpd_config
{
    unsigned
        task_priority; /*!< Priority of FreeRTOS task which runs the server */
    size_t
        stack_size; /*!< The maximum stack size allowed for the server task */
    BaseType_t core_id; /*!< The core the HTTP server task will run on */

    /**
     * TCP Port number for receiving and transmitting HTTP traffic
     */
    uint16_t server_port;

    /**
     * UDP Port number for asynchronously exchanging control signals
     * between various components of the server
     */
    uint16_t ctrl_port;

    uint16_t max_open_sockets;  /*!< Max number of sockets/clients connected at
                                   any time (3 sockets are reserved for internal
                                   working of the HTTP server) */
    uint16_t max_uri_handlers;  /*!< Maximum allowed uri handlers */
    uint16_t max_resp_headers;  /*!< Maximum allowed additional headers in HTTP
                                   response */
    uint16_t backlog_conn;      /*!< Number of backlog connections */
    bool lru_purge_enable;      /*!< Purge "Least Recently Used" connection */
    uint16_t recv_wait_timeout; /*!< Timeout for recv function (in seconds)*/
    uint16_t send_wait_timeout; /*!< Timeout for send function (in seconds)*/

    /**
     * Global user context.
     *
     * This field can be used to store arbitrary user data within the server
     * context. The value can be retrieved using the server handle, available
     * e.g. in the httpd_req_t struct.
     *
     * When shutting down, the server frees up the user context by
     * calling free() on the global_user_ctx field. If you wish to use a custom
     * function for freeing the global user context, please specify that here.
     */
    void *global_user_ctx;

    /**
     * Free function for global user context
     */
    httpd_free_ctx_fn_t global_user_ctx_free_fn;

    /**
     * Global transport context.
     *
     * Similar to global_user_ctx, but used for session encoding or encryption
     * (e.g. to hold the SSL context). It will be freed using free(), unless
     * global_transport_ctx_free_fn is specified.
     */
    void *global_transport_ctx;

    /**
     * Free function for global transport context
     */
    httpd_free_ctx_fn_t global_transport_ctx_free_fn;

    bool enable_so_linger;   /*!< bool to enable/disable linger */
    int linger_timeout;      /*!< linger timeout (in seconds) */
    bool keep_alive_enable;  /*!< Enable keep-alive timeout */
    int keep_alive_idle;     /*!< Keep-alive idle time. Default is 5 (second) */
    int keep_alive_interval; /*!< Keep-alive interval time. Default is 5
                                (second) */
    int keep_alive_count; /*!< Keep-alive packet retry send count. Default is 3
                             counts */
    /**
     * Custom session opening callback.
     *
     * Called on a new session socket just after accept(), but before reading
     * any data.
     *
     * This is an opportunity to set up e.g. SSL encryption using
     * global_transport_ctx and the send/recv/pending session overrides.
     *
     * If a context needs to be maintained between these functions, store it in
     * the session using httpd_sess_set_transport_ctx() and retrieve it later
     * with httpd_sess_get_transport_ctx()
     *
     * Returning a value other than ESP_OK will immediately close the new
     * socket.
     */
    httpd_open_func_t open_fn;

    /**
     * Custom session closing callback.
     *
     * Called when a session is deleted, before freeing user and transport
     * contexts and before closing the socket. This is a place for custom
     * de-init code common to all sockets.
     *
     * The server will only close the socket if no custom session closing
     * callback is set. If a custom callback is used, `close(sockfd)` should be
     * called in here for most cases.
     *
     * Set the user or transport context to NULL if it was freed here, so the
     * server does not try to free it again.
     *
     * This function is run for all terminated sessions, including sessions
     * where the socket was closed by the network stack - that is, the file
     * descriptor may not be valid anymore.
     */
    httpd_close_func_t close_fn;

    /**
     * URI matcher function.
     *
     * Called when searching for a matching URI:
     *     1) whose request handler is to be executed right
     *        after an HTTP request is successfully parsed
     *     2) in order to prevent duplication while registering
     *        a new URI handler using `httpd_register_uri_handler()`
     *
     * Available options are:
     *     1) NULL : Internally do basic matching using `strncmp()`
     *     2) `httpd_uri_match_wildcard()` : URI wildcard matcher
     *
     * Users can implement their own matching functions (See description
     * of the `httpd_uri_match_func_t` function prototype)
     */
    httpd_uri_match_func_t uri_match_fn;
} httpd_config_t;

esp_err_t httpd_stop(httpd_handle_t handle);
int httpd_req_recv(httpd_req_t *r, char *buf, size_t buf_len);
esp_err_t httpd_resp_send_chunk(httpd_req_t *r, const char *buf,
                                ssize_t buf_len);
esp_err_t httpd_resp_send(httpd_req_t *r, const char *buf, ssize_t buf_len);
size_t httpd_req_get_hdr_value_len(httpd_req_t *r, const char *field);
esp_err_t httpd_start(httpd_handle_t *handle, const httpd_config_t *config);
esp_err_t httpd_register_uri_handler(httpd_handle_t handle,
                                     const httpd_uri_t *uri_handler);
