#include <string>
#include <sstream>
#include <vector>
#include <gtest/gtest.h>

#include "dice_parser/dice_parser.hpp"

struct test_case_t
{
    std::string test_str;
    double      ans;
};

struct err_case_t
{
    std::string test_str;
    err_code    ans;
};

TEST(valid, arithmetic)
{
    std::vector<test_case_t> test_cases{
        // Unary Arithmetic
        {"+5",5},
        {"-6",-6},
        // Integer Arithmetic
        {"3+10",13},
        {"16/4",4},
        {"12-20",-8},
        {"4*2",8},
        // Floating Point Arithmetic
        {"1.5+2.2",3.7},
        {"5/2",2.5},
        {"100.0 - 50.5",49.5},
        {"4*2.2",8.8},
        // Parenthesis
        {"(3+5)*2", 16},
        {"((3+5)*(1+1))/2", 8}
    };

    DiceParser parser;
    for(auto test_case : test_cases)
    {
        auto parse_ans = std::get<double>(parser.parse(test_case.test_str));
        EXPECT_EQ(parse_ans, test_case.ans) << test_case.test_str;
    }
}

TEST(valid, dice_stats)
{
    std::vector<test_case_t> test_cases{
        // Basic Dice Formulas
        {"stats(3d6)", 3},
        {"stats(adv)", 1},
        {"stats(dis)", 1},
        {"stats(max(3d4))", 1},
        {"stats(min(3d8))", 1},
        {"stats(max(2,3d10))", 2},
        {"stats(min(2,4d12))", 2},
    };

    DiceParser parser;
    for(auto test_case : test_cases)
    {
        auto parse_ans = std::get<DiceDistr>(parser.parse(test_case.test_str)).minimum();
        EXPECT_EQ(parse_ans, test_case.ans) << test_case.test_str;
    }
}

TEST(valid, dice_comparisons)
{
    std::vector<test_case_t> test_cases{
        // Basic Dice Formulas
        {"1d4 == 1", 0.25},
        {"1d4 != 1", 0.75},
        {"1d4 >= 1", 1.00},
        {"1d4 <= 1", 0.25},
        {"1d4 > 1", 0.75},
        {"1d4 < 1", 0.00},
        {"1 == 1d4", 0.25},
        {"1 != 1d4", 0.75},
        {"1 >= 1d4", 0.25},
        {"1 <= 1d4", 1.00},
        {"1 > 1d4", 0.00},
        {"1 < 1d4", 0.75},
    };

    DiceParser parser;
    for(auto test_case : test_cases)
    {
        auto parse_ans = std::get<double>(parser.parse(test_case.test_str));
        EXPECT_EQ(parse_ans, test_case.ans) << test_case.test_str;
    }
}

TEST(valid, dice_rolls)
{
    std::vector<test_case_t> test_cases{
        // Basic Die Rolls
        {"1d1", 1},
        {"max(2d1)", 1},
        {"min(3d1)", 1},
        {"max(2,3d1)", 2},
        {"min(3,4d1)", 3},
        // Basic Dice Arithmetic
        {"+2d1", 2},
        {"-2d1", -2},
        {"2d1 - 1d1", 1},
        {"2d1 + 1d1", 3},
        {"(2d1 - 1d1) + 1d1", 2},
        {"4d1 - 1", 3},
        {"4d1 + 1", 5},
        {"4d1*2", 8},
        {"6d1 - 1", 5},
        {"6d1 + 1", 7},
        {"6d1*2", 12},
    };

    DiceParser parser;
    for(auto test_case : test_cases)
    {
        auto parse_ans = std::get<double>(parser.parse(test_case.test_str));
        EXPECT_EQ(parse_ans, test_case.ans) << test_case.test_str;
    }
}

TEST(invalid, error_codes)
{
    std::vector<err_case_t> error_cases{
        // Invaliid tokens
        {"~2d6", err_code::unknown_symbol},
        {"7 6 -", err_code::invalid_syntax},
    };

    DiceParser parser;
    for(auto error_case : error_cases)
    {
        auto parse_ans = std::get<err_code>(parser.parse(error_case.test_str));
        EXPECT_EQ(parse_ans, error_case.ans) << error_case.test_str;
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    if (RUN_ALL_TESTS())
        ;

    return 0;
}
