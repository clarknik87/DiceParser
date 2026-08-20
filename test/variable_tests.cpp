#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <gtest/gtest.h>

#include "unit_test.hpp"
#include "dice_parser/dice_parser.hpp"

TEST(valid, assignment)
{
    DiceParser parser;
    EXPECT_EQ(std::get<action_code>(parser.parse("myvar = 10")), action_code::action_success);
    EXPECT_EQ(std::get<double>(parser.parse("myvar")), 10.0);
    EXPECT_EQ(std::get<double>(parser.parse("myvar-5.0")), 5.0);
    EXPECT_EQ(std::get<action_code>(parser.parse("myvar = 20/5")), action_code::action_success);
    EXPECT_EQ(std::get<double>(parser.parse("myvar")), 4.0);
    EXPECT_EQ(std::get<action_code>(parser.parse("myvar = 3d1")), action_code::action_success);
    EXPECT_EQ(std::get<double>(parser.parse("myvar")), 3.0);
    EXPECT_EQ(std::get<action_code>(parser.parse("myvar = 12.0")), action_code::action_success);
    EXPECT_EQ(std::get<double>(parser.parse("myvar")), 12.0);
    EXPECT_EQ(std::get<action_code>(parser.parse("newvar = myvar+2.0")), action_code::action_success);
    EXPECT_EQ(std::get<double>(parser.parse("newvar")), 14.0);
    EXPECT_EQ(std::get<action_code>(parser.parse("myvar2 = 4d1")), action_code::action_success);
    EXPECT_EQ(std::get<double>(parser.parse("myvar2")), 4.0);
    EXPECT_EQ(std::get<action_code>(parser.parse("newvar2 = myvar2+2.0")), action_code::action_success);
    EXPECT_EQ(std::get<double>(parser.parse("newvar2")), 6.0);
}
