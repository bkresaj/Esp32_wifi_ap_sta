#pragma once

#include <gmock/gmock.h>
#include <singleton_template.h>

#include "stdio_wrapper.h"

class MockStdio : public SingletonTemplate<MockStdio>
{
   public:
    MOCK_METHOD(FILE*, fopen, (const char* __filename, const char* __modes),
                ());
    MOCK_METHOD(size_t, fread,
                (void* __ptr, size_t __size, size_t __n, FILE* __stream), ());
    MOCK_METHOD(int, fclose, (FILE * __stream), ());
    MOCK_METHOD(int, fprintf, (FILE*, const char*), ());
    MOCK_METHOD(int, fseek, (FILE * __stream, long int __off, int __whence),
                ());
    MOCK_METHOD(long int, ftell, (FILE * __stream), ());
};