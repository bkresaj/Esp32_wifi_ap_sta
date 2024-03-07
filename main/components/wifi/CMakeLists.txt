cmake_minimum_required(VERSION 3.20)

project(wifi)

add_library(${PROJECT_NAME} wifi.cpp)

add_library(bkr::wifi ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC idf::esp_wifi bkr::https_server)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
