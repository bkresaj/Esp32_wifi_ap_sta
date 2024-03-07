cmake_minimum_required(VERSION 3.20)
set(INTERNAL_NAME mock_esp-tls)

add_library(${INTERNAL_NAME} INTERFACE)

target_include_directories(${INTERNAL_NAME} INTERFACE .)
