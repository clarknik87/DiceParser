#include "dice_parser/dice_parser.hpp"

DiceParser::DiceParser() : scanner(std::cin, std::cerr, var_map), parser(&scanner, result, var_map) {}
DiceParser::DiceParser(const VariableMap& map) : scanner(std::cin, std::cerr, var_map), parser(&scanner, result, var_map), var_map(map) {}

parse_result_t DiceParser::parse(const std::string& dice_str)
{
    errstream.clear();
    istream.clear();
    istream.str(dice_str);
    scanner.switch_streams(istream, errstream);
    try
    {
        parser.parse();
    }
    catch(action_code e)
    {
        result = parse_result_t{e};
    }
    return result;
}
