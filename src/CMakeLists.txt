find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

BISON_TARGET(parser
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.yy
    ${CMAKE_CURRENT_BINARY_DIR}/parser.cc
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.hh
)

FLEX_TARGET(built_lexer
    ${CMAKE_CURRENT_SOURCE_DIR}/scanner.ll
    ${CMAKE_CURRENT_BINARY_DIR}/scanner.cc
)

add_executable(dice_lexer
    ${FLEX_built_lexer_OUTPUTS}
    ${BISON_parser_OUTPUTS}
)

target_include_directories(dice_lexer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)
