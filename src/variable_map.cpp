#include "variable_map.hpp"
#include "dice_parser/action_code.hpp"

void VariableMap::add_num_variable(const std::string& key, double val)
{
    num_list.erase(key);
    dice_list.erase(key);
    num_list[key] = val;
}

void VariableMap::add_dice_variable(const std::string& key, DiceDistr val)
{
    num_list.erase(key);
    dice_list.erase(key);
    dice_list[key] = val;
}

double VariableMap::get_num_variable(const std::string& key)
{
    if(!check_num_variable(key))
        throw action_code::variable_error;    
    return num_list.at(key);
}

DiceDistr VariableMap::get_dice_variable(const std::string& key)
{
    if(!check_dice_variable(key))
        throw action_code::variable_error;
    return dice_list.at(key);
}

bool VariableMap::check_num_variable(const std::string& key)
{
    return (num_list.find(key) != num_list.end());
}

bool VariableMap::check_dice_variable(const std::string& key)
{
    return (dice_list.find(key) != dice_list.end());
}
