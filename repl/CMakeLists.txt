add_executable(repl
    repl.cpp
)

target_link_libraries(repl PRIVATE
    dice_parser
)
