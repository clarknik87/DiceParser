#include <map>
#include <algorithm>
#include <string>
#include <cmath>

#include "builtin_func.hpp"

namespace builtin
{

using one_arg_func_ptr = double (*)(double);
using two_arg_func_ptr = double (*)(double, double);

auto one_arg_funcs = std::map<std::string, one_arg_func_ptr>({
    {"abs", fabs},
    {"sqrt", sqrt},
    {"ceil", ceil},
    {"floor", floor},
    {"trunc", trunc},
    {"round", round}
});

auto two_arg_funcs = std::map<std::string, two_arg_func_ptr>({
    {"pow", pow}
});

bool search_one_arg_func(const std::string& funcname)
{
    return one_arg_funcs.contains(funcname);
}
bool search_two_arg_func(const std::string& funcname)
{
    return two_arg_funcs.contains(funcname);
}

double call_one_arg_func(const std::string& funcname, double arg1)
{
    return one_arg_funcs[funcname](arg1);
}
double call_two_arg_func(const std::string& funcname, double arg1, double arg2)
{
    return two_arg_funcs[funcname](arg1, arg2);
}

}