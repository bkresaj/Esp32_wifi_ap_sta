#include "spiffs.h"

#include <esp_log.h>
#include <esp_spiffs.h>

#include <cassert>
#include <memory>

Spiffs::Spiffs()
{
    ESP_LOGI(TAG_.data(), "Initializing SPIFFS");

    const size_t NUMBER_OF_MAX_FILES{5};

    esp_vfs_spiffs_conf_t conf = {.base_path = "/spiffs",
                                  .partition_label = NULL,
                                  .max_files = NUMBER_OF_MAX_FILES,
                                  .format_if_mount_failed = true};

    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(TAG_.data(), "Failed to mount or format filesystem");
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            ESP_LOGE(TAG_.data(), "Failed to find SPIFFS partition");
        }
        else
        {
            ESP_LOGE(TAG_.data(), "Failed to initialize SPIFFS (%s)",
                     esp_err_to_name(ret));
        }
        assert(false);
    }

    mutex_ = xSemaphoreCreateMutex();

    if (mutex_ == nullptr)
    {
        ESP_LOGE(TAG_.data(), "Failed to initialize mutex");
        assert(false);
    }

    if (!init())
    {
        ESP_LOGE(TAG_.data(), "Failed to initialize spiffs");
        assert(false);
    }
}

bool Spiffs::writeToFile(const std::string& filename, const std::string& data)
{
    if (xSemaphoreTake(mutex_, pdMS_TO_TICKS(MUTEX_TIMEOUT_)) == pdTRUE)
    {
        auto file = std::unique_ptr<FILE, decltype(&fileCustomDeleter)>(
            std::fopen(filename.c_str(), "w"), fileCustomDeleter);

        size_t bytes_written{0};

        if (file == nullptr)
        {
            ESP_LOGE(TAG_.data(), "writeToFile: Fopen failed");
            // cppcheck-suppress cstyleCast
            xSemaphoreGive(mutex_);
            return false;
        }

        bytes_written = std::fprintf(file.get(), "%s", data.c_str());
        ESP_LOGI(TAG_.data(), "Bytes written: %d", bytes_written);

        if (bytes_written == data.size())
        {
            ESP_LOGI(TAG_.data(), "File written successfully");
            return true;
        }
        else
        {
            ESP_LOGE(TAG_.data(), "File write failed");
            return false;
        }
    }
    else
    {
        ESP_LOGE(TAG_.data(), "File write failed because mutex is locked");
        return false;
    }
}

bool Spiffs::readFile(const std::string& filename, std::string& data)
{
    if (xSemaphoreTake(mutex_, pdMS_TO_TICKS(MUTEX_TIMEOUT_)) == pdTRUE)
    {
        auto file = std::unique_ptr<FILE, decltype(&fileCustomDeleter)>(
            std::fopen(filename.c_str(), "r"), fileCustomDeleter);

        if (file == nullptr)
        {
            ESP_LOGE(TAG_.data(), "readFile: Fopen failed");
            // cppcheck-suppress cstyleCast
            xSemaphoreGive(mutex_);
            return false;
        }

        size_t bytesRead =
            std::fread(data.data(), sizeof(char), data.size(), file.get());

        if (bytesRead == 0)
        {
            return false;
        }

        data += '\0';  // Add null terminator at the end of the string

        return true;
    }
    else
    {
        ESP_LOGE(TAG_.data(), "File read failed because mutex is locked");
        return false;
    }
}

bool Spiffs::init()
{
    std::string html_file =
        "<html> \
  <head> \
    <title>Test Led http protocol</title> \
  </head> \
  <body> \
    <center> \
      <h1 style=\"font-size: 2.5em; margin-top: 2em;\"><b>ESP32 Web Control</b></h1> \
    </center> \
    <center> \
      <p style=\"font-size: 1.5em; margin-top: 2em;\"><b>Welcome, enter SSID and password to connect to WiFi network :)</b></p> \
      <form> \
        <label for=\"ssid\">SSID and Password (separated by ','):</label> \
        <input type=\"text\" id=\"credentials\" name=\"credentials\"> \
        <input type=\"submit\" value=\"Connect\" style=\"margin-left: 1em;\" onclick=\"sendFormData(event)\"> \
      </form> \
    </center> \
    <script> \
      function sendFormData(event) { \
        event.preventDefault(); \
        const form = document.querySelector('form'); \
        const data = new FormData(form); \
        fetch('/', { \
          method: 'POST', \
          body: data \
        }) \
        .then(response => { \
          console.log('Response:', response); \
          clearTextbox(); \
        }) \
        .catch(error => { \
          console.error('Error:', error); \
        }); \
      } \
        \
      function clearTextbox() { \
        document.getElementById('credentials').value = ''; \
      } \
    </script> \
  </body> \
</html>";

    return writeToFile("/spiffs/html_file", html_file);
}

size_t Spiffs::getFileSize(const std::string& filename)
{
    if (xSemaphoreTake(mutex_, pdMS_TO_TICKS(MUTEX_TIMEOUT_)) == pdTRUE)
    {
        auto file = std::unique_ptr<FILE, decltype(&fileCustomDeleter)>(
            std::fopen(filename.c_str(), "r"), fileCustomDeleter);

        if (file == nullptr)
        {
            ESP_LOGE(TAG_.data(), "getFileSize: Fopen failed");
            // cppcheck-suppress cstyleCast
            xSemaphoreGive(mutex_);
            return false;
        }

        std::fseek(file.get(), 0, SEEK_END);
        size_t size = std::ftell(file.get());

        ESP_LOGI(TAG_.data(), "File size: %d", size);

        return size;
    }
    else
    {
        ESP_LOGE(TAG_.data(), "Get file size failed because mutex is locked");
        return false;
    }
}
