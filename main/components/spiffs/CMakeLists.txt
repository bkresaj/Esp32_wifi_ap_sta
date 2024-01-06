cmake_minimum_required(VERSION 3.20)

project(spiffs)

add_library(${PROJECT_NAME} spiffs.cpp)

add_library(bkr::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC idf::spiffs idf::freertos)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
