#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <gtest/gtest.h>
#include "unit_test.hpp"

TEST(valid, version)
{
    EXPECT_EQ(DiceParser::version_major, 2);
    EXPECT_EQ(DiceParser::version_minor, 0);
    EXPECT_GE(DiceParser::version_patch, 0);
}

// TEST(invalid, error_codes)
// {
//     std::vector<err_case_t> error_cases{
//         // Invaliid tokens
//         {"~2d6", action_code::unknown_symbol},
//         {"7 6 -", action_code::invalid_syntax},
//     };
    
//     DiceParser parser(
//     {
//         {"const1", "1.0"},
//         {"const2", "2.0"},
//         {"cdice1", "3d1"},
//         {"cdice2", "4d1"}
//     },
//     {}
//     );
//     for(auto error_case : error_cases)
//     {
//         auto parse_ans = std::get<action_code>(parser.parse(error_case.test_str));
//         EXPECT_EQ(parse_ans, error_case.ans) << error_case.test_str;
//     }

//     EXPECT_EQ(std::get<action_code>(parser.parse("const1 = 5.0")), action_code::const_assignment_err);
//     EXPECT_EQ(std::get<action_code>(parser.parse("const2 = 3d6")), action_code::const_assignment_err);
//     EXPECT_EQ(std::get<action_code>(parser.parse("cdice1 = 6.7")), action_code::const_assignment_err);
//     EXPECT_EQ(std::get<action_code>(parser.parse("cdice2 = 3d6")), action_code::const_assignment_err);

// }

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    if (RUN_ALL_TESTS())
        ;

    return 0;
}
