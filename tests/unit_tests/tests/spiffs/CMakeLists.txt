cmake_minimum_required(VERSION 3.20)

set(TARGET_NAME ut_spiffs)

add_executable(
  ${TARGET_NAME} ut_spiffs.cpp
                 ${PROJECT_DIRECTORY}/main/components/spiffs/spiffs.cpp)

target_compile_definitions(${TARGET_NAME} PRIVATE VALGRIND=$<BOOL:${VALGRIND}>
                                                  COVERAGE=$<BOOL:${COVERAGE}>)

if(DEFINED VALGRIND)
  add_test(
    NAME ${TARGET_NAME}
    COMMAND valgrind --leak-check=full # --show-leak-kinds=all
            $<TARGET_FILE:${TARGET_NAME}>
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
else()
  add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
endif()

target_link_libraries(
  ${TARGET_NAME} PUBLIC gtest gtest_main mock_esp_log mock_esp_spiffs
                        mock_stdio mock_freertos)

target_link_options(
  ${TARGET_NAME}
  PUBLIC
  -Wl,--wrap=fopen,--wrap=fread,--wrap=fclose,--wrap=fprintf,--wrap=fseek,--wrap=ftell
)

target_include_directories(${TARGET_NAME}
                           PRIVATE ${PROJECT_DIRECTORY}/main/components/spiffs)
