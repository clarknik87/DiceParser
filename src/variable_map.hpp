#ifndef VARIABLE_MAP_HPP_GUARD
#define VARIABLE_MAP_HPP_GUARD

#include <utility>
#include <vector>
#include <map>
#include <string>
#include "dice_distribution.hpp"

// scanner and parser forward declarations
namespace calc {
    class Scanner;
}
class DiceParser;

struct UserVar
{
    DiceDistr                   value;
    std::string                 expr;
    std::vector<std::string>    dependencies;
};

using expr_pair = std::pair<const std::string, const std::string>;
using expr_list = std::vector<expr_pair>;

class VariableMap
{
private:
    std::map<std::string, UserVar> var_list;
    calc::Scanner &scanner;
    DiceParser &parser;

    void add_node(const std::string& key, DiceDistr val, const std::string& expr);
    std::vector<std::string> lex_dependencies(const std::string& expr) const;
    std::vector<std::string> topological_sort(std::map<std::string, std::vector<std::string>> list) const;
    bool check_cycles() const;

public:
    VariableMap() = delete;
    VariableMap(
        calc::Scanner &p_scanner,
        DiceParser &p_parser
    );
    VariableMap(
        calc::Scanner &p_scanner,
        DiceParser &p_parser,
        const VariableMap& p_map
    );
    VariableMap(
        calc::Scanner &p_scanner,
        DiceParser &p_parser,
        expr_list& variables
    );

    DiceDistr   at(const std::string& key) const;
    bool        contains(const std::string& key) const;

    void add_variable(const std::string& key, DiceDistr val, const std::string& expr);
    void delete_variable(const std::string& key);

    std::map<std::string, std::string> get_var_map() const;
};

#endif//VARIABLE_MAP_HPP_GUARD
