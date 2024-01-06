#pragma once

#include "esp_err.h"
#include "esp_event_base.h"
#include "esp_wifi_default.h"
#include "esp_wifi_types.h"
#include "wifi_os_adapter.h"

typedef int wpa_crypto_funcs_t;

typedef struct
{
    wifi_osi_funcs_t *osi_funcs; /**< WiFi OS functions */
    wpa_crypto_funcs_t
        wpa_crypto_funcs;   /**< WiFi station crypto functions when connect */
    int static_rx_buf_num;  /**< WiFi static RX buffer number */
    int dynamic_rx_buf_num; /**< WiFi dynamic RX buffer number */
    int tx_buf_type;        /**< WiFi TX buffer type */
    int static_tx_buf_num;  /**< WiFi static TX buffer number */
    int dynamic_tx_buf_num; /**< WiFi dynamic TX buffer number */
    int cache_tx_buf_num;   /**< WiFi TX cache buffer number */
    int csi_enable;         /**< WiFi channel state information enable flag */
    int ampdu_rx_enable;    /**< WiFi AMPDU RX feature enable flag */
    int ampdu_tx_enable;    /**< WiFi AMPDU TX feature enable flag */
    int amsdu_tx_enable;    /**< WiFi AMSDU TX feature enable flag */
    int nvs_enable;         /**< WiFi NVS flash enable flag */
    int nano_enable; /**< Nano option for printf/scan family enable flag */
    int rx_ba_win;   /**< WiFi Block Ack RX window size */
    int wifi_task_core_id; /**< WiFi Task Core ID */
    int beacon_max_len;    /**< WiFi softAP maximum length of the beacon */
    int mgmt_sbuf_num;     /**< WiFi management short buffer number, the minimum
                              value is 6, the maximum value is 32 */
    uint64_t
        feature_caps; /**< Enables additional WiFi features and capabilities */
    bool sta_disconnected_pm;   /**< WiFi Power Management for station at
                                   disconnected status */
    int espnow_max_encrypt_num; /**< Maximum encrypt number of peers supported
                                   by espnow */
    int magic; /**< WiFi init magic number, it should be the last field */
} wifi_init_config_t;

#define BIT1 0x00000002
#define BIT0 0x00000001

#define WIFI_INIT_CONFIG_DEFAULT() {};

esp_err_t esp_wifi_set_mode(wifi_mode_t mode);
esp_err_t esp_wifi_set_config(wifi_interface_t interface, wifi_config_t *conf);
esp_err_t esp_wifi_start();
esp_err_t esp_wifi_stop(void);
esp_err_t esp_wifi_disconnect(void);
esp_err_t esp_wifi_init(const wifi_init_config_t *config);
esp_err_t esp_wifi_connect(void);
