#ifndef VARIABLE_MAP_HPP_GUARD
#define VARIABLE_MAP_HPP_GUARD

#include <utility>
#include <vector>
#include <map>
#include <string>
#include <variant>
#include "dice_distribution.hpp"

// scanner and parser forward declarations
namespace calc {
    class Scanner;
}
class DiceParser;

struct UserVar
{
    std::variant<double, DiceDistr> value{0.0};
    std::string                     expr;
    bool                            is_const{false};
    std::vector<std::string>        dependencies;
};

using expr_pair = std::pair<const std::string, const std::string>;
using expr_list = std::vector<expr_pair>;

class VariableMap
{
private:
    std::map<std::string, UserVar> var_list;
    calc::Scanner &scanner;
    DiceParser &parser;

    void add_node(const std::string& key, std::variant<double, DiceDistr> val, const std::string& expr, bool is_const);
    bool check_key(const std::string& key) const;
    std::vector<std::string> lex_dependencies(const std::string& expr) const;

public:
    VariableMap() = delete;
    VariableMap(
        calc::Scanner &p_scanner,
        DiceParser &p_parser
    );
    VariableMap(
        calc::Scanner &p_scanner,
        DiceParser &p_parser,
        expr_list& constants,
        expr_list& variables
    );

    
    double      get_num_variable(const std::string& key) const;
    DiceDistr   get_dice_variable(const std::string& key) const;
    bool        check_num_variable(const std::string& key) const;
    bool        check_dice_variable(const std::string& key) const;

    void add_constant(const std::string& key, std::variant<double, DiceDistr> val, const std::string& expr);
    void add_variable(const std::string& key, std::variant<double, DiceDistr> val, const std::string& expr);

    std::map<std::string, std::string> get_const_map() const;
    std::map<std::string, std::string> get_var_map() const;

    std::vector<std::string> topological_sort(std::map<std::string, std::vector<std::string>> list) const;
};

#endif//VARIABLE_MAP_HPP_GUARD
