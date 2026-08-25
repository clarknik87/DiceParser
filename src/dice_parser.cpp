#include <cmath>
#include "dice_parser/dice_parser.hpp"

constexpr char EOL[] = ";";
constexpr char STOKEN[] = "{";
constexpr char ETOKEN[] = "}"; 

DiceParser::DiceParser() : scanner(std::cin, std::cerr, var_map), parser(&scanner, result, var_map), var_map(scanner, *this) {}
DiceParser::DiceParser(const VariableMap& map) : scanner(std::cin, std::cerr, var_map), parser(&scanner, result, var_map), var_map(map) {}
DiceParser::DiceParser(expr_list variables) :
    scanner(std::cin, std::cerr, var_map), parser(&scanner, result, var_map), var_map(scanner, *this, variables) {}

parse_result_t DiceParser::parse(const std::string& dice_str)
{
    auto split = [](std::string s){
        std::vector<std::string> tokens;
        size_t pos = 0;
        std::string token;
        while ((pos = s.find(EOL)) != std::string::npos) {
            token = s.substr(0, pos);
            if(!token.empty())
                tokens.push_back(token);
            s.erase(0, pos + (sizeof(EOL)-1));
        }
        if(!s.empty())
            tokens.push_back(s);

        return tokens;
    };

    result = parse_result_t{action_code::empty_command};
    for(auto statement : split(dice_str))
    {
        bool preprocess_err = false;
        scanner.set_input(preprocess(statement, &preprocess_err));
        if(!preprocess_err)
        {
            try
            {
                parser.parse();
            }
            catch(action_code e)
            {
                result = parse_result_t{e};
                break;
            }
        }
        else
        {
            result = parse_result_t{action_code::variable_undefined};
            break;
        }
    }
    return result;
}

std::string DiceParser::preprocess(const std::string& p_str, bool* var_undefined)
{
    std::string str{p_str};
    if(var_undefined)
        *var_undefined = false;
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
        else if(std::holds_alternative<DiceDistr>(result))
        {
            DiceDistr parsed_val = std::get<DiceDistr>(result);
            parsed_str = parsed_val.get_expr();
        }
        else if(std::holds_alternative<action_code>(result))
        {
            action_code parsed_val = std::get<action_code>(result);
            if(parsed_val == action_code::variable_undefined && var_undefined)
                *var_undefined = true;
        }
        str.replace(sidx, eidx-sidx+1, parsed_str);
    }
    return str;
}

std::string DiceParser::interpolate(const std::string& intrp_str)
{
    return preprocess(intrp_str);
}

VariableMap& DiceParser::get_variable_map()
{
    return var_map;
}
