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
    {"abs", &DiceDistr::abs},
    {"sqrt", &DiceDistr::sqrt},
    {"ceil", &DiceDistr::ceil},
    {"floor", &DiceDistr::floor},
    {"trunc", &DiceDistr::trunc},
    {"round", &DiceDistr::round}
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