#include "esp_log.h"
#include "nvs_flash.h"
#include "spiffs.h"
#include "wifi_manager.h"

/**
 * Main function for starting application.
 *
 * @return None.
 */
extern "C" void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    WifiManager::getInstance().dispatch(Start());

    while (1)
    {
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}
