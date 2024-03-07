cmake_minimum_required(VERSION 3.20)
set(INTERNAL_NAME mock_https_server)

add_library(${INTERNAL_NAME} mock_https_server.cpp)

target_link_libraries(${INTERNAL_NAME} PUBLIC gmock singleton_template mock_spiffs)

target_include_directories(${INTERNAL_NAME}
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
