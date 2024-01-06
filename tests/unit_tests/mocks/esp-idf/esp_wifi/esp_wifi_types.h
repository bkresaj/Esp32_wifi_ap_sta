#pragma once

#include "esp_event_base.h"
#include "esp_interface.h"

typedef struct
{
    uint8_t mac[6]; /**< MAC address of the station connected to Soft-AP */
    uint8_t aid; /**< the aid that soft-AP gives to the station connected to  */
    bool is_mesh_child; /**< flag to identify mesh child */
} wifi_event_ap_staconnected_t;

typedef struct
{
    uint8_t mac[6]; /**< MAC address of the station disconnects to soft-AP */
    uint8_t aid; /**< the aid that soft-AP gave to the station disconnects to */
    bool is_mesh_child; /**< flag to identify mesh child */
} wifi_event_ap_stadisconnected_t;

typedef enum
{
    WIFI_IF_STA = ESP_IF_WIFI_STA,
    WIFI_IF_AP = ESP_IF_WIFI_AP,
#if defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2)
    WIFI_IF_NAN = ESP_IF_WIFI_NAN,
#endif
    WIFI_IF_MAX
} wifi_interface_t;

typedef enum
{
    WIFI_MODE_NULL = 0, /**< null mode */
    WIFI_MODE_STA,      /**< WiFi station mode */
    WIFI_MODE_AP,       /**< WiFi soft-AP mode */
    WIFI_MODE_APSTA,    /**< WiFi station + soft-AP mode */
    WIFI_MODE_NAN,      /**< WiFi NAN mode */
    WIFI_MODE_MAX
} wifi_mode_t;

typedef enum
{
    WIFI_AUTH_OPEN = 0,        /**< authenticate mode : open */
    WIFI_AUTH_WEP,             /**< authenticate mode : WEP */
    WIFI_AUTH_WPA_PSK,         /**< authenticate mode : WPA_PSK */
    WIFI_AUTH_WPA2_PSK,        /**< authenticate mode : WPA2_PSK */
    WIFI_AUTH_WPA_WPA2_PSK,    /**< authenticate mode : WPA_WPA2_PSK */
    WIFI_AUTH_WPA2_ENTERPRISE, /**< authenticate mode : WPA2_ENTERPRISE */
    WIFI_AUTH_WPA3_PSK,        /**< authenticate mode : WPA3_PSK */
    WIFI_AUTH_WPA2_WPA3_PSK,   /**< authenticate mode : WPA2_WPA3_PSK */
    WIFI_AUTH_WAPI_PSK,        /**< authenticate mode : WAPI_PSK */
    WIFI_AUTH_OWE,             /**< authenticate mode : OWE */
    WIFI_AUTH_MAX
} wifi_auth_mode_t;

typedef enum
{
    WIFI_CIPHER_TYPE_NONE = 0,    /**< the cipher type is none */
    WIFI_CIPHER_TYPE_WEP40,       /**< the cipher type is WEP40 */
    WIFI_CIPHER_TYPE_WEP104,      /**< the cipher type is WEP104 */
    WIFI_CIPHER_TYPE_TKIP,        /**< the cipher type is TKIP */
    WIFI_CIPHER_TYPE_CCMP,        /**< the cipher type is CCMP */
    WIFI_CIPHER_TYPE_TKIP_CCMP,   /**< the cipher type is TKIP and CCMP */
    WIFI_CIPHER_TYPE_AES_CMAC128, /**< the cipher type is AES-CMAC-128 */
    WIFI_CIPHER_TYPE_SMS4,        /**< the cipher type is SMS4 */
    WIFI_CIPHER_TYPE_GCMP,        /**< the cipher type is GCMP */
    WIFI_CIPHER_TYPE_GCMP256,     /**< the cipher type is GCMP-256 */
    WIFI_CIPHER_TYPE_AES_GMAC128, /**< the cipher type is AES-GMAC-128 */
    WIFI_CIPHER_TYPE_AES_GMAC256, /**< the cipher type is AES-GMAC-256 */
    WIFI_CIPHER_TYPE_UNKNOWN,     /**< the cipher type is unknown */
} wifi_cipher_type_t;

typedef struct
{
    bool capable;  /**< Deprecated variable. Device will always connect in PMF
                      mode if other device also advertizes PMF capability. */
    bool required; /**< Advertizes that Protected Management Frame is required.
                      Device will not associate to non-PMF capable devices. */
} wifi_pmf_config_t;

