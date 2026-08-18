#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <gtest/gtest.h>

#include "unit_test.hpp"
#include "dice_parser/dice_parser.hpp"

TEST(DiceDistr, valid_ctor)
{
    std::vector<test_case_t> test_cases{
        // integer scalars
        {"1", 1.0},
        {"2", 2.0},
        {"20", 20.0},
        {"100", 100.0},
        {"0", 0.0},
        // double scalars
        {"1.0", 1.0},
        {"1.5", 1.5},
        {"0.3", 0.3},
        {"15.4", 15.4},
        {"0.0", 0.0},
        // NdS format
        {"1d1", 1.0},
        {"2d1", 2.0},
        {"1d6", 3.5},
        {"1d10", 5.5},
        {"1d12", 6.5},
        {"1d20", 10.5},
        {"2d4", 5.0},
        {"3d6", 10.5},
        // adv/dis format
        {"adv", 13.825},
        {"dis", 7.175},
        // min(NdS) format
        {"min(3d1)", 1.0},
        {"min(2d20)", 7.175},
        {"min(2d2)", 1.25},
        {"min(3d6)", 2.0417},
        // max(NdS) format
        {"max(3d1)", 1.0},
        {"max(2d20)", 13.825},
        {"max(2d2)", 1.75},
        {"max(2d6)", 4.472},
        // min(X, NdS)
        {"min(3,4d1)", 3.0},
        {"min(2,3d6)", 5.542},
        {"min(2,4d2)", 2.375},
        {"min(2,2d10)", 11.0},
        // max(X,NdS)
        {"max(2,3d1)", 2.0},
        {"max(2,3d6)", 8.458},
        {"max(2,4d2)", 3.625},
        {"max(2,2d10)", 11.0},
    };

    DiceParser parser;
    for(auto test_case : test_cases)
    {
        auto parse_ans = parser.parse(test_case.test_str);
        if(std::holds_alternative<double>(parse_ans))
            EXPECT_NEAR(std::get<double>(parse_ans), test_case.ans, float_epsilon) << test_case.test_str;
        else if(std::holds_alternative<DiceDistr>(parse_ans))
            EXPECT_NEAR(std::get<DiceDistr>(parse_ans).expected_value(), test_case.ans, float_epsilon) << test_case.test_str;
        else
            FAIL() << test_case.test_str;
    }
}

TEST(DiceDistr, invalid_ctor)
{
        std::vector<err_case_t> error_cases{
        // NdS format
        {"0d4", action_code::invalid_formula},
        {"4d0", action_code::invalid_formula},
        // adv/dis format
        // min(NdS) format
        {"min(3d0)", action_code::invalid_formula},
        {"min(0d6)", action_code::invalid_formula},
        // max(NdS) format
        {"max(3d0)", action_code::invalid_formula},
        {"max(0d1)", action_code::invalid_formula},
        // min(X, NdS)
        {"min(0,4d1)", action_code::invalid_formula},
        {"min(1,0d1)", action_code::invalid_formula},
        {"min(3,4d0)", action_code::invalid_formula},
        {"min(2,1d10)", action_code::invalid_formula},
        // max(X,NdS)
        {"max(0,4d1)", action_code::invalid_formula},
        {"max(1,0d1)", action_code::invalid_formula},
        {"max(3,4d0)", action_code::invalid_formula},
        {"max(2,1d10)", action_code::invalid_formula},

    };

    DiceParser parser;
    for(auto error_case : error_cases)
    {
        auto parse_ans = std::get<action_code>(parser.parse(error_case.test_str));
        EXPECT_EQ(parse_ans, error_case.ans) << error_case.test_str;
    }
}

