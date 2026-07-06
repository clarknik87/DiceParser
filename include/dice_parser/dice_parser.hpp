#ifndef DICE_PARSER_HPP_GUARD
#define DICE_PARSER_HPP_GUARD

#include <vector>
#include <utility>
#include "parser.hpp"
#include "scanner.hpp"
#include "variable_map.hpp"
#include "dice_parser/parser_result.hpp"

class DiceParser
{
private:
    calc::Scanner scanner;
    calc::Parser parser;

    parse_result_t result;
    VariableMap var_map;
public:
    DiceParser();
    DiceParser(const VariableMap& map);
    DiceParser(expr_list constants, expr_list variables);


    parse_result_t parse(const std::string& dice_str);
    std::string interpolate(const std::string& intrp_str);
    VariableMap& get_variable_map();
};

#endif//DICE_PARSER_HPP_GUARD