typedef enum
{
    WPA3_SAE_PWE_UNSPECIFIED,
    WPA3_SAE_PWE_HUNT_AND_PECK,
    WPA3_SAE_PWE_HASH_TO_ELEMENT,
    WPA3_SAE_PWE_BOTH,
} wifi_sae_pwe_method_t;

typedef struct
{
    uint8_t ssid[32]; /**< SSID of soft-AP. If ssid_len field is 0, this must be
                         a Null terminated string. Otherwise, length is set
                         according to ssid_len. */
    uint8_t password[64]; /**< Password of soft-AP. */
    uint8_t ssid_len;     /**< Optional length of SSID field. */
    uint8_t channel;      /**< Channel of soft-AP */
    wifi_auth_mode_t
        authmode;           /**< Auth mode of soft-AP. Do not support AUTH_WEP,
                               AUTH_WAPI_PSK and AUTH_OWE in soft-AP mode. When the auth
                               mode is set to WPA2_PSK, WPA2_WPA3_PSK or WPA3_PSK, the
                               pairwise cipher will be overwritten with
                               WIFI_CIPHER_TYPE_CCMP.  */
    uint8_t ssid_hidden;    /**< Broadcast SSID or not, default 0, broadcast the
                               SSID */
    uint8_t max_connection; /**< Max number of stations allowed to connect in */
    uint16_t beacon_interval; /**< Beacon interval which should be multiples of
                                 100. Unit: TU(time unit, 1 TU = 1024 us).
                                 Range: 100 ~ 60000. Default value: 100 */
    wifi_cipher_type_t
        pairwise_cipher; /**< Pairwise cipher of SoftAP, group cipher will be
                            derived using this. Cipher values are valid starting
                            from WIFI_CIPHER_TYPE_TKIP, enum values before that
                            will be considered as invalid and default cipher
                            suites(TKIP+CCMP) will be used. Valid cipher suites
                            in softAP mode are WIFI_CIPHER_TYPE_TKIP,
                            WIFI_CIPHER_TYPE_CCMP and
                            WIFI_CIPHER_TYPE_TKIP_CCMP. */
    bool ftm_responder;  /**< Enable FTM Responder mode */
    wifi_pmf_config_t
        pmf_cfg; /**< Configuration for Protected Management Frame */
    wifi_sae_pwe_method_t
        sae_pwe_h2e; /**< Configuration for SAE PWE derivation method */
} wifi_ap_config_t;

typedef enum
{
    WIFI_FAST_SCAN =
        0, /**< Do fast scan, scan will end after find SSID match AP */
    WIFI_ALL_CHANNEL_SCAN, /**< All channel scan, scan will end after scan all
                              the channel */
} wifi_scan_method_t;

typedef enum
{
    WIFI_CONNECT_AP_BY_SIGNAL = 0, /**< Sort match AP in scan list by RSSI */
    WIFI_CONNECT_AP_BY_SECURITY,   /**< Sort match AP in scan list by security
                                      mode */
} wifi_sort_method_t;

/** @brief Structure describing parameters for a WiFi fast scan */
typedef struct
{
    int8_t rssi; /**< The minimum rssi to accept in the fast scan mode */
    wifi_auth_mode_t
        authmode; /**< The weakest authmode to accept in the fast scan mode
                       Note: Incase this value is not set and password is set as
                     per WPA2 standards(password len >= 8), it will be defaulted
                     to WPA2 and device won't connect to deprecated WEP/WPA
                     networks. Please set authmode threshold as
                     WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK to connect to WEP/WPA
                     networks */
} wifi_scan_threshold_t;

/** Configuration for SAE-PK  */
typedef enum
{
    WPA3_SAE_PK_MODE_AUTOMATIC = 0,
    WPA3_SAE_PK_MODE_ONLY = 1,
    WPA3_SAE_PK_MODE_DISABLED = 2,
} wifi_sae_pk_mode_t;

