#ifndef BUILTIN_FUNC_HPP
#define BUILTIN_FUNC_HPP

#include <string>
#include "dice_distribution.hpp"

namespace builtin
{

bool search_one_arg_func(const std::string& funcname);
bool search_two_arg_func(const std::string& funcname);

DiceDistr call_one_arg_func(const std::string& funcname, const DiceDistr& arg1);
DiceDistr call_two_arg_func(const std::string& funcname, const DiceDistr& arg1, const DiceDistr& arg2);

}

#endif//BUILTIN_FUNC_HPP
