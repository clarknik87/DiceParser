#ifndef ACTION_CODE_HPP_GUARD
#define ACTION_CODE_HPP_GUARD

#include <ostream>

enum class action_code
{
    action_success,
    empty_command,
    unknown_symbol,
    invalid_formula,
    invalid_syntax,
    variable_undefined,
    delete_dependency_err,
    cyclic_graph_err,
};

inline std::ostream& operator<<(std::ostream& os, action_code& code)
{
    switch(code)
    {
    case action_code::action_success:
        os << "action_code::action_success";
        break;
    case action_code::empty_command:
        os << "action_code::empty_command";
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
    case action_code::delete_dependency_err:
        os << "action_code::delete_dependency_err";
        break;
    case action_code::cyclic_graph_err:
        os << "action_code::cyclic_graph_err";
        break;
    default:
        os << "action_code:: unknown code returned";
        break;
    }
    return os;
}

#endif//ACTION_CODE_HPP_GUARD
