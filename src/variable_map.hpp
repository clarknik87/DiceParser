#ifndef VARIABLE_MAP_HPP_GUARD
#define VARIABLE_MAP_HPP_GUARD

#include <vector>
#include <map>
#include <string>
#include "dice_distribution.hpp"

class VariableMap
{
private:
    std::map<std::string, double> const_num_list;
    std::map<std::string, DiceDistr> const_dice_list;
    std::map<std::string, double> num_list;
    std::map<std::string, DiceDistr> dice_list;
public:
    VariableMap() = default;
    VariableMap(
        std::initializer_list<std::pair<const std::string, double>> cnum_variables,
        std::initializer_list<std::pair<const std::string, double>> num_variables,
        std::initializer_list<std::pair<const std::string, DiceDistr>> cdice_variables,
        std::initializer_list<std::pair<const std::string, DiceDistr>> dice_variables
    );
    VariableMap(
        std::map<std::string, double> cnum_variables,
        std::map<std::string, DiceDistr> cdice_variables,
        std::map<std::string, double> num_variables,
        std::map<std::string, DiceDistr> dice_variables
    );

    void        add_num_variable(const std::string& key, double val);
    void        add_dice_variable(const std::string& key, DiceDistr val);
    double      get_num_constant(const std::string& key);
    double      get_num_variable(const std::string& key);
    DiceDistr   get_dice_constant(const std::string& key);
    DiceDistr   get_dice_variable(const std::string& key);
    bool        check_num_constant(const std::string& key);
    bool        check_num_variable(const std::string& key);
    bool        check_dice_constant(const std::string& key);
    bool        check_dice_variable(const std::string& key);

    std::map<std::string, double>       get_num_const_map();
    std::map<std::string, DiceDistr>    get_dice_const_map();
    std::map<std::string, double>       get_num_var_map();
    std::map<std::string, DiceDistr>    get_dice_var_map();
};

#endif//VARIABLE_MAP_HPP_GUARD
