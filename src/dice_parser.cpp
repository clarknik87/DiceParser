#include <cmath>
#include "dice_parser/dice_parser.hpp"

constexpr char STOKEN[] = "{";
constexpr char ETOKEN[] = "}"; 

DiceParser::DiceParser() : scanner(std::cin, std::cerr, var_map), parser(&scanner, result, var_map), var_map(scanner, *this) {}
// DiceParser::DiceParser(const VariableMap& map) : scanner(std::cin, std::cerr, var_map), parser(&scanner, result, var_map), var_map(map) {}
DiceParser::DiceParser(expr_list& constants, expr_list& variables) : 
    scanner(std::cin, std::cerr, var_map), parser(&scanner, result, var_map), var_map(scanner, *this, constants, variables) {}

parse_result_t DiceParser::parse(const std::string& dice_str)
{
    scanner.set_input(dice_str);
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

std::string DiceParser::interpolate(const std::string& intrp_str)
{
    std::string str{intrp_str};
    while((str.find(STOKEN) != str.npos) && (str.find(ETOKEN) != str.npos))
    {
        int sidx = str.find(STOKEN);
        int eidx = str.find(ETOKEN);
        std::string parsed_str;
        parse_result_t result = parse(str.substr(sidx+1, eidx-sidx-1));
        if(std::holds_alternative<double>(result))
        {
            double parsed_val = std::get<double>(result);
            if(std::fabs(std::fmod(parsed_val, 1.0)) < 0.00000001)
                parsed_str = std::to_string(static_cast<int>(std::round(parsed_val)));
            else
                parsed_str = std::to_string(parsed_val);
        }
        str.replace(sidx, eidx-sidx+1, parsed_str);
    }
    return str;
}

const VariableMap& DiceParser::get_variable_map() const
{
    return var_map;
}

// void DiceParser::set_variable_map(const VariableMap& map)
// {
//     var_map = map;
// }
