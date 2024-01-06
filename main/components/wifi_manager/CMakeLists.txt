cmake_minimum_required(VERSION 3.20)

project(wifi_manager)

add_library(${PROJECT_NAME} wifi_manager.cpp wifi_transition.cpp
                            wifi_states.cpp)

add_library(bkr::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC bkr::wifi bkr::http_server
                                             idf::freertos bkr::ntp_server)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
