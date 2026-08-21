#ifndef DICE_PARSER_HPP_GUARD
#define DICE_PARSER_HPP_GUARD

#include <vector>
#include <utility>
#include "parser.hpp"
#include "scanner.hpp"
#include "variable_map.hpp"
#include "dice_parser/parser_result.hpp"
#include "../version.hpp"

class DiceParser
{
private:
    calc::Scanner scanner;
    calc::Parser parser;

    parse_result_t result;
    VariableMap var_map;
public:
    // Version
    static constexpr int version_major{VersionInfo::MAJOR};
    static constexpr int version_minor{VersionInfo::MINOR};
    static constexpr int version_patch{VersionInfo::PATCH};

    // Parser Construction
    DiceParser();
    DiceParser(const VariableMap& map);
    DiceParser(expr_list variables);

    // Parsing Interface
    parse_result_t parse(const std::string& dice_str);
    std::string interpolate(const std::string& intrp_str);

    // Variable Map Access
    VariableMap& get_variable_map();
};

#endif//DICE_PARSER_HPP_GUARD
