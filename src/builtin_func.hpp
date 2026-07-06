#ifndef BUILTIN_FUNC_HPP
#define BUILTIN_FUNC_HPP

#include <string>

namespace builtin
{

bool search_one_arg_func(const std::string& funcname);
bool search_two_arg_func(const std::string& funcname);

double call_one_arg_func(const std::string& funcname, double arg1);
double call_two_arg_func(const std::string& funcname, double arg1, double arg2);

}

#endif//BUILTIN_FUNC_HPP
