#include <gtest/gtest.h>
#include <spiffs.h>
#include <stdio.h>

#include "esp_log.h"
#include "mock_esp_spiffs.h"
#include "mock_freertos.h"
#include "mock_stdio.h"

using namespace testing;

class UtSpiffs : public ::Test
{
   public:
    StrictMock<MockEspSpiffs> mock_esp_spiffs;
    StrictMock<MockFreertos> mock_freertos;
};

TEST_F(UtSpiffs, assertIfEspSpiffsRegisterFailsWhenConstructingSpiffs)
{
#ifdef VALGRIND
    GTEST_FLAG_SET(death_test_style, "threadsafe");
#endif

    ASSERT_DEATH(
        {
            InSequence s;

            EXPECT_CALL(mock_esp_spiffs, esp_vfs_spiffs_register(_))
                .WillOnce(Return(ESP_FAIL));
            EXPECT_CALL(mock_freertos, xSemaphoreCreateMutex()).Times(0);

            Spiffs spiffs;
        },
        ".*");
}

TEST_F(UtSpiffs, assertIfMutexCreationFailsWhenConstructingSpiffs)
{
#ifdef VALGRIND
    GTEST_FLAG_SET(death_test_style, "threadsafe");
#endif

    ASSERT_DEATH(
        {
            InSequence s;

            EXPECT_CALL(mock_esp_spiffs, esp_vfs_spiffs_register(_))
                .WillOnce(Return(ESP_OK));
            EXPECT_CALL(mock_freertos, xSemaphoreCreateMutex()).Times(1);

            Spiffs spiffs;
        },
        ".*");
}

TEST_F(UtSpiffs, assertIfInitFailsWhenConstructingSpiffs)
{
#ifdef VALGRIND
    GTEST_FLAG_SET(death_test_style, "threadsafe");
#endif

    ASSERT_DEATH(
        {
            InSequence s;

            EXPECT_CALL(mock_esp_spiffs, esp_vfs_spiffs_register(_))
                .WillOnce(Return(ESP_OK));
            EXPECT_CALL(mock_freertos, xSemaphoreCreateMutex()).Times(1);
            EXPECT_CALL(mock_freertos, xSemaphoreTake(_, _))
                .WillOnce(Return(pdFALSE));

            Spiffs spiffs;
        },
        ".*");
}

TEST_F(UtSpiffs, constructingSpiffsSuccess)
{
    InSequence s;

    SemaphoreHandle_t handle = reinterpret_cast<SemaphoreHandle_t>(1);

    EXPECT_CALL(mock_esp_spiffs, esp_vfs_spiffs_register(_))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mock_freertos, xSemaphoreCreateMutex())
        .WillOnce(Return(handle));

    StrictMock<MockStdio> mock_stdio;

    size_t size_of_html_file{1197};
    FILE* file = reinterpret_cast<FILE*>(1);

    EXPECT_CALL(mock_freertos, xSemaphoreTake(_, _)).WillOnce(Return(pdTRUE));
    EXPECT_CALL(mock_stdio, fopen(_, _)).WillOnce(Return(file));
    EXPECT_CALL(mock_stdio, fprintf(_, _)).WillOnce(Return(size_of_html_file));
    EXPECT_CALL(mock_stdio, fclose(_)).Times(1);
    EXPECT_CALL(mock_freertos, xSemaphoreGive(_)).Times(1);

    Spiffs spiffs;
}

TEST_F(UtSpiffs, getFileSizeFailsIfTakingMutexFails)
{
    SemaphoreHandle_t handle = reinterpret_cast<SemaphoreHandle_t>(1);

    EXPECT_CALL(mock_esp_spiffs, esp_vfs_spiffs_register(_))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mock_freertos, xSemaphoreCreateMutex())
        .WillOnce(Return(handle));

    StrictMock<MockStdio> mock_stdio;

    size_t size_of_html_file{1197};
    FILE* file = reinterpret_cast<FILE*>(1);

    EXPECT_CALL(mock_freertos, xSemaphoreTake(_, _)).WillOnce(Return(pdTRUE));
    EXPECT_CALL(mock_stdio, fopen(_, _)).WillOnce(Return(file));
    EXPECT_CALL(mock_stdio, fprintf(_, _)).WillOnce(Return(size_of_html_file));
    EXPECT_CALL(mock_stdio, fclose(_)).Times(1);
    EXPECT_CALL(mock_freertos, xSemaphoreGive(_)).Times(1);

    Spiffs spiffs;

    std::string file_path = "/spiffs/html_file";

    EXPECT_CALL(mock_freertos, xSemaphoreTake(_, _)).WillOnce(Return(pdFALSE));

    EXPECT_FALSE(spiffs.getFileSize(file_path));
}

