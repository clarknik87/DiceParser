include(FetchContent)
FetchContent_Declare(
  googletest
  DOWNLOAD_EXTRACT_TIMESTAMP ON
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
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
