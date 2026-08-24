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

TEST(preprocess, valid)
{
    DiceParser parser({
        {"dex", "3"},
        {"bardic", "1d4"},
    });

    // Assert that variables are loaded correctly
    ASSERT_EQ(std::get<double>(parser.parse("dex")), 3.0);
    ASSERT_EQ(std::get<DiceDistr>(parser.parse("bardic")).expected_value(), 2.5);

    // Verify that using {} allows use of dereferenced values
    EXPECT_EQ(std::get<action_code>(parser.parse("A = dex")), action_code::action_success);
    EXPECT_EQ(std::get<action_code>(parser.parse("B = {dex}")), action_code::action_success);
    EXPECT_EQ(std::get<action_code>(parser.parse("dex = 4")), action_code::action_success);
    EXPECT_EQ(std::get<double>(parser.parse("A")), 4.0);
    EXPECT_EQ(std::get<double>(parser.parse("B")), 3.0);

    EXPECT_EQ(std::get<action_code>(parser.parse("A = bardic")), action_code::action_success);
    EXPECT_EQ(std::get<action_code>(parser.parse("B = {bardic}")), action_code::action_success);
    EXPECT_EQ(std::get<action_code>(parser.parse("bardic = 1d6")), action_code::action_success);
    EXPECT_EQ(std::get<DiceDistr>(parser.parse("A")).maximum(), 6.0);
    EXPECT_EQ(std::get<DiceDistr>(parser.parse("B")).maximum(), 4.0);
}

TEST(preprocess, invalid)
{
    DiceParser parser;
    EXPECT_EQ(std::get<action_code>(parser.parse("{dex}")), action_code::variable_undefined);
    EXPECT_EQ(std::get<action_code>(parser.parse("{bardic}")), action_code::variable_undefined);
}
