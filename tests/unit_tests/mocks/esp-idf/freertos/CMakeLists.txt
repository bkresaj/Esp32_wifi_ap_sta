cmake_minimum_required(VERSION 3.20)
set(INTERNAL_NAME mock_freertos)

add_library(${INTERNAL_NAME} mock_freertos.cpp)

target_link_libraries(${INTERNAL_NAME} PUBLIC gmock singleton_template)

target_include_directories(${INTERNAL_NAME}
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
