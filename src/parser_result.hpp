#ifndef PARSER_RESULT_HPP_GUARD
#define PARSER_RESULT_HPP_GUARD

#include <variant>
#include "dice_distribution.hpp"

using parse_result_t = std::variant<double, DiceDistr>;

#endif//PARSER_RESULT_HPP_GUARD
