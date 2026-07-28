#ifndef UNIT_TEST_HPP_GUARD
#define UNIT_TEST_HPP_GUARD

#include <string>
#include "dice_parser/dice_parser.hpp"

constexpr double float_epsilon{0.001};

struct test_case_t
{
    std::string test_str;
    double      ans;
};

struct interpolate_case_t
{
    std::string test;
    std::string ans;
};

struct err_case_t
{
    std::string test_str;
    action_code    ans;
};

#endif//UNIT_TEST_HPP_GUARD
