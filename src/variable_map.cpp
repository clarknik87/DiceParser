#include "variable_map.hpp"
#include "dice_parser/action_code.hpp"

VariableMap::VariableMap(
        std::initializer_list<std::pair<const std::string, double>> cnum_variables,
        std::initializer_list<std::pair<const std::string, double>> num_variables,
        std::initializer_list<std::pair<const std::string, DiceDistr>> cdice_variables,
        std::initializer_list<std::pair<const std::string, DiceDistr>> dice_variables
    ) : const_num_list(cnum_variables), num_list(num_variables), const_dice_list(cdice_variables) ,dice_list(dice_variables) {}

VariableMap::VariableMap(
        std::map<std::string, double> cnum_variables,
        std::map<std::string, DiceDistr> cdice_variables,
        std::map<std::string, double> num_variables,
        std::map<std::string, DiceDistr> dice_variables
    ) : const_num_list(cnum_variables), num_list(num_variables), const_dice_list(cdice_variables) ,dice_list(dice_variables) {}

void VariableMap::add_num_variable(const std::string& key, double val)
{
    if(check_num_constant(key))
        throw action_code::const_assignment_err;
    num_list.erase(key);
    dice_list.erase(key);
    num_list[key] = val;
}

void VariableMap::add_dice_variable(const std::string& key, DiceDistr val)
{
    if(check_dice_constant(key))
        throw action_code::const_assignment_err;
    num_list.erase(key);
    dice_list.erase(key);
    dice_list[key] = val;
}

double VariableMap::get_num_constant(const std::string& key)
{
    if(!check_num_constant(key))
        throw action_code::constant_undefined;
    return const_num_list.at(key);
}

double VariableMap::get_num_variable(const std::string& key)
{
    if(!check_num_variable(key))
        throw action_code::variable_undefined;
    return num_list.at(key);
}

DiceDistr VariableMap::get_dice_constant(const std::string& key)
{
    if(!check_dice_constant(key))
        throw action_code::constant_undefined;
    return const_dice_list.at(key);
}

DiceDistr VariableMap::get_dice_variable(const std::string& key)
{
    if(!check_dice_variable(key))
        throw action_code::variable_undefined;
    return dice_list.at(key);
}

bool VariableMap::check_num_constant(const std::string& key)
{
    return (const_num_list.find(key) != const_num_list.end());
}

bool VariableMap::check_num_variable(const std::string& key)
{
    return (num_list.find(key) != num_list.end());
}

bool VariableMap::check_dice_constant(const std::string& key)
{
    return (const_dice_list.find(key) != const_dice_list.end());
}

bool VariableMap::check_dice_variable(const std::string& key)
{
    return (dice_list.find(key) != dice_list.end());
}

std::map<std::string, double> VariableMap::get_num_const_map()
{
    return const_num_list;
}

std::map<std::string, DiceDistr> VariableMap::get_dice_const_map()
{
    return const_dice_list;
}

std::map<std::string, double> VariableMap::get_num_var_map()
{
    return num_list;
}

std::map<std::string, DiceDistr> VariableMap::get_dice_var_map()
{
    return dice_list;
}
