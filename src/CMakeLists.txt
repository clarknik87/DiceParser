find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# pass project version defined at top level CMakeLists.txt 
add_custom_target(GenerateVersion ALL
  COMMAND ${CMAKE_COMMAND}
    -DSRC_DIR=${CMAKE_CURRENT_SOURCE_DIR}
    -DBIN_DIR=${CMAKE_CURRENT_BINARY_DIR}
    -DPROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    -DPROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    -P "${CMAKE_CURRENT_SOURCE_DIR}/GenerateVersion.cmake"
  BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/version.h"
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
    dice_pmf.cpp
    dice_distribution.cpp
    dice_parser.cpp
    variable_map.cpp
)

add_dependencies(dice_parser GenerateVersion)

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
