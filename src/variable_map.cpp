#include <algorithm>
#include <stdexcept>
#include <variant>
#include <queue>
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
        const VariableMap& p_map
    ) : scanner(p_scanner), parser(p_parser), var_list(p_map.var_list) {}

VariableMap::VariableMap(
        calc::Scanner &p_scanner,
        DiceParser &p_parser,
        expr_list& variables
    ) : scanner(p_scanner), parser(p_parser)
{
    std::map<std::string, std::vector<std::string>> parse_list;
    for(auto v : variables)
        parse_list[v.first] = lex_dependencies(v.second);

    auto parse_order = topological_sort(parse_list);
    for(auto key : parse_order)
    {
        auto it = std::find_if(variables.begin(), variables.end(), [&key](expr_pair& i){ return i.first == key; });
        if(it == variables.cend())
            throw std::runtime_error(key + " not found in either constants nor variables");
        else
        {
            auto val = parser.parse(it->second);
            if(std::holds_alternative<double>(val))
                add_variable(it->first, DiceDistr(std::get<double>(val)), it->second);
            else if(std::holds_alternative<DiceDistr>(val))
                add_variable(it->first, std::get<DiceDistr>(val), it->second);
            else
                throw std::runtime_error(it->second + " does not parse to either double or DiceDistr");
        }
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

bool VariableMap::check_cycles() const
{
    try
    {
        std::map<std::string, std::vector<std::string>> list;
        for(auto i : var_list)
            list.insert({i.first, i.second.dependencies});
        topological_sort(list);
    }
    catch(const std::exception& e)
    {
        return false;
    }
    return true;
}

std::vector<std::string> VariableMap::lex_dependencies(const std::string& expr) const
{
    std::vector<std::string> dependencies;
    scanner.set_input(expr);
    auto sym = scanner.lex();
    while(sym.kind() != calc::Parser::symbol_kind::S_YYEOF)
    {
        if(sym.kind() == calc::Parser::symbol_kind::S_DICE_VARIABLE
        || sym.kind() == calc::Parser::symbol_kind::S_NEW_VARIABLE)
        {
            dependencies.emplace_back(scanner.YYText());
        }
        auto next_sym = scanner.lex();
        sym.move(next_sym);
    }
    return dependencies;
}

void VariableMap::add_node(const std::string& key, DiceDistr val, const std::string& expr)
{
    std::vector<std::string> dependencies;
    if(contains(key))
    {
        dependencies = std::move(var_list[key].dependencies);
    }
    
    var_list.erase(key);
    var_list[key] = UserVar(
        val,
        expr,
        dependencies
    );
    for(auto d : lex_dependencies(expr))
    {
        if(contains(d))
            var_list[d].dependencies.emplace_back(key);
        else
            throw action_code::variable_undefined;
    }
    if(!check_cycles())
        throw action_code::cyclic_graph_err; 
    
    // update other nodes
    std::queue<std::string> update_stack;
    update_stack.push(key);

    while(!update_stack.empty())
    {
        for(auto d : var_list[update_stack.front()].dependencies)
        {
            if(contains(d))
            {
                // the parser is not reentrant, it wipes the scanner state with
                // each call. For this method to work, we need to instantiate a new
                // parser to evaluate the expr. We pass in *this to ensure that the
                // temp parser has the most up to date variable data
                DiceParser temp_parser(*this);
                auto val = temp_parser.parse(var_list[d].expr);
                if(std::holds_alternative<double>(val))
                    var_list[d].value = DiceDistr(std::get<double>(val));
                else if(std::holds_alternative<DiceDistr>(val))
                    var_list[d].value = std::get<DiceDistr>(val);
                else
                    throw action_code::invalid_syntax;
                for(auto nd : var_list[d].dependencies)
                    update_stack.push(nd);
            }
            else
                throw action_code::variable_undefined;
        }
        update_stack.pop();
    }
}

void VariableMap::add_variable(const std::string& key, DiceDistr val, const std::string& expr)
{
    add_node(key, val, expr);
}

void VariableMap::delete_variable(const std::string& key)
{
    if(!contains(key))
        throw action_code::variable_undefined;
    if(var_list.at(key).dependencies.size() > 0)
        throw action_code::delete_dependency_err;
    for(auto& [_, value] : var_list)
        std::erase(value.dependencies, key);
    var_list.erase(key);
}

DiceDistr VariableMap::at(const std::string& key) const
{
    if(!contains(key))
        throw action_code::variable_undefined;
    return var_list.at(key).value;
}

bool VariableMap::contains(const std::string& key) const
{
    return (var_list.find(key) != var_list.end());
}

std::map<std::string, std::string> VariableMap::get_var_map() const
{
    std::map<std::string, std::string> var_map;
    for(auto it : var_list)
    {
        var_map[it.first] = it.second.expr;
    }
    return var_map;
}
