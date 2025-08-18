#ifndef ACTION_CODE_HPP_GUARD
#define ACTION_CODE_HPP_GUARD

#include <ostream>

enum class action_code
{
    action_success,
    unknown_symbol,
    invalid_syntax,
    variable_undefined,
    constant_undefined,
    const_assignment_err,
};

inline std::ostream& operator<<(std::ostream& os, action_code& code)
{
    switch(code)
    {
    case action_code::action_success:
        os << "action_code::action_success";
        break;
    case action_code::unknown_symbol:
        os << "action_code::unknown_symbol";
        break;
    case action_code::invalid_syntax:
        os << "action_code::invalid_syntax";
        break;
    case action_code::variable_undefined:
        os << "action_code::variable_undefined";
        break;
    case action_code::constant_undefined:
        os << "action_code::constant_undefined";
        break;
    case action_code::const_assignment_err:
        os << "action_code::const_assignment_err";
        break;
    }
    return os;
}

#endif//ACTION_CODE_HPP_GUARD
