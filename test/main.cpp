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

// TEST(var_map, ctor)
// {
//     std::vector<std::pair<const std::string, const std::string>> constants{
//         {"A", "0"}
//     };
//     std::vector<std::pair<const std::string, const std::string>> variables{
//         {"C", "A + B"},
//         {"E", "1"},
//         {"B", "D"},
//         {"D", "6"}
//     };
//     DiceParser parser(
//         constants,
//         variables
//     );
//     // Verify all variables were parsed correctly
//     EXPECT_EQ(std::get<double>(parser.parse("A")), 0.0);
//     EXPECT_EQ(std::get<double>(parser.parse("B")), 6.0);
//     EXPECT_EQ(std::get<double>(parser.parse("C")), 6.0);
//     EXPECT_EQ(std::get<double>(parser.parse("D")), 6.0);
//     EXPECT_EQ(std::get<double>(parser.parse("E")), 1.0);
//     // Verify constants and variables were not mixed
//     EXPECT_NO_THROW(parser.get_variable_map().get_const_map().at("A"));
//     EXPECT_NO_THROW(parser.get_variable_map().get_var_map().at("B"));
//     EXPECT_NO_THROW(parser.get_variable_map().get_var_map().at("C"));
//     EXPECT_NO_THROW(parser.get_variable_map().get_var_map().at("D"));
//     EXPECT_NO_THROW(parser.get_variable_map().get_var_map().at("E"));
//     EXPECT_ANY_THROW(parser.get_variable_map().get_var_map().at("A"));
//     EXPECT_ANY_THROW(parser.get_variable_map().get_const_map().at("B"));
//     EXPECT_ANY_THROW(parser.get_variable_map().get_const_map().at("C"));
//     EXPECT_ANY_THROW(parser.get_variable_map().get_const_map().at("D"));
//     EXPECT_ANY_THROW(parser.get_variable_map().get_const_map().at("E"));
// }

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

// TEST(variable_map, add_node)
// {
//     DiceParser parser;
    
//     // check that none of the symbols are defined in the parser
//     EXPECT_EQ(std::get<action_code>(parser.parse("var1")), action_code::invalid_syntax);
//     EXPECT_EQ(std::get<action_code>(parser.parse("const1")), action_code::invalid_syntax);
//     EXPECT_EQ(std::get<action_code>(parser.parse("cdice1")), action_code::invalid_syntax);
//     EXPECT_EQ(std::get<action_code>(parser.parse("dice1")), action_code::invalid_syntax);
    
//     auto& var_map = parser.get_variable_map();
//     var_map.add_constant("const1", 1.0, "1.0");
//     var_map.add_constant("cdice1", DiceDistr("3d1"), "3d1");
//     var_map.add_variable("var1", 1.0, "1.0");
//     var_map.add_variable("dice1", DiceDistr("3d1"), "3d1");

//     // check that the symbols are now defined
//     EXPECT_EQ(std::get<double>(parser.parse("var1")), 1.0);
//     EXPECT_EQ(std::get<double>(parser.parse("const1")), 1.0);
//     EXPECT_EQ(std::get<DiceDistr>(parser.parse("cdice1")).roll(), 3.0);
//     EXPECT_EQ(std::get<DiceDistr>(parser.parse("dice1")).roll(), 3.0);
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

// TEST(variable_map, interface_tests)
// {
//     DiceParser parser;
//     auto& var_map = parser.get_variable_map();
//     const std::string num_key = "num_key";
//     const double num_val = 8.0;
//     const std::string dice_key = "dice_key";
//     DiceDistr dice_val("5d1");

//     // check that the num_list and dice_lists are empty
//     ASSERT_FALSE(var_map.check_num_variable(num_key));
//     ASSERT_FALSE(var_map.check_num_variable(dice_key));
//     ASSERT_FALSE(var_map.check_dice_variable(dice_key));
//     ASSERT_FALSE(var_map.check_dice_variable(num_key));

//     // check add actions
//     var_map.add_variable(num_key, num_val, std::to_string(num_val));
//     ASSERT_TRUE(var_map.check_num_variable(num_key));
//     ASSERT_FALSE(var_map.check_dice_variable(num_key));
//     EXPECT_EQ(var_map.get_num_variable(num_key), num_val);

//     var_map.add_variable(dice_key, dice_val, dice_val.get_expr());
//     ASSERT_TRUE(var_map.check_dice_variable(dice_key));
//     ASSERT_FALSE(var_map.check_num_variable(dice_key));
//     EXPECT_EQ(var_map.get_dice_variable(dice_key).roll(), dice_val.roll());

//     // check reassignment of same type
//     var_map.add_variable(num_key, num_val+1, std::to_string(num_val+1));
//     ASSERT_TRUE(var_map.check_num_variable(num_key));
//     ASSERT_FALSE(var_map.check_dice_variable(num_key));
//     EXPECT_EQ(var_map.get_num_variable(num_key), num_val+1);

//     // check reassignment of different type
//     var_map.add_variable(dice_key, num_val-1, std::to_string(num_val-1));
//     ASSERT_TRUE(var_map.check_num_variable(dice_key));
//     EXPECT_EQ(var_map.get_num_variable(dice_key), num_val-1);
// }

// TEST(variable_map, ctor_tests)
// {
//     DiceParser parser(
//         {
//             {"var1", "1.0"},
//             {"var2", "2.0"},
//             {"dice1", "3d1"},
//             {"dice2", "4d1"}
//         },
//         {}
//     );
//     auto& var_map = parser.get_variable_map();
//     EXPECT_EQ(var_map.get_num_variable("var1"), 1.0);
//     EXPECT_EQ(var_map.get_num_variable("var2"), 2.0);
//     EXPECT_EQ(var_map.get_dice_variable("dice1").roll(), DiceDistr("3d1").roll());
//     EXPECT_EQ(var_map.get_dice_variable("dice2").roll(), DiceDistr("4d1").roll());
// }

// TEST(variable_map, update_dependencies)
// {
//     {
//         DiceParser parser(
//             {},
//             {
//                 {"A", "1"},
//                 {"B", "A+4"},
//             }
//         );
//         ASSERT_EQ(std::get<double>(parser.parse("A")), 1.0);
//         ASSERT_EQ(std::get<double>(parser.parse("B")), 5.0);
//         EXPECT_EQ(std::get<action_code>(parser.parse("A = 4")), action_code::action_success);
//         EXPECT_EQ(std::get<double>(parser.parse("B")), 8.0);
//     }
//     {
//         DiceParser parser(
//             {},
//             {
//                 {"A", "1"},
//                 {"B", "A+4"},
//                 {"C", "A+B"},
//                 {"D", "C-1"},
//             }
//         );
//         ASSERT_EQ(std::get<double>(parser.parse("A")), 1.0);
//         ASSERT_EQ(std::get<double>(parser.parse("B")), 5.0);
//         ASSERT_EQ(std::get<double>(parser.parse("C")), 6.0);
//         ASSERT_EQ(std::get<double>(parser.parse("D")), 5.0);
//         EXPECT_EQ(std::get<action_code>(parser.parse("A = 4")), action_code::action_success);
//         EXPECT_EQ(std::get<double>(parser.parse("A")), 4.0);
//         EXPECT_EQ(std::get<double>(parser.parse("B")), 8.0);
//         EXPECT_EQ(std::get<double>(parser.parse("C")), 12.0);
//         EXPECT_EQ(std::get<double>(parser.parse("D")), 11.0);
//     }
// }



int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    if (RUN_ALL_TESTS())
        ;

    return 0;
}
