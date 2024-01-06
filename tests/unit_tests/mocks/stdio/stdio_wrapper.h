#pragma once

#include <stdio.h>

extern "C"
{
    FILE* __real_fopen(const char* __filename, const char* __modes);
    size_t __real_fread(void* __ptr, size_t __size, size_t __n, FILE* __stream);
    int __real_fclose(FILE* __stream);
    int __real_fprintf(FILE* stream, const char* format, ...);
    int __real_fseek(FILE* __stream, long int __off, int __whence);
    long int __real_ftell(FILE* __stream);

    FILE* __wrap_fopen(const char* __filename, const char* __modes);
    size_t __wrap_fread(void* __ptr, size_t __size, size_t __n, FILE* __stream);
    int __wrap_fclose(FILE* __stream);
    int __wrap_fprintf(FILE* stream, const char* format, ...);
    int __wrap_fseek(FILE* __stream, long int __off, int __whence);
    long int __wrap_ftell(FILE* __stream);

    // function added from which we could alter between real or custom functions
    // on runtime
    void changeToRealFunctions();
}