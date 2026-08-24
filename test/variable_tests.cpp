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

TEST(var_map, ctor)
{
    DiceParser parser({
        {"C", "A + B"},
        {"E", "1"},
        {"B", "D"},
        {"D", "6"},
        {"A", "0"}
    });
    // Verify all variables were parsed correctly
    EXPECT_EQ(std::get<double>(parser.parse("A")), 0.0);
    EXPECT_EQ(std::get<double>(parser.parse("B")), 6.0);
    EXPECT_EQ(std::get<double>(parser.parse("C")), 6.0);
    EXPECT_EQ(std::get<double>(parser.parse("D")), 6.0);
    EXPECT_EQ(std::get<double>(parser.parse("E")), 1.0);
}

TEST(variable_map, add_node)
{
    DiceParser parser;
    
    // check that none of the symbols are defined in the parser
    EXPECT_EQ(std::get<action_code>(parser.parse("var1")), action_code::invalid_syntax);
    EXPECT_EQ(std::get<action_code>(parser.parse("const1")), action_code::invalid_syntax);
    EXPECT_EQ(std::get<action_code>(parser.parse("cdice1")), action_code::invalid_syntax);
    EXPECT_EQ(std::get<action_code>(parser.parse("dice1")), action_code::invalid_syntax);
    
    auto& var_map = parser.get_variable_map();
    var_map.add_variable("var1", DiceDistr("1.0"), "1.0");
    var_map.add_variable("dice1", DiceDistr("3d2"), "3d2");

    // check that the symbols are now defined
    EXPECT_EQ(std::get<double>(parser.parse("var1")), 1.0);
    EXPECT_EQ(std::get<DiceDistr>(parser.parse("dice1")).expected_value(), 4.5);
}

TEST(variable_map, interface_tests)
{
    DiceParser parser;
    auto& var_map = parser.get_variable_map();
    const std::string num_key = "num_key";
    const double num_val = 8.0;
    const std::string dice_key = "dice_key";
    DiceDistr dice_val("5d1");

    // check that the num_list and dice_lists are empty
    ASSERT_FALSE(var_map.contains(dice_key));
    ASSERT_FALSE(var_map.contains(num_key));

    // check add actions
    var_map.add_variable(num_key, num_val, std::to_string(num_val));
    ASSERT_TRUE(var_map.contains(num_key));
    EXPECT_EQ(var_map.at(num_key).get_scalar(), num_val);

    var_map.add_variable(dice_key, dice_val, dice_val.get_expr());
    ASSERT_TRUE(var_map.contains(dice_key));
    EXPECT_EQ(var_map.at(dice_key).get_scalar(), dice_val.get_scalar());

    // check reassignment
    var_map.add_variable(num_key, num_val+1, std::to_string(num_val+1));
    ASSERT_TRUE(var_map.contains(num_key));
    EXPECT_EQ(var_map.at(num_key).get_scalar(), num_val+1);

    var_map.add_variable(dice_key, num_val-1, std::to_string(num_val-1));
    ASSERT_TRUE(var_map.contains(dice_key));
    EXPECT_EQ(var_map.at(dice_key).get_scalar(), num_val-1);
}

TEST(variable_map, ctor_tests)
{
    DiceParser parser({
        {"var1", "1.0"},
        {"var2", "2.0"},
        {"dice1", "3d1"},
        {"dice2", "4d1"}
    });
    auto& var_map = parser.get_variable_map();
    EXPECT_EQ(var_map.at("var1"), 1.0);
    EXPECT_EQ(var_map.at("var2"), 2.0);
    EXPECT_EQ(var_map.at("dice1").roll(), DiceDistr("3d1").roll());
    EXPECT_EQ(var_map.at("dice2").roll(), DiceDistr("4d1").roll());
}

TEST(variable_map, update_dependencies)
{
    {
        DiceParser parser({
            {"A", "1"},
            {"B", "A+4"},
        });
        ASSERT_EQ(std::get<double>(parser.parse("A")), 1.0);
        ASSERT_EQ(std::get<double>(parser.parse("B")), 5.0);
        EXPECT_EQ(std::get<action_code>(parser.parse("A = 4")), action_code::action_success);
        EXPECT_EQ(std::get<double>(parser.parse("B")), 8.0);
    }
    {
        DiceParser parser({
            {"A", "1"},
            {"B", "A+4"},
            {"C", "A+B"},
            {"D", "C-1"},
        });
        ASSERT_EQ(std::get<double>(parser.parse("A")), 1.0);
        ASSERT_EQ(std::get<double>(parser.parse("B")), 5.0);
        ASSERT_EQ(std::get<double>(parser.parse("C")), 6.0);
        ASSERT_EQ(std::get<double>(parser.parse("D")), 5.0);
        EXPECT_EQ(std::get<action_code>(parser.parse("A = 4")), action_code::action_success);
        EXPECT_EQ(std::get<double>(parser.parse("A")), 4.0);
        EXPECT_EQ(std::get<double>(parser.parse("B")), 8.0);
        EXPECT_EQ(std::get<double>(parser.parse("C")), 12.0);
        EXPECT_EQ(std::get<double>(parser.parse("D")), 11.0);
    }
}

TEST(varaible_map, delete_tests)
{
   DiceParser parser;
   ASSERT_EQ(std::get<action_code>(parser.parse("A = 1")), action_code::action_success);
   ASSERT_EQ(std::get<action_code>(parser.parse("B = A+2")), action_code::action_success);
   ASSERT_EQ(std::get<action_code>(parser.parse("delete(A)")), action_code::delete_dependency_err);
   ASSERT_EQ(std::get<action_code>(parser.parse("delete(B)")), action_code::action_success);
   ASSERT_EQ(std::get<action_code>(parser.parse("delete(A)")), action_code::action_success);
}
