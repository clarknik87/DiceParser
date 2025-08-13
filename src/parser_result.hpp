#ifndef PARSER_RESULT_HPP_GUARD
#define PARSER_RESULT_HPP_GUARD

#include <variant>
#include "dice_distribution.hpp"

enum class err_code
{
    unknown_symbol,
    invalid_syntax
};

using parse_result_t = std::variant<double, DiceDistr, err_code>;

#endif//PARSER_RESULT_HPP_GUARD
