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

TEST(parser, error_codes)
{
    std::vector<err_case_t> error_cases{
        // Invalid tokens
        {"~2d6", action_code::unknown_symbol},
        {"7 6 -", action_code::invalid_syntax},
    };
    
    DiceParser parser;
    for(auto error_case : error_cases)
    {
        auto parse_ans = std::get<action_code>(parser.parse(error_case.test_str));
        EXPECT_EQ(parse_ans, error_case.ans) << error_case.test_str;
    }
}

TEST(parser, empty_statements)
{
    DiceParser parser;
    EXPECT_EQ(std::get<action_code>(parser.parse("")), action_code::empty_command);
    EXPECT_EQ(std::get<action_code>(parser.parse(";")), action_code::empty_command);
    EXPECT_EQ(std::get<action_code>(parser.parse(";;;")), action_code::empty_command);
}

TEST(parser, multiple_statements)
{
    DiceParser parser;
    EXPECT_EQ(std::get<double>(parser.parse("1+1;2")), 2);
    EXPECT_EQ(std::get<double>(parser.parse("1+1;2;")), 2);
    EXPECT_EQ(std::get<double>(parser.parse("A=2; A+1; A")), 2);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    if (RUN_ALL_TESTS())
        ;

    return 0;
}
