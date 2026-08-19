#include <map>
#include <algorithm>
#include <string>
#include <cmath>

#include "builtin_func.hpp"

namespace builtin
{

using one_arg_func_ptr = DiceDistr (DiceDistr::*)() const;
using two_arg_func_ptr = DiceDistr (DiceDistr::*)(const DiceDistr&) const;

auto one_arg_funcs = std::map<std::string, one_arg_func_ptr>({
    // cmath functions
    {"abs", &DiceDistr::abs},
    {"sqrt", &DiceDistr::sqrt},
    {"ceil", &DiceDistr::ceil},
    {"floor", &DiceDistr::floor},
    {"trunc", &DiceDistr::trunc},
    {"round", &DiceDistr::round},
    // statistics functions
    {"roll", &DiceDistr::roll_as_distr},
    {"minimum", &DiceDistr::minimum_as_distr},
    {"maximum", &DiceDistr::maximum_as_distr},
    {"expected", &DiceDistr::expected_value_as_distr},
    {"variance", &DiceDistr::variance_as_distr},
    {"std_dev", &DiceDistr::standard_dev_as_distr},
});

auto two_arg_funcs = std::map<std::string, two_arg_func_ptr>({
    {"pow", &DiceDistr::pow}
});

bool search_one_arg_func(const std::string& funcname)
{
    return one_arg_funcs.contains(funcname);
}
bool search_two_arg_func(const std::string& funcname)
{
    return two_arg_funcs.contains(funcname);
}

DiceDistr call_one_arg_func(const std::string& funcname, const DiceDistr& arg1)
{
    return (arg1.*(one_arg_funcs[funcname]))();
}
DiceDistr call_two_arg_func(const std::string& funcname, const DiceDistr& arg1, const DiceDistr& arg2)
{
    return (arg1.*(two_arg_funcs[funcname]))(arg2);
}

}