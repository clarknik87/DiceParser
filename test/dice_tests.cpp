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
        auto parse_ans = std::get<DiceDistr>(parser.parse(test_case.test_str));
        EXPECT_NEAR(parse_ans.expected_value(), test_case.ans, float_epsilon) << test_case.test_str;
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
