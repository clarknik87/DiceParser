#ifndef VARIABLE_MAP_HPP_GUARD
#define VARIABLE_MAP_HPP_GUARD

#include <vector>
#include <map>
#include <string>
#include <variant>
#include "dice_distribution.hpp"

// scanner and parser forward declares
namespace calc {
    class Scanner;
    class Parser;
}

struct UserVar
{
    std::variant<double, DiceDistr> value{0.0};
    std::string                     expr;
    bool                            is_const{false};
    std::vector<std::string>        dependencies;
};

class VariableMap
{
private:
    std::map<std::string, UserVar> var_list;
    calc::Scanner *scanner;
    calc::Parser *parser;

    void add_node(const std::string& key, std::variant<double, DiceDistr> val, const std::string& expr, bool is_const);
    bool check_key(const std::string& key);

public:
    VariableMap() = delete;
    VariableMap(
        calc::Scanner *p_scanner,
        calc::Parser *p_parser
    );
    VariableMap(
        calc::Scanner *p_scanner,
        calc::Parser *p_parser,
        std::initializer_list<std::pair<const std::string, const std::string>> constants,
        std::initializer_list<std::pair<const std::string, const std::string>> variables
    );

    
    double      get_num_variable(const std::string& key);
    DiceDistr   get_dice_variable(const std::string& key);
    bool        check_num_variable(const std::string& key);
    bool        check_dice_variable(const std::string& key);

    void add_constant(const std::string& key, std::variant<double, DiceDistr> val, const std::string& expr);
    void add_variable(const std::string& key, std::variant<double, DiceDistr> val, const std::string& expr);

    std::map<std::string, std::string> get_const_map();
    std::map<std::string, std::string> get_var_map();
};

#endif//VARIABLE_MAP_HPP_GUARD
