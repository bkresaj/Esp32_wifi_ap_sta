#include "mock_esp_spiffs.h"

esp_err_t esp_vfs_spiffs_register(const esp_vfs_spiffs_conf_t *conf)
{
    return MockEspSpiffs::getInstance().esp_vfs_spiffs_register(conf);
}