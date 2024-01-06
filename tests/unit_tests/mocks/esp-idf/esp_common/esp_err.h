#pragma once

typedef int esp_err_t;

#define ESP_OK 0
#define ESP_FAIL -1

#define ESP_ERR_NOT_FOUND 0x105 /*!< Requested resource not found */

#define ESP_ERROR_CHECK(x)        \
    do                            \
    {                             \
        esp_err_t __err_rc = (x); \
        if (__err_rc != ESP_OK)   \
        {                         \
        }                         \
    } while (0)

inline const char *esp_err_to_name(esp_err_t code)
{
    return "b";
}
