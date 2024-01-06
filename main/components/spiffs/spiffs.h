#ifndef SPIFFS_H
#define SPIFFS_H

#include <string>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

/**
 * @class Spiffs
 * @brief Singleton class for interacting with Spiffs(SPI flash file system) on
 * Esp32.
 */
class Spiffs
{
   public:
    Spiffs();
    ~Spiffs() = default;

    /**
     * Function for getting size of Spiffs file.
     *
     * @param filename Name of the file.
     * @return Size of file.
     */
    size_t getFileSize(const std::string& filename);

    /**
     * Function for reading Spiffs file.
     *
     * @param filename Name of the file.
     * @param data String where data from file will be copied.
     * @return Bool returning true on success or false on failure.
     */
    bool readFile(const std::string& filename, std::string& data);

   private:
    bool init();
    bool writeToFile(const std::string& filename, const std::string& data);
    static void fileCustomDeleter(FILE* ptr)
    {
        if (ptr)
        {
            std::fclose(ptr);
        }
        if (mutex_)
        {
            // cppcheck-suppress cstyleCast
            xSemaphoreGive(mutex_);
        }
    }

    static inline SemaphoreHandle_t mutex_{nullptr};
    const size_t MUTEX_TIMEOUT_{5000};
    const std::string TAG_ = "Spiffs";
};

#endif  // SPIFFS_H
