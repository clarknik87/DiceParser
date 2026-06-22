#include "variable_map.hpp"
#include "dice_parser/action_code.hpp"

VariableMap::VariableMap(
        calc::Scanner *p_scanner,
        calc::Parser *p_parser
    ) : scanner(p_scanner), parser(p_parser) {}

VariableMap::VariableMap(
        calc::Scanner *p_scanner,
        calc::Parser *p_parser,
        std::initializer_list<std::pair<const std::string, const std::string>> constants,
        std::initializer_list<std::pair<const std::string, const std::string>> variables
    ) : scanner(p_scanner), parser(p_parser) {}

void VariableMap::add_variable(const std::string& key, std::variant<double, DiceDistr> val, std::string expr)
{
    if((check_num_variable(key) || check_dice_variable(key)) && var_list[key].is_const)
        throw action_code::const_assignment_err;
    var_list.erase(key);
    var_list[key] = UserVar(
        val,
        expr,
        false,
        std::vector<std::string>()
    );
}

double VariableMap::get_num_variable(const std::string& key)
{
    if(!check_num_variable(key))
        throw action_code::variable_undefined;
    return std::get<double>(var_list.at(key).value);
}

DiceDistr VariableMap::get_dice_variable(const std::string& key)
{
    if(!check_dice_variable(key))
        throw action_code::variable_undefined;
    return std::get<DiceDistr>(var_list.at(key).value);
}

bool VariableMap::check_num_variable(const std::string& key)
{
    return (var_list.find(key) != var_list.end()) && (std::holds_alternative<double>(var_list[key].value));
}

bool VariableMap::check_dice_variable(const std::string& key)
{
    return (var_list.find(key) != var_list.end()) && (std::holds_alternative<DiceDistr>(var_list[key].value));
}

// std::map<std::string, double> VariableMap::get_num_const_map()
// {
//     return const_num_list;
// }

// std::map<std::string, DiceDistr> VariableMap::get_dice_const_map()
// {
//     return const_dice_list;
// }

// std::map<std::string, double> VariableMap::get_num_var_map()
// {
//     return num_list;
// }

// std::map<std::string, DiceDistr> VariableMap::get_dice_var_map()
// {
//     return dice_list;
// }
