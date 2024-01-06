cmake_minimum_required(VERSION 3.20)

project(unit_tests)

enable_testing()

set(PROJECT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/..")

if(DEFINED VALGRIND AND (DEFINED COVERAGE OR DEFINED HTML_COVERAGE))
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g --coverage")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g --coverage")
elseif(DEFINED VALGRIND)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
elseif(DEFINED COVERAGE OR DEFINED HTML_COVERAGE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
endif()

# build google test together with the project
execute_process(COMMAND ${CMAKE_COMMAND} .
                WORKING_DIRECTORY /root/gtest/googletest)
execute_process(COMMAND ${CMAKE_COMMAND} --build .
                WORKING_DIRECTORY /root/gtest/googletest)

add_subdirectory(/root/gtest/googletest /root/gtest/googletest/build
                 EXCLUDE_FROM_ALL)
add_subdirectory(unit_tests)

if(DEFINED COVERAGE OR DEFINED HTML_COVERAGE)
  set(COVERAGE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/coverage")

  file(MAKE_DIRECTORY ${COVERAGE_OUTPUT_DIRECTORY})

  if(DEFINED HTML_COVERAGE)
    set(GCOVR_ARGUMENTS
        --root=${PROJECT_DIRECTORY}
        --exclude=${PROJECT_DIRECTORY}/external
        --exclude-directories=unit_tests/mocks/
        --html
        --html-details
        -o
        ${COVERAGE_OUTPUT_DIRECTORY}/coverage.html
        --print-summary)
  else()
    set(GCOVR_ARGUMENTS
        --root=${PROJECT_DIRECTORY}
        --exclude=${PROJECT_DIRECTORY}/external
        --exclude-directories=unit_tests/mocks/
        --print-summary
        |
        tail
        -3)
  endif()

  add_custom_target(
    coverage
    COMMAND gcovr ${GCOVR_ARGUMENTS}
    DEPENDS unit_tests)

endif()
