#include "mock_stdio.h"

#include <cstdarg>

extern "C"
{
    static bool real_functions_flag = false;

    FILE* __wrap_fopen(const char* __filename, const char* __modes)
    {
        try
        {
            return MockStdio::getInstance().fopen(__filename, __modes);
        }
        catch (const std::exception& e)
        {
            return __real_fopen(__filename, __modes);
        }
    }

    void changeToRealFunctions()
    {
        real_functions_flag = true;
    }

    size_t __wrap_fread(void* __ptr, size_t __size, size_t __n, FILE* __stream)
    {
        try
        {
            if (!real_functions_flag)
            {
                return MockStdio::getInstance().fread(__ptr, __size, __n,
                                                      __stream);
            }
            else
            {
                return __real_fread(__ptr, __size, __n, __stream);
            }
        }
        catch (const std::exception& e)
        {
            if (!real_functions_flag)
            {
                return __real_fread(__ptr, __size, __n, __stream);
            }
            else
            {
                std::cerr << "Exception: " << e.what() << std::endl;
            }
        }
    }

    int __wrap_fclose(FILE* __stream)
    {
        try
        {
            if (!real_functions_flag)
            {
                return MockStdio::getInstance().fclose(__stream);
            }
            else
            {
                return __real_fclose(__stream);
            }
        }
        catch (const std::exception& e)
        {
            if (!real_functions_flag)
            {
                return __real_fclose(__stream);
            }
            else
            {
                std::cerr << "Exception: " << e.what() << std::endl;
            }
        }
    }

    int __wrap_fprintf(FILE* stream, const char* format, ...)
    {
        try
        {
            std::string non_variadic = "";

            if (format != NULL)
            {
                va_list args;

                va_start(args, format);
                // Get length of format including arguments
                int nr = vsnprintf(NULL, 0, format, args);
                va_end(args);

                char buffer[nr + 1];
                va_start(args, format);
                vsnprintf(buffer, nr + 1, format, args);
                va_end(args);

                non_variadic = std::string(buffer);
            }

            return MockStdio::getInstance().fprintf(stream,
                                                    non_variadic.c_str());
        }
        catch (const std::exception& e)
        {
            return __real_fprintf(stream, format);
        }
    }

    int __wrap_fseek(FILE* __stream, long int __off, int __whence)
    {
        try
        {
            if (!real_functions_flag)
            {
                return MockStdio::getInstance().fseek(__stream, __off,
                                                      __whence);
            }
            else
            {
                return __real_fseek(__stream, __off, __whence);
            }
        }
        catch (const std::exception& e)
        {
            if (!real_functions_flag)
            {
                return __real_fseek(__stream, __off, __whence);
            }
            else
            {
                std::cerr << "Exception: " << e.what() << std::endl;
            }
        }
    }

    long int __wrap_ftell(FILE* __stream)
    {
        try
        {
            return MockStdio::getInstance().ftell(__stream);
        }
        catch (const std::exception& e)
        {
            return __real_ftell(__stream);
        }
    }
}
