# GENERATE_VERSION.cmake
#
# This script allows the patch version to increment with every git commit
#
# It should be called from the local CMakeLists.txt as it's own target, to
# ensure that the version info is reconfigured every build, not just when CMake
# is reconfigured. The script expects certain variable to be defined as
# arguments when it is called:
#       -DSRC_DIR=${CMAKE_CURRENT_SOURCE_DIR}             // or wherever version.hpp.in is located 
#       -DBIN_DIR=${CMAKE_CURRENT_BINARY_DIR}             // wherever version.hpp will be placed
#       -DPROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}  // or whatever the major version should be
#       -DPROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}  // or whatever the minor version should be

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
    "${SRC_DIR}/version.hpp.in"
    "${BIN_DIR}/../version.hpp"
)
