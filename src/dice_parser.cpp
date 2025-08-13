#include "dice_parser.hpp"

DiceParser::DiceParser() : scanner(std::cin, std::cerr), parser(&scanner, result) {}

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
    catch(err_code e)
    {
        result = parse_result_t{e};
    }
    return result;
}
