add_executable(unit_tests 
    main.cpp
)

target_link_libraries(unit_tests PRIVATE dice_parser)
