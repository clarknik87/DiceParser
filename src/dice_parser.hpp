#ifndef DICE_PARSER_HPP_GUARD
#define DICE_PARSER_HPP_GUARD

#include "parser.hpp"
#include "scanner.hpp"
#include "parser_result.hpp"

class DiceParser
{
private:
    calc::Scanner scanner;
    calc::Parser parser;

    std::istringstream istream;
    std::ostringstream errstream;
    parse_result_t result;
public:
    DiceParser();
    parse_result_t parse(const std::string& dice_str);
};

#endif//DICE_PARSER_HPP_GUARD
