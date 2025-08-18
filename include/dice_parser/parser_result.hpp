#ifndef PARSER_RESULT_HPP_GUARD
#define PARSER_RESULT_HPP_GUARD

#include <variant>
#include "dice_distribution.hpp"
#include "dice_parser/action_code.hpp"


using parse_result_t = std::variant<double, DiceDistr, action_code>;

#endif//PARSER_RESULT_HPP_GUARD
