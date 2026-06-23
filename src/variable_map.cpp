#include "variable_map.hpp"
#include "scanner.hpp"
#include "parser.hpp"
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

std::vector<std::string> VariableMap::lex_dependencies(const std::string& expr)
{
    std::vector<std::string> dependencies;
    if(scanner != nullptr)
    {
        scanner->set_input(expr);
        auto sym = scanner->lex();
        while(sym.kind() != calc::Parser::symbol_kind::S_YYEOF)
        {
            if(sym.kind() == calc::Parser::symbol_kind::S_DICE_VARIABLE 
            || sym.kind() == calc::Parser::symbol_kind::S_NUM_VARIABLE
            || sym.kind() == calc::Parser::symbol_kind::S_NEW_VARIABLE)
            {
                dependencies.emplace_back(scanner->YYText());
            }
            auto next_sym = scanner->lex();
            sym.move(next_sym);
        }
    }
    return dependencies;
}

void VariableMap::add_node(const std::string& key, std::variant<double, DiceDistr> val, const std::string& expr, bool is_const)
{
    if(check_key(key) && !is_const && var_list[key].is_const)
        throw action_code::const_assignment_err;
    var_list.erase(key);
    var_list[key] = UserVar(
        val,
        expr,
        is_const,
        lex_dependencies(expr)
    );
}

void VariableMap::add_constant(const std::string& key, std::variant<double, DiceDistr> val, const std::string& expr)
{
    add_node(key, val, expr, true);
}

void VariableMap::add_variable(const std::string& key, std::variant<double, DiceDistr> val, const std::string& expr)
{
    add_node(key, val, expr, false);
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

bool VariableMap::check_key(const std::string& key)
{
     return (var_list.find(key) != var_list.end());
}

bool VariableMap::check_num_variable(const std::string& key)
{
    return check_key(key) && (std::holds_alternative<double>(var_list[key].value));
}

bool VariableMap::check_dice_variable(const std::string& key)
{
    return check_key(key) && (std::holds_alternative<DiceDistr>(var_list[key].value));
}

std::map<std::string, std::string> VariableMap::get_const_map()
{
    std::map<std::string, std::string> const_map;
    for(auto it : var_list)
    {
        if(it.second.is_const)
            const_map[it.first] = it.second.expr;
    }
    return const_map;
}

std::map<std::string, std::string> VariableMap::get_var_map()
{
    std::map<std::string, std::string> var_map;
    for(auto it : var_list)
    {
        if(!it.second.is_const)
            var_map[it.first] = it.second.expr;
    }
    return var_map;
}
