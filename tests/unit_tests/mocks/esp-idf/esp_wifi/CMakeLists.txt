cmake_minimum_required(VERSION 3.20)
set(INTERNAL_NAME mock_esp_wifi)

add_library(${INTERNAL_NAME} mock_esp_wifi.cpp)

target_link_libraries(
  ${INTERNAL_NAME} PUBLIC gmock singleton_template mock_esp_netif mock_esp_event
                          mock_esp_common mock_esp_hw_support)

target_include_directories(${INTERNAL_NAME}
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
