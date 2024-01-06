#pragma once

#include "spiffs.h"

class MockSpiffs : public SingletonTemplate<MockSpiffs>
{
   public:
    MOCK_METHOD(bool, init, (), ());
    MOCK_METHOD(size_t, getFileSize, (const std::string filename), ());
    MOCK_METHOD(bool, readFile, (const std::string filename, std::string& data),
                ());
    MOCK_METHOD(bool, writeToFile,
                (const std::string filename, const std::string& data), ());
};
