cmake_minimum_required(VERSION 3.20)

set(TARGET_NAME ut_wifi)

add_executable(${TARGET_NAME}
               ut_wifi.cpp ${PROJECT_DIRECTORY}/main/components/wifi/wifi.cpp)

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
  ${TARGET_NAME}
  PUBLIC gtest
         gtest_main
         gmock
         mock_esp_wifi
         mock_freertos
         mock_esp_log
         mock_https_server
         mock_esp_netif
         mock_esp_event)

target_include_directories(${TARGET_NAME}
                           PRIVATE ${PROJECT_DIRECTORY}/main/components/wifi)
