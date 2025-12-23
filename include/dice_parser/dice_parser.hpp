#ifndef DICE_PARSER_HPP_GUARD
#define DICE_PARSER_HPP_GUARD

#include "parser.hpp"
#include "scanner.hpp"
#include "variable_map.hpp"
#include "dice_parser/parser_result.hpp"

class DiceParser
{
private:
    calc::Scanner scanner;
    calc::Parser parser;

    std::istringstream istream;
    std::ostringstream errstream;
    parse_result_t result;
    VariableMap var_map;
public:
    DiceParser();
    DiceParser(const VariableMap& map);
    parse_result_t parse(const std::string& dice_str);
    const VariableMap& get_variable_map() const;
    void set_variable_map(const VariableMap& map);
};

#endif//DICE_PARSER_HPP_GUARD