#define SAE_H2E_IDENTIFIER_LEN 32
/** @brief STA configuration settings for the device */
typedef struct
{
    uint8_t ssid[32];               /**< SSID of target AP. */
    uint8_t password[64];           /**< Password of target AP. */
    wifi_scan_method_t scan_method; /**< do all channel scan or fast scan */
    bool bssid_set; /**< whether set MAC address of target AP or not. Generally,
                       station_config.bssid_set needs to be 0; and it needs to
                       be 1 only when users need to check the MAC address of the
                       AP.*/
    uint8_t bssid[6]; /**< MAC address of target AP*/
    uint8_t channel;  /**< channel of target AP. Set to 1~13 to scan starting
                         from the specified channel before connecting to AP. If
                         the channel of AP is unknown, set it to 0.*/
    uint16_t listen_interval; /**< Listen interval for ESP32 station to receive
                                 beacon when WIFI_PS_MAX_MODEM is set. Units: AP
                                 beacon intervals. Defaults to 3 if set to 0. */
    wifi_sort_method_t sort_method; /**< sort the connect AP in the list by rssi
                                       or security mode */
    wifi_scan_threshold_t
        threshold; /**< When sort_method is set, only APs which have an auth
                      mode that is more secure than the selected auth mode and a
                      signal stronger than the minimum RSSI will be used. */
    wifi_pmf_config_t
        pmf_cfg; /**< Configuration for Protected Management Frame. Will be
                    advertised in RSN Capabilities in RSN IE. */
    uint32_t rm_enabled : 1; /**< Whether Radio Measurements are enabled for the
                                connection */
    uint32_t btm_enabled : 1; /**< Whether BSS Transition Management is enabled
                                 for the connection */
    uint32_t mbo_enabled : 1; /**< Whether MBO is enabled for the connection */
    uint32_t ft_enabled : 1;  /**< Whether FT is enabled for the connection */
    uint32_t owe_enabled : 1; /**< Whether OWE is enabled for the connection */
    uint32_t transition_disable : 1; /**< Whether to enable transition disable
                                        feature */
    uint32_t reserved : 26;          /**< Reserved for future feature set */
    wifi_sae_pwe_method_t
        sae_pwe_h2e; /**< Configuration for SAE PWE derivation method */
    wifi_sae_pk_mode_t sae_pk_mode; /**< Configuration for SAE-PK (Public Key)
                                       Authentication method */
    uint8_t failure_retry_cnt; /**< Number of connection retries station will do
                                  before moving to next AP. scan_method should
                                  be set as WIFI_ALL_CHANNEL_SCAN to use this
                                  config. Note: Enabling this may cause
                                  connection time to increase incase best AP
                                  doesn't behave properly. */
    uint32_t he_dcm_set : 1; /**< Whether DCM max.constellation for transmission
                                and reception is set. */
    uint32_t
        he_dcm_max_constellation_tx : 2; /**< Indicate the max.constellation for
                                            DCM in TB PPDU the STA supported. 0:
                                            not supported. 1: BPSK, 2: QPSK, 3:
                                            16-QAM. The default value is 3. */
    uint32_t
        he_dcm_max_constellation_rx : 2; /**< Indicate the max.constellation for
                                            DCM in both Data field and HE-SIG-B
                                            field the STA supported. 0: not
                                            supported. 1: BPSK, 2: QPSK, 3:
                                            16-QAM. The default value is 3. */
    uint32_t he_mcs9_enabled : 1; /**< Whether to support HE-MCS 0 to 9. The
                                     default value is 0. */
    uint32_t he_su_beamformee_disabled : 1; /**< Whether to disable support for
                                               operation as an SU beamformee. */
    uint32_t
        he_trig_su_bmforming_feedback_disabled : 1; /**< Whether to disable
                                                       support the transmission
                                                       of SU feedback in an HE
                                                       TB sounding sequence. */
    uint32_t
        he_trig_mu_bmforming_partial_feedback_disabled : 1; /**< Whether to
                                                               disable support
                                                               the transmission
                                                               of
                                                               partial-bandwidth
                                                               MU feedback in an
                                                               HE TB sounding
                                                               sequence. */
    uint32_t
        he_trig_cqi_feedback_disabled : 1; /**< Whether to disable support the
                                              transmission of CQI feedback in an
                                              HE TB sounding sequence. */
    uint32_t he_reserved : 22; /**< Reserved for future feature set */
    uint8_t
        sae_h2e_identifier[SAE_H2E_IDENTIFIER_LEN]; /**< Password identifier for
                                                       H2E. this needs to be
                                                       null terminated string */
} wifi_sta_config_t;

typedef struct
{
    uint8_t op_channel; /**< NAN Discovery operating channel */
    uint8_t
        master_pref;   /**< Device's preference value to serve as NAN Master */
    uint8_t scan_time; /**< Scan time in seconds while searching for a NAN
                          cluster */
    uint16_t
        warm_up_sec; /**< Warm up time before assuming NAN Anchor Master role */
} wifi_nan_config_t;

typedef union
{
    wifi_ap_config_t ap;   /**< configuration of AP */
    wifi_sta_config_t sta; /**< configuration of STA */
    wifi_nan_config_t nan; /**< configuration of NAN */
} wifi_config_t;

