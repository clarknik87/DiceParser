#include <algorithm>
#include <stdexcept>
#include <variant>
#include "variable_map.hpp"
#include "scanner.hpp"
#include "dice_parser/dice_parser.hpp"
#include "dice_parser/action_code.hpp"

VariableMap::VariableMap(
        calc::Scanner &p_scanner,
        DiceParser &p_parser
    ) : scanner(p_scanner), parser(p_parser) {}

VariableMap::VariableMap(
        calc::Scanner &p_scanner,
        DiceParser &p_parser,
        expr_list& constants,
        expr_list& variables
    ) : scanner(p_scanner), parser(p_parser)
{
    std::map<std::string, std::vector<std::string>> parse_list;
    for(auto c : constants)
        parse_list[c.first] = lex_dependencies(c.second);
    for(auto v : variables)
        parse_list[v.first] = lex_dependencies(v.second);

    auto parse_order = topological_sort(parse_list);
    for(auto key : parse_order)
    {
        auto it = std::find_if(constants.begin(), constants.end(), [&key](expr_pair& i){ return i.first == key; });
        if(it != constants.cend())
        {
            auto val = parser.parse(it->second);
            if(std::holds_alternative<double>(val))
                add_constant(it->first, std::get<double>(val), it->second);
            else if(std::holds_alternative<DiceDistr>(val))
                add_constant(it->first, std::get<DiceDistr>(val), it->second);
            else
                throw std::runtime_error(it->second + " does not parse to either double or DiceDistr");
            continue;
        }

        it = std::find_if(variables.begin(), variables.end(), [&key](expr_pair& i){ return i.first == key; });
        if(it != variables.cend())
        {
            auto val = parser.parse(it->second);
            if(std::holds_alternative<double>(val))
                add_variable(it->first, std::get<double>(val), it->second);
            else if(std::holds_alternative<DiceDistr>(val))
                add_variable(it->first, std::get<DiceDistr>(val), it->second);
            else
                throw std::runtime_error(it->second + " does not parse to either double or DiceDistr");
            continue;
        }
        //else key not found
        throw std::runtime_error(key + " not found in either constants nor variables");        
    }
}

std::vector<std::string> VariableMap::topological_sort(std::map<std::string, std::vector<std::string>> list) const
{
    /*
    Kahn's algorithm (modified from wikipedia)
    L ← Empty list that will contain the sorted elements
    S ← Set of all nodes with no dependencies

    while S is not empty do
        remove a node n from S
        add n to L
        for each node m with n as a dependency do
            remove edge e from the graph
            if m has no other dependencies then
                insert m into S

    if graph has edges then
        return error   (graph has at least one cycle)
    else 
        return L   (a topologically sorted order)
    */
    std::vector<std::string> L;
    std::vector<std::string> S;
    for(auto i : list)
        if(i.second.empty())
            S.push_back(i.first);
    
    while(!S.empty())
    {
        std::string n(std::move(S.back()));
        S.pop_back();
        L.push_back(n);

        for(auto& i: list)
        {
            if(std::find(i.second.cbegin(), i.second.cend(), n) != i.second.cend())
            {
                std::erase(i.second, n);
                if(i.second.empty())
                    S.push_back(i.first);
            }
        }
    }
    
    if(!std::all_of(list.cbegin(), list.cend(), [&](std::pair<const std::string, std::vector<std::string>> i){return i.second.empty();}))
        throw action_code::cyclic_graph_err;
    return L;
}

std::vector<std::string> VariableMap::lex_dependencies(const std::string& expr) const
{
    std::vector<std::string> dependencies;
    scanner.set_input(expr);
    auto sym = scanner.lex();
    while(sym.kind() != calc::Parser::symbol_kind::S_YYEOF)
    {
        if(sym.kind() == calc::Parser::symbol_kind::S_DICE_VARIABLE 
        || sym.kind() == calc::Parser::symbol_kind::S_NUM_VARIABLE
        || sym.kind() == calc::Parser::symbol_kind::S_NEW_VARIABLE)
        {
            dependencies.emplace_back(scanner.YYText());
        }
        auto next_sym = scanner.lex();
        sym.move(next_sym);
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
        {}
    );
    for(auto d : lex_dependencies(expr))
    {
        if(check_key(d))
            var_list[d].dependencies.emplace_back(key);
        else
            throw 0;
    }
}

void VariableMap::add_constant(const std::string& key, std::variant<double, DiceDistr> val, const std::string& expr)
{
    add_node(key, val, expr, true);
}

void VariableMap::add_variable(const std::string& key, std::variant<double, DiceDistr> val, const std::string& expr)
{
    add_node(key, val, expr, false);
}

double VariableMap::get_num_variable(const std::string& key) const
{
    if(!check_num_variable(key))
        throw action_code::variable_undefined;
    return std::get<double>(var_list.at(key).value);
}

DiceDistr VariableMap::get_dice_variable(const std::string& key) const
{
    if(!check_dice_variable(key))
        throw action_code::variable_undefined;
    return std::get<DiceDistr>(var_list.at(key).value);
}

bool VariableMap::check_key(const std::string& key) const
{
     return (var_list.find(key) != var_list.end());
}

bool VariableMap::check_num_variable(const std::string& key) const
{
    return check_key(key) && (std::holds_alternative<double>(var_list.at(key).value));
}

bool VariableMap::check_dice_variable(const std::string& key) const
{
    return check_key(key) && (std::holds_alternative<DiceDistr>(var_list.at(key).value));
}

std::map<std::string, std::string> VariableMap::get_const_map() const
{
    std::map<std::string, std::string> const_map;
    for(auto it : var_list)
    {
        if(it.second.is_const)
            const_map[it.first] = it.second.expr;
    }
    return const_map;
}

std::map<std::string, std::string> VariableMap::get_var_map() const
{
    std::map<std::string, std::string> var_map;
    for(auto it : var_list)
    {
        if(!it.second.is_const)
            var_map[it.first] = it.second.expr;
    }
    return var_map;
}
