#pragma once

#include <cstdio>

#define ESP_LOGE(tag, format, ...) 0 && std::printf(format, ##__VA_ARGS__)
#define ESP_LOGW(tag, format, ...) 0 && std::printf(format, ##__VA_ARGS__)
#define ESP_LOGI(tag, format, ...) 0 && std::printf(format, ##__VA_ARGS__)
#define ESP_LOGD(tag, format, ...) 0 && std::printf(format, ##__VA_ARGS__)
#define ESP_LOGV(tag, format, ...) 0 && std::printf(format, ##__VA_ARGS__)