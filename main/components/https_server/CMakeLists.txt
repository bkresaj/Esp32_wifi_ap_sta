cmake_minimum_required(VERSION 3.20)

project(https_server)

add_library(${PROJECT_NAME} https_server.cpp)

add_library(bkr::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC idf::esp_https_server bkr::spiffs)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