TEST_F(UtSpiffs, getFileSizeFailsIfOpeningFileFails)
{
    InSequence s;

    SemaphoreHandle_t handle = reinterpret_cast<SemaphoreHandle_t>(1);

    EXPECT_CALL(mock_esp_spiffs, esp_vfs_spiffs_register(_))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mock_freertos, xSemaphoreCreateMutex())
        .WillOnce(Return(handle));

    StrictMock<MockStdio> mock_stdio;

    size_t size_of_html_file{1197};
    FILE* file = reinterpret_cast<FILE*>(1);

    EXPECT_CALL(mock_freertos, xSemaphoreTake(_, _)).WillOnce(Return(pdTRUE));
    EXPECT_CALL(mock_stdio, fopen(_, _)).WillOnce(Return(file));
    EXPECT_CALL(mock_stdio, fprintf(_, _)).WillOnce(Return(size_of_html_file));
    EXPECT_CALL(mock_stdio, fclose(_)).Times(1);
    EXPECT_CALL(mock_freertos, xSemaphoreGive(_)).Times(1);

    Spiffs spiffs;

    std::string file_path = "/spiffs/html_file";

    EXPECT_CALL(mock_freertos, xSemaphoreTake(_, _)).WillOnce(Return(pdTRUE));
    EXPECT_CALL(mock_stdio, fopen(_, _)).WillOnce(Return(nullptr));
    EXPECT_CALL(mock_freertos, xSemaphoreGive(_)).Times(1);

    EXPECT_FALSE(spiffs.getFileSize(file_path));
}

TEST_F(UtSpiffs, getFileSizeSucces)
{
    InSequence s;

    SemaphoreHandle_t handle = reinterpret_cast<SemaphoreHandle_t>(1);

    EXPECT_CALL(mock_esp_spiffs, esp_vfs_spiffs_register(_))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mock_freertos, xSemaphoreCreateMutex())
        .WillOnce(Return(handle));

    StrictMock<MockStdio> mock_stdio;

    size_t size_of_html_file{1197};
    FILE* file = reinterpret_cast<FILE*>(1);

    EXPECT_CALL(mock_freertos, xSemaphoreTake(_, _)).WillOnce(Return(pdTRUE));
    EXPECT_CALL(mock_stdio, fopen(_, _)).WillOnce(Return(file));
    EXPECT_CALL(mock_stdio, fprintf(_, _)).WillOnce(Return(size_of_html_file));
    EXPECT_CALL(mock_stdio, fclose(_)).Times(1);
    EXPECT_CALL(mock_freertos, xSemaphoreGive(_)).Times(1);

    Spiffs spiffs;

    size_t file_size{1197};
    std::string file_path = "/spiffs/html_file";

    EXPECT_CALL(mock_freertos, xSemaphoreTake(_, _)).WillOnce(Return(pdTRUE));
    EXPECT_CALL(mock_stdio, fopen(_, _)).WillOnce(Return(file));
    EXPECT_CALL(mock_stdio, fseek(_, _, _)).Times(1);
    EXPECT_CALL(mock_stdio, ftell(_)).WillOnce(Return(file_size));
    EXPECT_CALL(mock_stdio, fclose(_)).Times(1);
    EXPECT_CALL(mock_freertos, xSemaphoreGive(_)).Times(1);

    EXPECT_EQ(spiffs.getFileSize(file_path), file_size);
}

TEST_F(UtSpiffs, readFileFailsIfTakingMutexFails)
{
    SemaphoreHandle_t handle = reinterpret_cast<SemaphoreHandle_t>(1);

    EXPECT_CALL(mock_esp_spiffs, esp_vfs_spiffs_register(_))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mock_freertos, xSemaphoreCreateMutex())
        .WillOnce(Return(handle));

    StrictMock<MockStdio> mock_stdio;

    size_t size_of_html_file{1197};
    FILE* file = reinterpret_cast<FILE*>(1);

    EXPECT_CALL(mock_freertos, xSemaphoreTake(_, _)).WillOnce(Return(pdTRUE));
    EXPECT_CALL(mock_stdio, fopen(_, _)).WillOnce(Return(file));
    EXPECT_CALL(mock_stdio, fprintf(_, _)).WillOnce(Return(size_of_html_file));
    EXPECT_CALL(mock_stdio, fclose(_)).Times(1);
    EXPECT_CALL(mock_freertos, xSemaphoreGive(_)).Times(1);

    Spiffs spiffs;
    std::string data = "";

    EXPECT_CALL(mock_freertos, xSemaphoreTake(_, _)).WillOnce(Return(pdFALSE));

    EXPECT_FALSE(spiffs.readFile("/spiffs/html_file", data));
}

