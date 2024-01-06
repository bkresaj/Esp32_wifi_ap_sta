#pragma once

#include <gmock/gmock.h>
#include <singleton_template.h>

#include "esp_spiffs.h"

class MockEspSpiffs : public SingletonTemplate<MockEspSpiffs>
{
   public:
    MOCK_METHOD(esp_err_t, esp_vfs_spiffs_register,
                (const esp_vfs_spiffs_conf_t *conf), ());
};