TEST(DiceDistr, dice_operators)
{
    std::vector<test_case_t> test_cases{
        // unary plus
        {"+1", 1.0},
        {"+2d4", 5.0},
        // unary minus
        {"-1", -1.0},
        {"-1d4", -2.5},
        // addition
        {"1d2 + 1", 2.5},
        {"1d2 + 1d4", 4.0},
        {"1d2 + 2d4", 6.5},
        {"1.0 + 1d4", 3.5},
        // subtraction
        {"1d2 - 1", 0.5},
        {"1d2 - 1d4", -1.0},
        {"1d2 - 2d4", -3.5},
        {"1.0 - 1d4", -1.5},
    };

    DiceParser parser;
    for(auto test_case : test_cases)
    {
        auto parse_ans = parser.parse(test_case.test_str);
        if(std::holds_alternative<double>(parse_ans))
            EXPECT_NEAR(std::get<double>(parse_ans), test_case.ans, float_epsilon) << test_case.test_str;
        else if(std::holds_alternative<DiceDistr>(parse_ans))
            EXPECT_NEAR(std::get<DiceDistr>(parse_ans).expected_value(), test_case.ans, float_epsilon) << test_case.test_str;
        else
            FAIL() << test_case.test_str;
    }
}

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
        auto parse_ans = parser.parse(test_case.test_str);
        if(std::holds_alternative<double>(parse_ans))
            EXPECT_EQ(std::get<double>(parse_ans), test_case.ans) << test_case.test_str;
        else
            FAIL() << test_case.test_str;
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
        auto parse_ans = parser.parse(test_case.test_str);
        if(std::holds_alternative<double>(parse_ans))
            EXPECT_NEAR(std::get<double>(parse_ans), test_case.ans, float_epsilon) << test_case.test_str;
        else if(std::holds_alternative<DiceDistr>(parse_ans))
            EXPECT_NEAR(std::get<DiceDistr>(parse_ans).expected_value(), test_case.ans, float_epsilon) << test_case.test_str;
        else
            FAIL() << test_case.test_str;
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
        // Both sides
        {"1d1 == 1d4", 0.25},
        {"1d1 != 1d4", 0.75},
        {"1d1 >= 1d4", 0.25},
        {"1d1 <= 1d4", 1.00},
        {"1d1 > 1d4", 0.00},
        {"1d1 < 1d4", 0.75},
        // Used arithmetically (wierd but works)
        {"(1 == 1d4)+1.0", 1.25},
        {"(1 != 1d4)+1.0", 1.75},
        {"(1 >= 1d4)+1.0", 1.25},
        {"(1 <= 1d4)+1.0", 2.00},
        {"(1 > 1d4)+1.0", 1.00},
        {"(1 < 1d4)+1.0", 1.75},
    };

    DiceParser parser;
    for(auto test_case : test_cases)
    {
        auto parse_ans = parser.parse(test_case.test_str);
        if(std::holds_alternative<double>(parse_ans))
            EXPECT_EQ(std::get<double>(parse_ans), test_case.ans) << test_case.test_str;
        else
            FAIL() << test_case.test_str;
    }
}

// TEST(builtin, basics)
// {
//     std::vector<test_case_t> test_cases{
//         {"abs(-5)", 5.0},
//         {"abs(5)", 5.0},
//         {"abs(-5.4)", 5.4},
//         {"abs(4.2)", 4.2},
//         {"sqrt(4)", sqrt(4)},
//         {"sqrt(54.7)", sqrt(54.7)},
//         {"sqrt(100)", sqrt(100)},
//         {"sqrt(0.005678)", sqrt(0.005678)},
//         {"ceil(1.0)", 1},
//         {"ceil(1.1)", 2},
//         {"ceil(-1.1)", -1},
//         {"floor(1.0)", 1},
//         {"floor(1.1)", 1},
//         {"floor(1.9)", 1},
//         {"floor(-1.9)", -2},
//         {"trunc(1.0)", 1},
//         {"trunc(-1.1)", -1},
//         {"trunc(-1.9)", -1},
//         {"trunc(1.9)", 1},
//         {"trunc(1.1)", 1},
//         {"round(1.0)", 1},
//         {"round(1.1)", 1},
//         {"round(1.9)", 2},
//         {"round(-1.0)", -1},
//         {"round(-1.1)", -1},
//         {"round(-1.9)", -2},
//         {"pow(10,1)", 10},
//         {"pow(10,2)", 100},
//         {"pow(10,3)", 1000},
//     };

//     DiceParser parser;
//     for(auto test_case : test_cases)
//         EXPECT_EQ(std::get<double>(parser.parse(test_case.test_str)), test_case.ans);
// }

// TEST(variable_map, builtin_funcs)
// {
//     DiceParser parser(
//         {},
//         {
//             {"str_score", "9"},
//             {"dex_score", "17"},
//             {"con_score", "20"},
//             {"str", "floor((str_score-10)/2)"},
//             {"dex", "floor((dex_score-10)/2)"},
//             {"con", "floor((con_score-10)/2)"},
//         }        
//     );
//     EXPECT_EQ(std::get<double>(parser.parse("str")), -1);
//     EXPECT_EQ(std::get<double>(parser.parse("dex")), 3);
//     EXPECT_EQ(std::get<double>(parser.parse("con")), 5);
// }
