cmake_minimum_required(VERSION 3.20)
project(ap_and_station)

set(CMAKE_CXX_STANDARD 17)

include($ENV{IDF_PATH}/tools/cmake/idf.cmake)

idf_build_process(
  "esp32"
  COMPONENTS
  freertos
  esptool_py
  esp_wifi
  nvs_flash
  esp_https_server
  spiffs
  lwip
  SDKCONFIG
  ${CMAKE_CURRENT_LIST_DIR}/sdkconfig
  BUILD_DIR
  ${CMAKE_BINARY_DIR})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_definitions(CHAR_BIT=8)

add_executable(${CMAKE_PROJECT_NAME}.elf main.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}.elf bkr::spiffs idf::nvs_flash
                      bkr::wifi_manager)

add_compile_options(-g)

idf_build_executable(${CMAKE_PROJECT_NAME}.elf)

add_subdirectory(components)
