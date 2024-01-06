#pragma once

#include <gmock/gmock.h>
#include <singleton_template.h>

#include <array>
#include <string>

class Spiffs
{
   public:
    static Spiffs& getInstance();
    bool init();
    size_t getFileSize(const std::string filename);
    bool readFile(const std::string filename, std::string& data);
    bool writeToFile(const std::string filename, const std::string& data);
    Spiffs();
    ~Spiffs() = default;
    Spiffs(const Spiffs&) = delete;
    Spiffs& operator=(const Spiffs&) = delete;
    Spiffs(Spiffs&&) = delete;
    Spiffs& operator=(Spiffs&&) = delete;
};
