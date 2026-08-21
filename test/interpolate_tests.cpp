#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <gtest/gtest.h>

#include "unit_test.hpp"
#include "dice_parser/dice_parser.hpp"

TEST(interpolate, valid)
{
    constexpr int dex{3};
    constexpr int prof{2};

    std::vector<interpolate_case_t> test_cases{
        {"{}", ""},
        {"{dex}", std::to_string(dex)},
        {"{prof}", std::to_string(prof)},
        {"{7+3}", std::to_string(10)},
        {"1d20+{prof}", "1d20+" + std::to_string(prof)},
        {"prof", "prof"},
        {"1d8+{dex+prof}", "1d8+" + std::to_string(dex+prof)},
        {"2d6+{dex+4}", "2d6+" + std::to_string(dex+4)},
        {"{dex}+1d20+{prof}", std::to_string(dex)+"+1d20+"+std::to_string(prof)},
        {"{}{}{}{}{}dex", "dex"},
    };

    DiceParser parser({
        {"dex", std::to_string(dex)},
        {"prof", std::to_string(prof)}
    });
    for(auto test_case: test_cases)
    {
        EXPECT_EQ(parser.interpolate(test_case.test), test_case.ans);
    }
}

TEST(interpolate, invalid)
{
    std::vector<interpolate_case_t> test_cases{
        {"{dex}", ""},
        {"{prof}", ""},
        {"1d20+{prof}", "1d20+"},
        {"1d8+{dex+prof}", "1d8+"},
        {"2d6+{dex+4}", "2d6+"},
        {"{dex}+1d20+{prof}", "+1d20+"},
        {"1d20+{dex", "1d20+{dex"},
        {"1d20+dex}", "1d20+dex}"},
        {"1d20+{{7+4}", "1d20+"},
        {"{{dex}}", "}"}
    };

    DiceParser parser;
    for(auto test_case: test_cases)
    {
        EXPECT_EQ(parser.interpolate(test_case.test), test_case.ans);
    }
}
