cmake_minimum_required(VERSION 3.16)

project(dice_parser VERSION 2.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTS "Build units tests for the project" ON)

add_subdirectory(src)
if(BUILD_TESTS)
  add_subdirectory(test)
  add_subdirectory(repl)
endif()

set_target_properties(dice_parser PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
