cmake_minimum_required(VERSION 3.20)

set(TARGET_NAME ut_example)

add_executable(${TARGET_NAME} ut_example.cpp)

if(DEFINED VALGRIND)
  add_test(
    NAME ${TARGET_NAME}
    COMMAND valgrind --leak-check=full # --show-leak-kinds=all
            $<TARGET_FILE:${TARGET_NAME}>
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
else()
  add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
endif()

target_link_libraries(${TARGET_NAME} PUBLIC gtest gtest_main)
