include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests 
    main.cpp
)

target_link_libraries(unit_tests PRIVATE 
    dice_parser
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
