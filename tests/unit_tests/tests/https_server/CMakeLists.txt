cmake_minimum_required(VERSION 3.20)

set(TARGET_NAME ut_https_server)

add_executable(
  ${TARGET_NAME}
  ut_https_server.cpp
  ${PROJECT_DIRECTORY}/main/components/https_server/https_server.cpp)

if(DEFINED VALGRIND)
  add_test(
    NAME ${TARGET_NAME}
    COMMAND valgrind --leak-check=full # --show-leak-kinds=all
            $<TARGET_FILE:${TARGET_NAME}>
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
else()
  add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
endif()

target_link_libraries(${TARGET_NAME} PUBLIC gtest gtest_main mock_esp_log
                                            mock_spiffs mock_esp_https_server)

target_include_directories(
  ${TARGET_NAME} PRIVATE ${PROJECT_DIRECTORY}/main/components/https_server)