typedef enum
{
    WIFI_EVENT_WIFI_READY = 0,      /**< WiFi ready */
    WIFI_EVENT_SCAN_DONE,           /**< Finished scanning AP */
    WIFI_EVENT_STA_START,           /**< Station start */
    WIFI_EVENT_STA_STOP,            /**< Station stop */
    WIFI_EVENT_STA_CONNECTED,       /**< Station connected to AP */
    WIFI_EVENT_STA_DISCONNECTED,    /**< Station disconnected from AP */
    WIFI_EVENT_STA_AUTHMODE_CHANGE, /**< the auth mode of AP connected by
                                       device's station changed */

    WIFI_EVENT_STA_WPS_ER_SUCCESS, /**< Station wps succeeds in enrollee mode */
    WIFI_EVENT_STA_WPS_ER_FAILED,  /**< Station wps fails in enrollee mode */
    WIFI_EVENT_STA_WPS_ER_TIMEOUT, /**< Station wps timeout in enrollee mode */
    WIFI_EVENT_STA_WPS_ER_PIN,     /**< Station wps pin code in enrollee mode */
    WIFI_EVENT_STA_WPS_ER_PBC_OVERLAP, /**< Station wps overlap in enrollee mode
                                        */

    WIFI_EVENT_AP_START,           /**< Soft-AP start */
    WIFI_EVENT_AP_STOP,            /**< Soft-AP stop */
    WIFI_EVENT_AP_STACONNECTED,    /**< a station connected to Soft-AP */
    WIFI_EVENT_AP_STADISCONNECTED, /**< a station disconnected from Soft-AP */
    WIFI_EVENT_AP_PROBEREQRECVED,  /**< Receive probe request packet in soft-AP
                                      interface */

    WIFI_EVENT_FTM_REPORT, /**< Receive report of FTM procedure */

    /* Add next events after this only */
    WIFI_EVENT_STA_BSS_RSSI_LOW, /**< AP's RSSI crossed configured threshold */
    WIFI_EVENT_ACTION_TX_STATUS, /**< Status indication of Action Tx operation
                                  */
    WIFI_EVENT_ROC_DONE,         /**< Remain-on-Channel operation complete */

    WIFI_EVENT_STA_BEACON_TIMEOUT, /**< Station beacon timeout */

    WIFI_EVENT_CONNECTIONLESS_MODULE_WAKE_INTERVAL_START, /**< Connectionless
                                                             module wake
                                                             interval start */

    WIFI_EVENT_AP_WPS_RG_SUCCESS, /**< Soft-AP wps succeeds in registrar mode */
    WIFI_EVENT_AP_WPS_RG_FAILED,  /**< Soft-AP wps fails in registrar mode */
    WIFI_EVENT_AP_WPS_RG_TIMEOUT, /**< Soft-AP wps timeout in registrar mode */
    WIFI_EVENT_AP_WPS_RG_PIN,     /**< Soft-AP wps pin code in registrar mode */
    WIFI_EVENT_AP_WPS_RG_PBC_OVERLAP, /**< Soft-AP wps overlap in registrar mode
                                       */

    WIFI_EVENT_ITWT_SETUP,    /**< iTWT setup */
    WIFI_EVENT_ITWT_TEARDOWN, /**< iTWT teardown */
    WIFI_EVENT_ITWT_PROBE,    /**< iTWT probe */
    WIFI_EVENT_ITWT_SUSPEND,  /**< iTWT suspend */

    WIFI_EVENT_NAN_STARTED,    /**< NAN Discovery has started */
    WIFI_EVENT_NAN_STOPPED,    /**< NAN Discovery has stopped */
    WIFI_EVENT_NAN_SVC_MATCH,  /**< NAN Service Discovery match found */
    WIFI_EVENT_NAN_REPLIED,    /**< Replied to a NAN peer with Service Discovery
                                  match */
    WIFI_EVENT_NAN_RECEIVE,    /**< Received a Follow-up message */
    WIFI_EVENT_NDP_INDICATION, /**< Received NDP Request from a NAN Peer */
    WIFI_EVENT_NDP_CONFIRM,    /**< NDP Confirm Indication */
    WIFI_EVENT_NDP_TERMINATED, /**< NAN Datapath terminated indication */

    WIFI_EVENT_MAX, /**< Invalid WiFi event ID */
} wifi_event_t;

#define WIFI_EVENT (esp_event_base_t)1
#define IP_EVENT (esp_event_base_t)1