TEST_F(UtSpiffs, readFileFailsIfOpeningFileFails)
{
    InSequence s;

    SemaphoreHandle_t handle = reinterpret_cast<SemaphoreHandle_t>(1);

    EXPECT_CALL(mock_esp_spiffs, esp_vfs_spiffs_register(_))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mock_freertos, xSemaphoreCreateMutex())
        .WillOnce(Return(handle));

    StrictMock<MockStdio> mock_stdio;

    size_t size_of_html_file{1197};
    FILE* file = reinterpret_cast<FILE*>(1);

    EXPECT_CALL(mock_freertos, xSemaphoreTake(_, _)).WillOnce(Return(pdTRUE));
    EXPECT_CALL(mock_stdio, fopen(_, _)).WillOnce(Return(file));
    EXPECT_CALL(mock_stdio, fprintf(_, _)).WillOnce(Return(size_of_html_file));
    EXPECT_CALL(mock_stdio, fclose(_)).Times(1);
    EXPECT_CALL(mock_freertos, xSemaphoreGive(_)).Times(1);

    Spiffs spiffs;

    std::string data = "";

    EXPECT_CALL(mock_freertos, xSemaphoreTake(_, _)).WillOnce(Return(pdTRUE));
    EXPECT_CALL(mock_stdio, fopen(_, _)).WillOnce(Return(nullptr));
    EXPECT_CALL(mock_freertos, xSemaphoreGive(_)).Times(1);

    EXPECT_FALSE(spiffs.readFile("/spiffs/html_file", data));
}

TEST_F(UtSpiffs, readFileFailsWhenFreadFails)
{
    InSequence s;

    SemaphoreHandle_t handle = reinterpret_cast<SemaphoreHandle_t>(1);

    EXPECT_CALL(mock_esp_spiffs, esp_vfs_spiffs_register(_))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mock_freertos, xSemaphoreCreateMutex())
        .WillOnce(Return(handle));

    StrictMock<MockStdio> mock_stdio;

    size_t size_of_html_file{1197};
    FILE* file = reinterpret_cast<FILE*>(1);

    EXPECT_CALL(mock_freertos, xSemaphoreTake(_, _)).WillOnce(Return(pdTRUE));
    EXPECT_CALL(mock_stdio, fopen(_, _)).WillOnce(Return(file));
    EXPECT_CALL(mock_stdio, fprintf(_, _)).WillOnce(Return(size_of_html_file));
    EXPECT_CALL(mock_stdio, fclose(_)).Times(1);
    EXPECT_CALL(mock_freertos, xSemaphoreGive(_)).Times(1);

    Spiffs spiffs;

    std::string data = "";

    EXPECT_CALL(mock_freertos, xSemaphoreTake(_, _)).WillOnce(Return(pdTRUE));
    EXPECT_CALL(mock_stdio, fopen(_, _)).WillOnce(Return(file));
    EXPECT_CALL(mock_stdio, fread(_, _, _, _)).WillOnce(Return(0));
    EXPECT_CALL(mock_stdio, fclose(_)).Times(1);
    EXPECT_CALL(mock_freertos, xSemaphoreGive(_)).Times(1);

    EXPECT_FALSE(spiffs.readFile("/spiffs/html_file", data));
}

TEST_F(UtSpiffs, readFileSuccess)
{
    InSequence s;

    SemaphoreHandle_t handle = reinterpret_cast<SemaphoreHandle_t>(1);

    EXPECT_CALL(mock_esp_spiffs, esp_vfs_spiffs_register(_))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mock_freertos, xSemaphoreCreateMutex())
        .WillOnce(Return(handle));

    StrictMock<MockStdio> mock_stdio;

    size_t size_of_html_file{1197};
    FILE* file = reinterpret_cast<FILE*>(1);

    EXPECT_CALL(mock_freertos, xSemaphoreTake(_, _)).WillOnce(Return(pdTRUE));
    EXPECT_CALL(mock_stdio, fopen(_, _)).WillOnce(Return(file));
    EXPECT_CALL(mock_stdio, fprintf(_, _)).WillOnce(Return(size_of_html_file));
    EXPECT_CALL(mock_stdio, fclose(_)).Times(1);
    EXPECT_CALL(mock_freertos, xSemaphoreGive(_)).Times(1);

    Spiffs spiffs;

    std::string data = "";

    EXPECT_CALL(mock_freertos, xSemaphoreTake(_, _)).WillOnce(Return(pdTRUE));
    EXPECT_CALL(mock_stdio, fopen(_, _)).WillOnce(Return(file));
    EXPECT_CALL(mock_stdio, fread(_, _, _, _)).WillOnce(Return(5));
    EXPECT_CALL(mock_stdio, fclose(_)).Times(1);
    EXPECT_CALL(mock_freertos, xSemaphoreGive(_)).Times(1);

    EXPECT_TRUE(spiffs.readFile("/spiffs/html_file", data));

#ifdef COVERAGE
    // this line has nothing to with tests, it was added here because seg fault
    // was raised when gcov used wrap function after tests, since gcov needs
    // real functions like fread,fclose etc
    changeToRealFunctions();
#endif
}
