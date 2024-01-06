#include "mock_spiffs.h"

Spiffs& Spiffs::getInstance()
{
    static Spiffs spiffs;
    return spiffs;
}

Spiffs::Spiffs()
{
}

bool Spiffs::init()
{
    return MockSpiffs::getInstance().init();
}

size_t Spiffs::getFileSize(const std::string filename)
{
    return MockSpiffs::getInstance().getFileSize(filename);
}

bool Spiffs::readFile(const std::string filename, std::string& data)
{
    return MockSpiffs::getInstance().readFile(filename, data);
}

bool Spiffs::writeToFile(const std::string filename, const std::string& data)
{
    return MockSpiffs::getInstance().writeToFile(filename, data);
}