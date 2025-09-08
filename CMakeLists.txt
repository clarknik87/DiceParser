cmake_minimum_required(VERSION 3.16)

project(dice_parser)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTS "Build units tests for the project" ON)

include(FetchContent)
FetchContent_Declare(
  eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG        3.4.0
)
FetchContent_MakeAvailable(eigen)

add_subdirectory(src)
if(BUILD_TESTS)
  add_subdirectory(test)
  add_subdirectory(repl)
endif()

target_link_libraries(dice_parser PUBLIC
    Eigen3::Eigen
)

set_target_properties(dice_parser PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
