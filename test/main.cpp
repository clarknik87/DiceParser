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

// TEST(interpolate, valid)
// {
//     constexpr int dex{3};
//     constexpr int prof{2};

//     std::vector<interpolate_case_t> test_cases{
//         {"{}", ""},
//         {"{dex}", std::to_string(dex)},
//         {"{prof}", std::to_string(prof)},
//         {"{7+3}", std::to_string(10)},
//         {"1d20+{prof}", "1d20+" + std::to_string(prof)},
//         {"prof", "prof"},
//         {"1d8+{dex+prof}", "1d8+" + std::to_string(dex+prof)},
//         {"2d6+{dex+4}", "2d6+" + std::to_string(dex+4)},
//         {"{dex}+1d20+{prof}", std::to_string(dex)+"+1d20+"+std::to_string(prof)},
//         {"{}{}{}{}{}dex", "dex"},
//     };

//     DiceParser parser(
//         {
//             {"dex", std::to_string(dex)},
//             {"prof", std::to_string(prof)}
//         },
//         {}
//     );
//     for(auto test_case: test_cases)
//     {
//         EXPECT_EQ(parser.interpolate(test_case.test), test_case.ans);
//     }
// }

// TEST(interpolate, invalid)
// {
//     std::vector<interpolate_case_t> test_cases{
//         {"{dex}", ""},
//         {"{prof}", ""},
//         {"1d20+{prof}", "1d20+"},
//         {"1d8+{dex+prof}", "1d8+"},
//         {"2d6+{dex+4}", "2d6+"},
//         {"{dex}+1d20+{prof}", "+1d20+"},
//         {"1d20+{dex", "1d20+{dex"},
//         {"1d20+dex}", "1d20+dex}"},
//         {"1d20+{{7+4}", "1d20+"},
//         {"{{dex}}", "}"}
//     };

//     DiceParser parser;
//     for(auto test_case: test_cases)
//     {
//         EXPECT_EQ(parser.interpolate(test_case.test), test_case.ans);
//     }
// }

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    if (RUN_ALL_TESTS())
        ;

    return 0;
}
