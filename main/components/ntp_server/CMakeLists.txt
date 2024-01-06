cmake_minimum_required(VERSION 3.20)

project(ntp_server)

add_library(${PROJECT_NAME} ntp_server.c)

add_library(bkr::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC idf::lwip)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
