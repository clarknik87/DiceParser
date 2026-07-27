find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# pass project version defined at top level CMakeLists.txt 
# set the version patch to the number of commits since the last version tag
function(get_commit_count)
    find_package(Git QUIET)
    if(NOT Git_FOUND)
        message(WARNING "Git not found")
        return()
    endif()

    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_TAG
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE GIT_RESULT
    )

    if(NOT GIT_RESULT EQUAL 0)
        message(WARNING "Failed to get git tag")
        return()
    endif()

    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-list --count ${GIT_TAG}..HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_COUNT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE GIT_COUNT_RESULT
    )

    if(NOT GIT_RESULT EQUAL 0)
        message(WARNING "Failed to count commits since ${GIT_TAG}")
        return()
    endif()

    set(GIT_COMMIT_COUNT ${GIT_COMMIT_COUNT} PARENT_SCOPE)
endfunction()

get_commit_count()
set(PROJECT_VERSION_PATCH ${GIT_COMMIT_COUNT})
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/../version.hpp"
)

BISON_TARGET(bison_parser
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.yy
    ${CMAKE_CURRENT_BINARY_DIR}/parser.cc
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.hh
)

FLEX_TARGET(flex_scanner
    ${CMAKE_CURRENT_SOURCE_DIR}/scanner.ll
    ${CMAKE_CURRENT_BINARY_DIR}/scanner.cc
)

ADD_FLEX_BISON_DEPENDENCY(flex_scanner bison_parser)

add_library(dice_parser STATIC
    ${FLEX_flex_scanner_OUTPUTS}
    ${BISON_bison_parser_OUTPUTS}
    builtin_func.cpp
    dice_pdf.cpp
    dice_distribution.cpp
    dice_parser.cpp
    variable_map.cpp
)

target_include_directories(dice_parser PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Ensure dice_parser.cpp waits for generated files
set_source_files_properties(
    ${CMAKE_CURRENT_SOURCE_DIR}/dice_parser.cpp
    PROPERTIES
        OBJECT_DEPENDS ${BISON_bison_parser_OUTPUT_HEADER}
)
