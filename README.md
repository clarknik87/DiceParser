# DiceParser

DiceParser is a C++ based calculator that handles basic arithmetic with dice formulas and numbers. The project is based on a Bison parser and Flex scanner that allows the user to pass in arbitrary strings for evaluation.

## Example

```
DiceParser parser;

// rolls a 6 sided die, returns a value between 1-6
std::cout << std::get<double>(parser.parse("1d6")) << std::endl;

// rolls two 6 sided dice and sums, returns a value between 2-12
std::cout << std::get<double>(parser.parse("2d6")) << std::endl;

// rolls four 6 sided dice and sums the highest 3, returns a value between 3-18
std::cout << std::get<double>(parser.parse("max(3,4d6)")) << std::endl;

// basic arithmetic is also supported, this returns 70.0
std::cout << std::get<double>(parser.parse("7*10")) << std::endl;

// arithmetic can include dice formualas, this returns a value in the range of 0-5
std::cout << std::get<double>(parser.parse("1d6 - 1")) << std::endl;

```

## Return Type
DiceParser::parse has the following return type:
```
using parse_result_t = std::variant<double, DiceDistr, action_code>;
```
`action_code` is an enum defined in `parser_result.hpp`. This is returned if there is any error in the parsing process, ie unknown symbols, or invalid formual syntax, or if a successful action has no meaningful output, ie variable assignment.

`double` is returned in most cases. Any dice formulas in the string are "rolled" and the remaining math is performed as expected.

`DiceDistr` is returned if the formula is enclosed with the "stats()" function. DiceDistr is a class that contains the full probability density function of the formula entered. For example, "stats(1d6)" with return a DiceDistr representing a uniform distribution between 1 and 6. DiceDistr implements functions like `minimum()`, `maximum()`, `expected_value()`, `variance()`, and `standard_dev()` that can be used to analyze the associated pdf. The associated pdf is stored internally using a 2D Eigen matrix. The second row stores the roll value, the first row stores the asscoiated probability. A standard d4 (four sided die) would look like this:
```
[[0.25 0.25 0.25 0.25],
[1    2    3    4]]
```
Calling `roll()` on a dice formula simulates a roll on that formula.

## Syntax
The parser supports the following kind syntax structures (some based off popular TTRPG language):
- simple math: 4+5, 35/2, round(35/2), sqrt(4), pow(2,3) or any combination: sqrt(pow(3,2) + 56/3)
- simple dice formulas: 1d4, 2d6+3, 1d20-1d4 as well as non traditional dice: 2d3, 1d7, 34d17
- advantage and disadvantage: max(2d20), min(2d20), or simply: adv, dis
- to take the single highest/lowest of many dice use: max(5d20) or min(6d4)
- to take multiple of the highest/lowest of many dice use (eg roll character stats): max(3,4d6) or min(3,4d6)

The parser also supports comparison operations with dice formulas. Comparison operations (>,<,>=, or <=, ==, !=) return the probability of rolling higher/lower/equal to a given number. For example `parse("1d4 > 1")` will return 0.75, meaning a 4 sided die has a 75% chance of rolling higher than a 1.

Finally the parser supports assignment syntax, using a single `=`,  to allow the user to define variables at runtime. For example `parse("my_var = 1d20")` allows the user to later call `parse("my_var")` to roll a twenty sided die.

## Dependencies
The parser requires the user to have the `Bison` and `Flex` utilities installed. The library also depends on the `Eigen` library for the underlying matrix operations.
